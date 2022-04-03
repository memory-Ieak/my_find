#include "ast.h"

#define COUNT 10

struct ast *ast_new(enum token_type type)
{
    struct ast *new = malloc(sizeof(struct ast));
    new->type = type;
    new->value = NULL;
    new->left = NULL;
    new->right = NULL;
    return new;
}

void ast_free(struct ast *ast)
{
    if (ast == NULL)
        return;
        
    free_str(ast->value);

    ast_free(ast->left);
    ast->left = NULL;

    ast_free(ast->right);
    ast->right = NULL;

    free(ast);
}

void _ast_print(struct ast *root, int space)
{
    if (root == NULL)
        return;
 
    space += COUNT;
 
    _ast_print(root->right, space);
 
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%s ", token_print(root->type));
    print_str(root->value);
    printf("\n");

    _ast_print(root->left, space);
}

void ast_print(struct ast *root)
{
   _ast_print(root, 0);
}