#include "eval.h"

int eval(struct ast *root, struct file *file, struct statut *statut)
{
    if (root->type == TOKEN_NEGATION)
        return !eval(root->left, file, statut);
    
    if (root->type == TOKEN_PRINT)
        return 1;

    if (root->type == TOKEN_NAME)
        return name(root->value, file, statut);
    if (root->type == TOKEN_TYPE)
        return type(root->value, file, statut);
    if (root->type == TOKEN_NEWER)
        return newer(root->value, file, statut);

    if (root->type == TOKEN_EXEC)
        return exec(root->value, file, statut);
    if (root->type == TOKEN_EXEC_PLUS)
        return 0;
    if (root->type == TOKEN_EXECDIR)
        return exec_dir(root->value, file, statut);

    if (root->type == TOKEN_DELETE)
        return delete(root->value, file, statut);
    if (root->type == TOKEN_PERM) // Not implemented
        return 0;
    if (root->type == TOKEN_USER) // Not implemented
        return 0;
    if (root->type == TOKEN_GROUP)
        return group(root->value, file, statut);
    
    if (root->type == TOKEN_OR)
    {
        if (eval(root->left, file, statut) == 1)
            return 1;
        else
            return eval(root->right, file, statut);
    }
    if (root->type == TOKEN_AND)
    {
        if (eval(root->left, file, statut) == 0)
            return 0;
        else
            return eval(root->right, file, statut);
    }
    fprintf(stderr, "my_find: Unexpected token !\n");
    return 0;
}