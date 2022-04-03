#include "vector.h"

struct vector *vector_init(void)
{
    struct vector *vec = malloc(sizeof(struct vector));
    vec->capacity = 10;
    vec->size = 0;
    vec->data = malloc(sizeof(char*) * vec->capacity);
    *vec->data = NULL;
    return vec;
}

struct vector *vector_add(struct vector *vec, char *str)
{
    if (vec->size == vec->capacity - 1)
    {
        vec->capacity *= 2;
        vec->data = realloc(vec->data, sizeof(char*) * vec->capacity);
    }
    char *cpy = NULL;
    if (str)
    {
        cpy = malloc(sizeof(char) * (strlen(str) + 1));
        cpy = strcpy(cpy, str);
    }
    vec->data[vec->size] = cpy;
    vec->size++;
    return vec;
}

char **vector_extract_data(struct vector *vec)
{
    char **str = vec->data;
    free(vec);
    return str;
}

void vector_print(struct vector *vec)
{
    for (size_t i = 0; i < vec->size; i++)
        (i < vec->size - 1) ? printf("%s ", vec->data[i]) : printf("%s\n", vec->data[i]);
}