#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct vector
{
    size_t capacity;
    size_t size;
    char **data;
};

struct vector *vector_init(void);

struct vector *vector_add(struct vector *vec, char *str);

char **vector_extract_data(struct vector *vec);

void vector_print(struct vector *vec);