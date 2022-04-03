#pragma once

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum token_type
{
    TOKEN_OR,
    TOKEN_AND,
    TOKEN_NEGATION,
    TOKEN_LEFT_PARENTHESE,
    TOKEN_RIGHT_PARENTHESE,
    TOKEN_PRINT,
    TOKEN_NAME,
    TOKEN_TYPE,
    TOKEN_NEWER,
    TOKEN_EXEC,
    TOKEN_EXECDIR,
    TOKEN_EXEC_PLUS,
    TOKEN_DELETE,
    TOKEN_PERM,
    TOKEN_USER,
    TOKEN_GROUP,
    TOKEN_ARGUMENT,
    TOKEN_SEMICOLON,
    TOKEN_PLUS,
    TOKEN_EOF
};

struct token
{
    enum token_type type;
    char *value;
};

/**
 * \brief Allocate a new token
 */
struct token *token_new(enum token_type type);

/**
 * \brief Frees a token
 */
void token_free(struct token *token);

/**
 * \brief Compare a token 'type' with a list of tokens 'token_type'
 */
int token_cmp(enum token_type *token_type, enum token_type type, size_t t);

/**
 * \brief Print a token
 */
char *token_print(enum token_type type);