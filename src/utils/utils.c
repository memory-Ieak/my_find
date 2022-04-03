#include "utils.h"

int _strcmp(char *str, char **argv, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        if (!strcmp(str, argv[i]))
        {
            return 1;
        }
    }
    return 0;
}

size_t len_str(char **str)
{
    size_t i = 0;
    while (str[i])
        i++;
    return i;
}

void free_cpy_str(char **str)
{
    free(str);
}

char **substitution(char **str, char *path)
{
    char **cpy = malloc(sizeof(char *) * (len_str(str) + 1));
    size_t i = 0;
    while (str[i])
    {
        if (!strcmp(str[i], "{}"))
            cpy[i] = path;
        else
            cpy[i] = str[i];
        i++;
    }
    cpy[i] = NULL;
    return cpy;
}

char **substitution_dir(char **str, char *filename)
{
    char **cpy = malloc(sizeof(char *) * (len_str(str) + 1)); // +1 is for NULL
    size_t i = 0;
    while (str[i])
    {
        if (!strcmp(str[i], "{}"))
        {
            char *new = malloc(sizeof(char) * (strlen(filename) + 3)); // +3 is for ./ + \0
            snprintf(new, strlen(filename) + 3, "./%s", filename);
            cpy[i] = new;
        }
        else
        {
            char *new = malloc(sizeof(char) * (strlen(str[i]) + 1)); // +1 is for \0
            cpy[i] = strcpy(new, str[i]);
        }
        i++;
    }
    cpy[i] = NULL;
    return cpy;
}

void free_str(char **str)
{
    if (str)
    {
        size_t i = 0;
        while (str[i])
        {
            free(str[i]);
            i++;
        }
        free(str);
    }
}

void print_str(char **str)
{
    if (str)
    {
        size_t i = 0;
        while (str[i])
        {
            printf("%s ", str[i]);
            i++;
        }
    }
}

char **correct_exp(char **argv, int argc)
{
    struct vector *vec = vector_init();

    char *begin[] = { "!", "(", ")" };
    size_t begin_size = sizeof(begin) / sizeof(*begin);

    char *command[] = {"-newer", "-name", "-type",
                       "-print", "-exec", "-execdir",
                       "-delete", "-user", "-group",
                       "-perm" };
    size_t command_size = sizeof(command) / sizeof(*command);

    while (argc > 0
           && (_strcmp(*argv, begin, begin_size)
           || _strcmp(*argv, command, command_size)))
    {
        vec = vector_add(vec, *argv);
        argv++;
        argc--;
    }

    int i = 0;
    for (i = 0; i < argc; i++)
    {
        vec = vector_add(vec, argv[i]);
        if (i < argc - 1)
        {
            if ((strcmp("-o", argv[i]) && strcmp("-a", argv[i]) 
                && strcmp("!", argv[i]) && strcmp("(", argv[i]))
                && (!strcmp("!", argv[i + 1]) || !strcmp("(", argv[i + 1])
                || _strcmp(argv[i + 1], command, command_size)))
                vec = vector_add(vec, "-a");
        }
    }
    vec = vector_add(vec, "\0");
    vec = vector_add(vec, NULL);
    //vector_print(vec);
    return vector_extract_data(vec);
}