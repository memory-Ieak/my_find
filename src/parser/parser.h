#pragma once

#include <stdbool.h>

#include "lexer.h"
#include "ast.h"
#include "vector.h"

enum parser_status
{
    PARSER_OK,
    PARSER_UNEXPECTED_TOKEN,
};

/**
 * \brief Creates an AST by reading and interpreting tokens
 *   from the parser.
 */
enum parser_status parse(struct ast **res, struct lexer *lexer);
