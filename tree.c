#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "mylib.h"
#include <string.h>

#define IS_BLACK(x) ((NULL == (x)) || (BLACK == (x)->colour))
#define IS_RED(x) ((NULL !=(x)) && (RED == (x)->colour))

struct treenode{
    char *key;
    tree left;
    tree right;
    int freq;
    tree_t type;
    rbt_colour colour;
};

/* Finds and returns the leftmost child.
 * This can be used to find the successor
 * by inputting the right subtree
 
 * @param b the tree to be worked on
 *
 * @return the leftmost child
 */
tree get_left_most_child(tree b){
    if (b->left == NULL){
        return b;
    }else{
        return get_left_most_child(b->left);
    }
}

/* Delete by key value. Input the tree and key value to be
 * deleted and the subnode with that key value will be deleted
 * and given root returned
 *
 * @param b the tree to be worked on
 * @param str the key value to be deleted
 *
 * @return root
 */
tree tree_delete(tree b, char *str){
    tree tmp;
    if (tree_search(b, str) == 0){
        return b; /* nothing to delete */
    }else if(strcmp(b->key, str) < 0){
        b->right = tree_delete(b->right, str);
        return b;
    }else if(strcmp(b->key, str) > 0){
        b->left = tree_delete(b->left, str);
        return b;
    }else{
        /* if current node is a leaf, free it */
        if (b->left == NULL && b->right == NULL){
            free(b->key);
            free(b);
            b = NULL;
        }
        /* if only one child, point to that child */
        else if ((b->left==NULL&&b->right!=NULL)||
                 (b->left!=NULL&&b->right==NULL)){
            free(b->key);
            tmp = b;
            if (b->left != NULL){
                b = b->left;
            }
            else if (b->right != NULL){
                b = b->right;
            }
            free(tmp);
        }
        /* if two children */
        else{
            tmp = get_left_most_child(b->right);
            strcpy(b->key, tmp->key);
            b->right = tree_delete(b->right, tmp->key);
        }
    }
    return b;
}

/* Deletes every node, freeing them as it goes.
 *
 * @param b the tree to be worked on
 *
 * @return an empty tree
 */
tree tree_free(tree b){
    if (b == NULL){
        return NULL;
    }else{
        return tree_free(tree_delete(b, b->key));
    }
}

/* An inorder traversal of the tree.
 *
 * @param b the tree to be worked on
 */
void tree_inorder(tree b, void f(char *str, int a)){
    if (b != NULL){
        tree_inorder(b->left, f);
        f(b->key, b->freq);
        tree_inorder(b->right, f);
    }
}

/* A preorder traversal of the tree.
 *
 * @param b the tree to be worked on
 */
void tree_preorder(tree b, void f(char *str, int a)){
    if (b != NULL){
        f(b->key, b->freq);
        tree_preorder(b->left, f);
        tree_preorder(b->right, f);
    }
}

/* Right-rotation moves branches from left to right.
 *
 * @param t current value of the tree
 *
 * @return new value of the tree
 */
tree right_rotate(tree t){
    tree tmp = t;
    t = t->left;
    tmp->left = t->right;
    t->right = tmp;
    return t;
}

/* Left-rotation moves branches from right to left.
 *
 * @param t current value of the tree
 *
 * @return new value of the tree
 */
tree left_rotate(tree t){
    tree tmp = t;
    t = t->right;
    tmp->right = t->left;
    t->left = tmp;
    return t;
}

/* Swap function.
 *
 * @param t current value of the tree
 *
 * @return new value of the tree
 */
tree swap(tree t){
    t->colour = RED;
    t->left->colour = BLACK;
    t->right->colour = BLACK;
    return t;
}

/* Tree fix function.
 *
 * @param t current value of the tree
 *
 * @return new value of the tree
 */
static tree tree_fix(tree t){
    if(IS_RED(t->left) && IS_RED(t->left->left)){
        if(IS_RED(t->right)){
            swap(t);
        }else if(IS_BLACK(t->right)){
            /* right rotate root , colour new root (a) black,
             * and new child(old root) red*/
            t = right_rotate(t);
            t->colour = BLACK;
            t->right->colour = RED;/*old root*/
        }
        
    }else if(IS_RED(t->left) && IS_RED(t->left->right)){
        if(IS_RED(t->right)){
            swap(t);
        }
        else if(IS_BLACK(t->right)){
            /* left rotate left child (a), right rotate root (r),
             * colour new root (d) black and new child (R) red */
            t->left = left_rotate(t->left);
            t = right_rotate(t);
            t->colour = BLACK;
            t->right->colour = RED;/* old root */
        }
        
    }else if(IS_RED(t->right) && IS_RED(t->right->left)){
        if(IS_RED(t->left)){
            swap(t);
        }else if(IS_BLACK(t->left)){
            /* right rotate right child(b), left rotate root(r),
             * colour new root (e) black and new child (r) red */
            t->right = right_rotate(t->right);
            t = left_rotate(t);
            t->colour = BLACK;
            t->left->colour = RED;/* old root */
        }
        
    }else if(IS_RED(t->right) && IS_RED(t->right->right)){
        if(IS_RED(t->left)){
            swap(t);
        }
        else if(IS_BLACK(t->left)){
            /* left rotate root R, colour new root b black and new child R red */
            t = left_rotate(t);
            t->colour = BLACK;
            t->left->colour = RED;/*old root*/
        }
    }
    return t;
}

/* Makes the node black. This should be called on the root node after each
 * insertion.
 *
 * @param t the node to turn black
 */
tree tree_make_black(tree t){
	t->colour = BLACK;
	return t
}

/* Tree insert function. Allocates memory.
 *
 * @param b the current value of the tree.
 * @param str pointer to array of char to be inserted
 * @param t for emalloc
 *
 * @return new value of the tree
 */
tree tree_insert(tree b, char *str, tree_t t){
    if (b == NULL){ /* empty tree, base case */
        b = tree_new(t);
        b->key = emalloc((strlen(str)+1) * sizeof b->key[0]);
        strcpy(b->key, str);
        b->freq = 1;
        b->colour = RED;
        return b;
    }else if (strcmp(b->key, str) < 0){
        b->right = tree_insert(b->right, str, t);
    }else if (strcmp(b->key, str) > 0){
        b->left = tree_insert(b->left, str, t);
    }else{
        /* we have a duplicated item, increase freq */
        b->freq++;
    }
    if (b->type == RBT){
        b = tree_fix(b);
    }
    return b;
}

/* Tree new function to empty the tree.
 *
 * @param t current value of the tree
 *
 * @result new value of the tree
 */
tree tree_new(tree_t t){
    tree result = emalloc(sizeof *result);
    result->left = NULL;
    result->right = NULL;
    result->key = NULL;
    result->freq = 0;
    result->type = t;
    return result;
}

/* Tree search function traverses the tree.
 *
 * @param b the tree to be worked on
 * @param str char pointer of value to be searched for
 */
int tree_search(tree b, char *str){
    if (b == NULL){
        return 0;
    }else if (strcmp(str, b->key) == 0){
        return 1;
    }else if (strcmp(str, b->key) > 0){
        return tree_search(b->right, str);
    }else{
        return tree_search(b->left, str);
    }
}

/* Traverses the tree writing a DOT description about connections, and
 * possibly colours, to the given output stream.
 *
 * @param t the tree to output a DOT description of
 * @param out the stream to write the DOT output to
 */
static void tree_output_dot_aux(tree t, FILE *out) {
    if(t->key != NULL) {
        fprintf(out, "\"%s\"[label=\"{<f0>%s:%d|{<f1>|<f2>}}\"color=%s];\n",
                t->key, t->key, t->freq,
                (RBT == t->type && RED == t->colour) ? "red":"black");
    }
    if(t->left != NULL) {
        tree_output_dot_aux(t->left, out);
        fprintf(out, "\"%s\":f1 -> \"%s\":f0;\n", t->key, t->left->key);
    }
    if(t->right != NULL) {
        tree_output_dot_aux(t->right, out);
        fprintf(out, "\"%s\":f2 -> \"%s\":f0;\n", t->key, t->right->key);
    }
}

/* Output a DOT description of this tree to the given output stream.
 * DOT is a plain text graph description language (see www.graphviz.org).
 * You can create a viewable graph with the command
 *
 *    dot -Tpdf < graphfile.dot > graphfile.pdf
 *
 * You can also use png, ps, jpg, svg... instead of pdf
 *
 * @param t the tree to output the DOT description of
 * @param out the stream to write the DOT description to
 */
void tree_output_dot(tree t, FILE *out) {
    fprintf(out, "digraph tree {\nnode [shape = Mrecord, penwidth = 2];\n");
    tree_output_dot_aux(t, out);
    fprintf(out, "}\n");
}

/* Print key function prints to stdout.
 *
 * @param s char pointer
 * @param f
 */
void tree_print_key(char *s, int f){
    printf("%d    %s\n",f, s);
}
