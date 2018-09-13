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

/* Finds are returns the leftmost child.
 * This can be used to find the successor
 * by inputting the right subtree. */
tree get_left_most_child(tree b){
        if (b->left == NULL){
                return b;
        }else{
                return get_left_most_child(b->left);
        }
}

/* Delete by key value. Input the tree and key value to be
 * deleted and the subnode with that key value will be deleted
 * and given root returned. */
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
                /* If current node is a leaf, free it */
                if (b->left == NULL && b->right == NULL){
                        free(b->key);
                        free(b);
                        b = NULL;
                }
                /* If only one child, point to that child */
                else if ((b->left==NULL&&b->right!=NULL)||(b->left!=NULL&&b->right==NULL)){
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
                /* If two children */
                else{
                        tmp = get_left_most_child(b->right);
                        strcpy(b->key, tmp->key);
                        b->right = tree_delete(b->right, tmp->key);
                }
        }
        return b;
}

/* Deletes every node, freeing them as it goes. */
tree tree_free(tree b){
        if (b == NULL){
                return NULL;
        }else{
                return tree_free(tree_delete(b, b->key));
        }
}

void tree_inorder(tree b, void f(char *str, int a)){
    if (b != NULL){
		tree_inorder(b->left, f);
		f(b->key, b->freq);
		tree_inorder(b->right, f);
    }
}

void tree_preorder(tree b, void f(char *str, int a)){
	if (b != NULL){
		f(b->key, b->freq);
		tree_preorder(b->left, f);
		tree_preorder(b->right, f);
	}
}

tree right_rotate(tree t)
{
    tree tmp = t;
    t = t->left;
    tmp->left = t->right;
    t->right = tmp;
    return t;
}

tree left_rotate(tree t)
{
    tree tmp = t;
    t = t->right;
    tmp->right = t->left;
    t->left = tmp;
    return t;
}



/* This restores the red black conditions of the tree.
 *
 * @param b red black binary search tree to fix.
 */
tree rbt_fixup(tree r){
        printf("fixup key: %s\n", r->key);
        /* Case 1: if left child and left's left child are red */
        if (IS_RED(r->left) && IS_RED(r->left->left)){
                if (IS_RED(r->right)){ /* sibling red */
                        r->colour = RED;
                        r->left->colour = BLACK;
                        r->right->colour = BLACK;
                }else if(IS_BLACK(r->right)){ /* sibling black */
                        r->colour = RED;
                        r = right_rotate(r);
                        r->colour = BLACK;
                }
        }
        /* Case 2: if left child and left's right child are red */
        else if(IS_RED(r->left) && IS_RED(r->left->right)){
                if (IS_RED(r->right)){ /* sibling red */
                        r->colour = RED;
                        r->left->colour = BLACK;
                        r->right->colour = BLACK;
                }else if(IS_BLACK(r->right)){
                        r->colour = RED;
                        r->left = left_rotate(r->left);
                        r = right_rotate(r);
                        r->colour = BLACK;
                }
        }
        /* Case 3: if right child and right's left child are red */
        else if(IS_RED(r->right) && IS_RED(r->right->left)){
                if (IS_RED(r->left)){
                        r->colour = RED;
                        r->left->colour = BLACK;
                        r->right->colour = BLACK;
                }else if(IS_BLACK(r->left)){
                        r->right = right_rotate(r->right);
                        r->colour = RED;
                        r = left_rotate(r);
                        r->colour = BLACK;
                }
        }
        /* Case 4: if right child and right's right child are red */
        else if(IS_RED(r->right) && IS_RED(r->right->right)){
                if (IS_RED(r->left)){
                        r->colour = RED;
                        r->right->colour = BLACK;
                        r->left->colour = BLACK;
                }else if(IS_BLACK(r->left)){
                        r->colour = RED;
                        r = left_rotate(r);
                        r->colour = BLACK;
                }
        }
        return r;
}

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
                rbt_fixup(b);
        }
        return b;
}

tree tree_new(tree_t t){
        tree result = emalloc(sizeof *result);
        result->left = NULL;
        result->right = NULL;
        result->key = NULL;
        result->freq = 0;
        result->type = t;
        return result;
}

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

/**
 * Traverses the tree writing a DOT description about connections, and
 * possibly colours, to the given output stream.
 *
 * @param t the tree to output a DOT description of.
 * @param out the stream to write the DOT output to.
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

/**
 * Output a DOT description of this tree to the given output stream.
 * DOT is a plain text graph description language (see www.graphviz.org).
 * You can create a viewable graph with the command
 *
 *    dot -Tpdf < graphfile.dot > graphfile.pdf
 *
 * You can also use png, ps, jpg, svg... instead of pdf
 *
 * @param t the tree to output the DOT description of.
 * @param out the stream to write the DOT description to.
 */
void tree_output_dot(tree t, FILE *out) {
   fprintf(out, "digraph tree {\nnode [shape = Mrecord, penwidth = 2];\n");
   tree_output_dot_aux(t, out);
   fprintf(out, "}\n");
}

void tree_print_key(char *s, int f){
	printf("%d    %s\n",f, s);
}
