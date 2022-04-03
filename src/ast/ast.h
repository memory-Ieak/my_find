#pragma once

#include <unistd.h>
#include <stdlib.h>

#include "token.h"
#include "utils.h"

struct ast
{
    enum token_type type;
    char **value;
    struct ast *left;
    struct ast *right;
};

/**
 ** \brief Allocate a new ast with the given type
 */
struct ast *ast_new(enum token_type type);

/**
 ** \brief Recursively free the given ast
 */
void ast_free(struct ast *ast);


/**
 ** \brief Recursively print the given ast
 */
void ast_print(struct ast *root);
