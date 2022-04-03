#define _GNU_SOURCE

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "ast.h"
#include "eval.h"
#include "lexer.h"
#include "parser.h"
#include "option.h"
#include "file.h"
#include "statut.h"
#include "utils.h"

char **get_option(int *argc, char **argv, struct option *opt)
{
    char *option[] = { "-d", "-H", "-L", "-P" };
    size_t option_size = sizeof(option) / sizeof(*option);
    while (_strcmp(*argv, option, option_size))
    {
        opt = option_uptade(*argv, opt);
        argv++;
        *argc -= 1;
    }
    return argv;
}

void get_starting_points(int *argc, char **argv)
{
    char *expression[] = { "-newer", "-name", "-type",
                           "-print", "-exec", "-execdir",
                           "-user", "-group", "-perm", "-delete",
                           "-o", "-a", "(", ")", "!", ";", "+" };
    size_t expression_size = sizeof(expression) / sizeof(*expression);
    size_t i = 0;
    while (*argc > 0 && !_strcmp(*(argv + i), expression, expression_size))
    {
        *argc -= 1;
        i++;
    }
}

void _find(struct ast *ast, char *dir, char *arg, struct statut *statut)
{
    struct dirent *pDirent;
    DIR *pDir = opendir(dir);
    if (!pDir)
    {
        fprintf(stderr, "my_find: '%s': No such file or directory\n", dir);
        statut->exit_statut = 1;
        return;
    }
    while ((pDirent = readdir(pDir)))
    {
        char path[PATH_MAX];
        snprintf(path, PATH_MAX, "%s/%s", dir, pDirent->d_name);
        struct file file = { pDirent->d_name, pDirent->d_type, path, arg };

        if (!strcmp(pDirent->d_name, ".") || !strcmp(pDirent->d_name, ".."))
            continue;
        if (!ast || eval(ast, &file, statut))
            if (statut->print)
                printf("%s/%s\n", dir, pDirent->d_name);
        if (pDirent->d_type == DT_DIR)
            _find(ast, path, arg, statut);
	}
    closedir(pDir);
}

void find(struct ast *ast, char *dir, char *arg, struct statut *statut)
{
    struct file file = { dir, DT_DIR, dir, arg };
    struct stat filestat;
    if (stat(arg, &filestat) == -1)
    {
        fprintf(stderr, "my_find: Can't have stat from %s\n", arg);
        statut->exit_statut = 1;
        return ;
    }
    if (!ast || eval(ast, &file, statut))
    {
        if (statut->print)
            printf("%s\n", dir);
    }
    if (S_ISDIR(filestat.st_mode))
    {
        _find(ast, dir, arg, statut);
    }
}

int main(int argc, char *argv[])
{
    struct statut statut = { 0, 1 };
    if (argc == 1)
    {
        find(NULL, ".", ".", &statut);
        return statut.exit_statut;
    }
    else
        --argc;

    struct option option = option_init();

    argv = get_option(&argc, ++argv, &option);
    int begin = argc;
    get_starting_points(&argc, argv);
    int end = begin - argc;
    if (!argc)
    {
        for (int i = 0; i < end; i++)
            find(NULL, argv[i], argv[i], &statut);
        return statut.exit_statut;
    }

    char **exp = correct_exp(argv + end, argc);
    struct lexer *lexer = lexer_new(exp);
    struct ast *ast = NULL;

    enum parser_status status = parse(&ast, lexer);
    lexer_free(lexer);

    if (status == PARSER_UNEXPECTED_TOKEN)
    {
        free_str(exp);
        return 1;
    }
    
    //ast_print(ast);
    if (end == 0)
        find(ast, ".", ".", &statut);
    else
    {
        for (int i = 0; i < end; i++)
            find(ast, argv[i], argv[i], &statut);
    }

    ast_free(ast);
    free_str(exp);

    return statut.exit_statut;
}