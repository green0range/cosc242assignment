#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include "mylib.h"
#include <string.h>

struct bstnode{
	char *key;
	bst left;
	bst right;
        int freq;
        tree_t type;
};

/* Finds are returns the leftmost child.
 * This can be used to find the successor
 * by inputting the right subtree. */
bst get_left_most_child(bst b){
        if (b->left == NULL){
                return b;
        }else{
                return get_left_most_child(b->left);
        }
}

/* Delete by key value. Input the bst and key value to be
 * deleted and the subnode with that key value will be deleted
 * and given root returned. */
bst bst_delete(bst b, char *str){
        bst tmp;
        if (bst_search(b, str) == 0){
                return b; /* nothing to delete */
        }else if(strcmp(b->key, str) < 0){
                b->right = bst_delete(b->right, str);
                return b;
        }else if(strcmp(b->key, str) > 0){
                b->left = bst_delete(b->left, str);
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
                        b->right = bst_delete(b->right, tmp->key);
                }
        }
        return b;
}

/* Deletes every node, freeing them as it goes. */
bst bst_free(bst b){
        if (b == NULL){
                return NULL;
        }else{
                return bst_free(bst_delete(b, b->key));
        }
}

void bst_inorder(bst b, void f(char *str, int a)){
    if (b != NULL){
		bst_inorder(b->left, f);
		f(b->key, b->freq);
		bst_inorder(b->right, f);
    }
}

void bst_preorder(bst b, void f(char *str, int a)){
	if (b != NULL){
		f(b->key, b->freq);
		bst_preorder(b->left, f);
		bst_preorder(b->right, f);
	}
}

bst bst_insert(bst b, char *str, tree_t t){
	if (b == NULL){ /* empty tree, base case */
	        b = bst_new(t);
		b->key = emalloc((strlen(str)+1) * sizeof b->key[0]);
                strcpy(b->key, str);
                b->freq = 1;
		return b;
	}else if (strcmp(b->key, str) < 0){
		b->right = bst_insert(b->right, str, t);
		return b;
	}else if (strcmp(b->key, str) > 0){
		b->left = bst_insert(b->left, str, t);
		return b;
	}else{
		/* we have a duplicated item, increase freq */
                b->freq++;
		return b;
	}
}

bst bst_new(tree_t t){
        bst result = emalloc(sizeof *result);
        result->left = NULL;
        result->right = NULL;
        result->key = NULL;
        result->freq = 0;
        result->type = t;
        return result;
}

int bst_search(bst b, char *str){
	if (b == NULL){
		return 0;
	}else if (strcmp(str, b->key) == 0){
		return 1;
	}else if (strcmp(str, b->key) > 0){
		return bst_search(b->right, str);
	}else{
		return bst_search(b->left, str);
	}
}

void bst_print_key(char *s, int f){
	printf("%d    %s\n",f, s);
}
