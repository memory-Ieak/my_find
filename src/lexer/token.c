#include "token.h"

struct token *token_new(enum token_type type)
{
    struct token *new = malloc(sizeof(struct token));
    new->type = type;
    new->value = NULL;
    return new;
}

void token_free(struct token *token)
{
    free(token);
}

int token_cmp(enum token_type *tokens_type, enum token_type type, size_t t)
{
    for (size_t i = 0; i < t; i++)
        if (tokens_type[i] == type)
            return 1;
    return 0;
}

char *token_print(enum token_type type)
{
    switch (type)
    {
        case TOKEN_OR:
            return "or      ";
        case TOKEN_AND:
            return "and     ";
        case TOKEN_NEGATION:
            return "!       ";
        case TOKEN_LEFT_PARENTHESE:
            return "(       ";
        case TOKEN_RIGHT_PARENTHESE:
            return ")       ";
        case TOKEN_SEMICOLON:
            return ";       ";
        case TOKEN_PRINT:
            return "-print  ";
        case TOKEN_NAME:
            return "-name   ";
        case TOKEN_TYPE:
            return "-type   ";
        case TOKEN_NEWER:
            return "-newer  ";
        case TOKEN_EXEC:
            return "-exec   ";
         case TOKEN_EXEC_PLUS:
            return "-exec + ";
        case TOKEN_EXECDIR:
            return "-execdir";
        case TOKEN_PERM:
            return "-perm   ";
        case TOKEN_DELETE:
            return "-delete ";
        case TOKEN_USER:
            return "-user   ";
        case TOKEN_GROUP:
            return "-group  ";
        case TOKEN_ARGUMENT:
            return "WORD+   ";
        case TOKEN_EOF:
            return "EOF     ";
        default:
            return "ERROR   ";
    }
}