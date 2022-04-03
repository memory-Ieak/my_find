#include "lexer.h"

struct lexer *lexer_get_token(struct lexer *lexer);

struct lexer *lexer_new(char **input)
{
    struct lexer *lexer = malloc(sizeof(struct lexer));
    lexer->input = input;
    lexer->pos = 0;
    lexer->current_token = NULL;
    return lexer_get_token(lexer);
}

void lexer_free(struct lexer *lexer)
{
    token_free(lexer_peek(lexer));
    free(lexer);
}

char *get_word(struct lexer *lexer)
{
    return lexer->input[lexer->pos];
}

struct lexer *lexer_get_token(struct lexer *lexer)
{
    struct token tokens[] =
    {
        { TOKEN_OR, "-o" },
        { TOKEN_AND, "-a" },
        { TOKEN_NEGATION, "!" },
        { TOKEN_LEFT_PARENTHESE, "(" },
        { TOKEN_RIGHT_PARENTHESE, ")" },
        { TOKEN_SEMICOLON, ";" },
        { TOKEN_PLUS, "+" },
        { TOKEN_PRINT, "-print" },
        { TOKEN_NAME, "-name" },
        { TOKEN_TYPE, "-type" },
        { TOKEN_NEWER, "-newer" },
        { TOKEN_EXEC, "-exec" },
        { TOKEN_EXECDIR, "-execdir" },
        { TOKEN_DELETE, "-delete" },
        { TOKEN_PERM, "-perm" },
        { TOKEN_USER, "-user" },
        { TOKEN_GROUP, "-group" },
        { TOKEN_EOF, "\0" }
    };
    char *word = get_word(lexer);
    struct token *token = token_new(TOKEN_ARGUMENT);
    token->value = word;
    for (unsigned i = 0; i < sizeof(tokens) / sizeof(*tokens); i++)
    {
        if (!strcmp(tokens[i].value, word))
        {
            token->type = tokens[i].type;
            break;
        }
    }
    lexer->current_token = token;
    lexer->pos++;
    return lexer;
}

struct token *lexer_peek(struct lexer *lexer)
{
    return lexer->current_token;
}

struct token *lexer_pop(struct lexer *lexer)
{
    struct token *previous_token = lexer->current_token;
    lexer = lexer_get_token(lexer);
    return previous_token;
}
