#pragma once

#include <stdio.h>
#include <stddef.h>
#include <string.h>

struct option
{
    char d;
    char H;
    char L;
    char P;
};

struct option option_init(void)
{
    struct option option;
    option.d = 0;
    option.H = 0;
    option.L = 0;
    option.P = 0;
    return option;
}

struct option *option_uptade(char *str, struct option *option)
{
    if (!strcmp(str, "-d"))
        option->d = 1;
    if (!strcmp(str, "-H"))
        option->H = 1;
    if (!strcmp(str, "-L"))
        option->L = 1;
    if (!strcmp(str, "-P"))
        option->P = 1;
    return option;
}

void option_print(struct option *option)
{
    printf("option:\n");
    printf("    %d\n", option->d);
    printf("    %d\n", option->H);
    printf("    %d\n", option->L);
    printf("    %d\n", option->P);
}
