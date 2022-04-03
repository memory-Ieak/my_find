#include "parser.h"

static enum parser_status parse_exp(struct ast **res, struct lexer *lexer);
static enum parser_status parse_sexp(struct ast **res, struct lexer *lexer);
static enum parser_status parse_texp(struct ast **res, struct lexer *lexer);
static enum parser_status parse_command(struct ast **res, struct lexer *lexer);
static enum parser_status parse_parenthesis(struct ast **res,
                                            struct lexer *lexer);

static enum parser_status handle_parse_error(enum parser_status status,
                                             struct ast **res)
{
    fprintf(stderr, "my_find: unexpected token\n");
    ast_free(*res);
    *res = NULL;
    return status;
}

/**
 * \brief Parse either an expression, or nothing
 *
 * input:  EOF  |  exp EOF
 */
enum parser_status parse(struct ast **res, struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_EOF)
    {
        *res = NULL;
        return PARSER_OK;
    }

    enum parser_status status = parse_exp(res, lexer);
    
    if (status != PARSER_OK)
        return handle_parse_error(status, res);
    
    if (lexer_peek(lexer)->type == TOKEN_EOF)
        return PARSER_OK;

    return handle_parse_error(PARSER_UNEXPECTED_TOKEN, res);
}

/**
 * \brief Parse sexp expressions separated by + and -
 *
 * exp:      sexp  ('-or' sexp)*
 */
static enum parser_status parse_exp(struct ast **res, struct lexer *lexer)
{
    enum parser_status status = parse_sexp(res, lexer);
    if (status != PARSER_OK)
        return status;

    while (true)
    {
        struct token *tok = lexer_peek(lexer);
        if (tok->type != TOKEN_OR)
            return PARSER_OK;

        struct ast *new_ast;
        new_ast = ast_new(TOKEN_OR);

        token_free(lexer_pop(lexer));

        new_ast->left = *res;
        *res = new_ast;

        if ((status = parse_sexp(&new_ast->right, lexer)) != PARSER_OK)
            return status;
    }
}

/**
 * \brief Parse texp expressions separated by * and /
 *
 * sexp:      texp  ('-a' texp)*
 */
static enum parser_status parse_sexp(struct ast **res, struct lexer *lexer)
{
    enum parser_status status = parse_texp(res, lexer);
    if (status != PARSER_OK)
        return status;

    while (true)
    {
        struct token *tok = lexer_peek(lexer);
        if (tok->type != TOKEN_AND)
            return PARSER_OK;

        struct ast *new_ast;
        new_ast = ast_new(TOKEN_AND);

        token_free(lexer_pop(lexer));

        new_ast->left = *res;
        *res = new_ast;

        if ((status = parse_texp(&new_ast->right, lexer)) != PARSER_OK)
            return status;
    }
}

/**
 * \brief Parse either a number, a - a number, or a parenthesized expression
 *
 * texp:      command WORD+
 *         |  '!' command WORD+
 *         |  '(' exp ')'
 *         |  '!' '(' exp ')'
 */
static enum parser_status parse_texp(struct ast **res, struct lexer *lexer)
{
    if (lexer_peek(lexer)->type == TOKEN_NEGATION)
    {
        token_free(lexer_pop(lexer));
        struct ast *negate_node = ast_new(TOKEN_NEGATION);
        *res = negate_node;
        res = &negate_node->left;
    }

    if (lexer_peek(lexer)->type == TOKEN_PRINT
        || lexer_peek(lexer)->type == TOKEN_NAME
        || lexer_peek(lexer)->type == TOKEN_TYPE
        || lexer_peek(lexer)->type == TOKEN_NEWER
        || lexer_peek(lexer)->type == TOKEN_EXEC
        || lexer_peek(lexer)->type == TOKEN_EXECDIR
        || lexer_peek(lexer)->type == TOKEN_EXEC_PLUS
        || lexer_peek(lexer)->type == TOKEN_PERM
        || lexer_peek(lexer)->type == TOKEN_DELETE
        || lexer_peek(lexer)->type == TOKEN_GROUP
        || lexer_peek(lexer)->type == TOKEN_USER)
        return parse_command(res, lexer);

    if (lexer_peek(lexer)->type == TOKEN_LEFT_PARENTHESE)
        return parse_parenthesis(res, lexer);

    *res = NULL;
    return PARSER_UNEXPECTED_TOKEN;
}

/**
 * \brief Parse a command. Assumes the next token is a command.
 */
static enum parser_status parse_command(struct ast **res, struct lexer *lexer)
{
    struct token *tok = lexer_pop(lexer);
    struct vector *vec = vector_init();

    enum token_type tokens_type[] = { TOKEN_NAME, TOKEN_TYPE, TOKEN_NEWER,
                                      TOKEN_USER, TOKEN_GROUP, TOKEN_PERM };
    size_t tokens_type_size = 6;

    if (tok->type == TOKEN_PRINT)
        vector_add(vec, NULL);
    
    else if (token_cmp(tokens_type, tok->type, tokens_type_size)
             && lexer_peek(lexer)->type == TOKEN_ARGUMENT)
    {
        vector_add(vec, lexer_peek(lexer)->value);
        vector_add(vec, NULL);
        token_free(lexer_pop(lexer));
    }

    else if (tok->type == TOKEN_DELETE)
    {
        if (lexer_peek(lexer)->type == TOKEN_ARGUMENT)
            token_free(lexer_pop(lexer));
    }
    
    else if ((tok->type == TOKEN_EXEC || tok->type == TOKEN_EXECDIR)
             && lexer_peek(lexer)->type == TOKEN_ARGUMENT)
    {
        size_t i = 0;
        char cmp = 0;
        while (lexer_peek(lexer)->type == TOKEN_ARGUMENT)
        {
            if (!strcmp(lexer_peek(lexer)->value, "{}"))
                i++;
            else if (i > 0)
                cmp = 1;
            vector_add(vec, lexer_peek(lexer)->value);
            token_free(lexer_pop(lexer));
        }
        
        vector_add(vec, NULL);

        if (lexer_peek(lexer)->type == TOKEN_SEMICOLON
            && (tok->type == TOKEN_EXEC || tok->type == TOKEN_EXECDIR))
        {
            token_free(lexer_pop(lexer));
        }

        else if (lexer_peek(lexer)->type == TOKEN_PLUS
                 && tok->type == TOKEN_EXEC
                 && i == 1 && cmp == 0)
        {
            tok->type = TOKEN_EXEC_PLUS;
            token_free(lexer_pop(lexer));
        }
        else
        {
            free_str(vector_extract_data(vec));
            token_free(tok);
            return PARSER_UNEXPECTED_TOKEN;
        }
    }

    else
    {
        free_str(vector_extract_data(vec));
        token_free(tok);
        return PARSER_UNEXPECTED_TOKEN;
    }


    struct ast *ast = ast_new(tok->type);
    ast->value = vector_extract_data(vec);
    *res = ast;

    token_free(tok);
    return PARSER_OK;
}

/**
 * \brief Parse a parenthesized expression. Assumes the next token is an opening
 * parenthesis.
 */
static enum parser_status parse_parenthesis(struct ast **res,
                                            struct lexer *lexer)
{
    token_free(lexer_pop(lexer));

    enum parser_status status = parse_exp(res, lexer);
    if (status != PARSER_OK)
        return status;

    if (lexer_peek(lexer)->type != TOKEN_RIGHT_PARENTHESE)
        return PARSER_UNEXPECTED_TOKEN;

    token_free(lexer_pop(lexer));
    return PARSER_OK;
}
