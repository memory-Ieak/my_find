#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

int _strcmp(char *str, char **argv, size_t size);

void free_cpy_str(char **str);

char **substitution(char **str, char *path);

char **substitution_dir(char **str, char *filename);

void free_str(char **str);

void print_str(char **str);

char **correct_exp(char **argv, int argc);