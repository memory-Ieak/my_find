#pragma once

#include <dirent.h>

#include "ast.h"
#include "action.h"
#include "file.h"
#include "statut.h"

/**
 ** \brief Evaluate the AST
 */
int eval(struct ast *root, struct file *file, struct statut *statut);