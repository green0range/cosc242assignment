#ifndef BST_H_
#define BST_H_

typedef struct bstnode *bst;
typedef enum tree_e { BST, RBT } tree_t;
typedef enum rbt_colour_e { RED, BLACK } rbt_colour;

extern bst bst_delete(bst b, char *str);
extern bst bst_free(bst b);
extern void bst_inorder(bst b, void f(char *str, int f));
extern bst bst_insert(bst b, char *str, tree_t t);
extern bst bst_new();
extern void bst_preorder(bst b, void f(char *str, int f));
extern int bst_search(bst b, char *str);
extern void bst_print_key(char *str, int f);

#endif
