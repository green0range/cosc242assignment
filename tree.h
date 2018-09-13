#ifndef TREE_H_
#define TREE_H_

typedef struct treenode *tree;
typedef enum tree_e { BST, RBT } tree_t;
typedef enum rbt_colour_e { RED, BLACK } rbt_colour;

extern tree tree_delete(tree b, char *str);
extern tree tree_free(tree b);
extern void tree_inorder(tree b, void f(char *str, int f));
extern tree tree_insert(tree b, char *str, tree_t t);
extern tree tree_new();
extern void tree_preorder(tree b, void f(char *str, int f));
extern int tree_search(tree b, char *str);
extern void tree_print_key(char *str, int f);
extern void tree_output_dot(tree t, FILE *out);

#endif
