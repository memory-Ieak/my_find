#define _GNU_SOURCE

#include "action.h"

int name(char *args[], struct file *file, struct statut *statut)
{
    int match = fnmatch(*args, file->name, 0);
    if (match == 0)
        return 1;
    else if (match == FNM_NOMATCH)
        return 0;
    else
        statut->exit_statut = 1;
    return 0;
}

int _type(int type, unsigned char d_type)
{
    if (type == d_type)
        return 1;
    return 0;
}

int type(char *args[], struct file *file, struct statut *statut)
{
    if (**args == 'b')
        return _type(file->type, DT_BLK);
    if (**args == 'c')
        return _type(file->type, DT_CHR);
    if (**args == 'd')
        return _type(file->type, DT_DIR);
    if (**args == 'f')
        return _type(file->type, DT_REG);
    if (**args == 'l')
        return _type(file->type, DT_LNK);
    if (**args == 'p')
        return _type(file->type, DT_FIFO);
    if (**args == 's')
        return _type(file->type, DT_SOCK);
    statut->exit_statut = 1;
    fprintf(stderr, "my_find: Wrong argument %s\n", *args);
    return 0;
}

int newer(char *args[], struct file *file, struct statut *statut)
{
    struct stat user_stat;
    struct stat input_stat;
    if (stat(file->name, &user_stat) == -1 || stat(*args, &input_stat) == -1)
    {
        fprintf(stderr, "my_find: Can't have stat from %s or %s\n", file->name, file->starting_point);
        statut->exit_statut = 1;
        return 0;
    }
    if (user_stat.st_mtime > input_stat.st_mtime)
        return 1;
    return 0;
}

int exec(char *args[], struct file *file, struct statut *statut)
{
    int _exit_statut = 0;
    char **sub = substitution(args, file->path);
    int pid = fork();
    if (pid == 0)
    {
        execvp(sub[0], sub);
    }
    else
    {
        int wstatus;
        waitpid(pid, &wstatus, 0);
        _exit_statut = WEXITSTATUS(wstatus);
    }
    free_cpy_str(sub);
    statut->print = 0;
    return (_exit_statut == 0) ? 1 : 0;
}

int exec_dir(char *args[], struct file *file, struct statut *statut)
{
    int _exit_statut = 0;
    char **sub = substitution_dir(args, file->name);
    int pid = fork();
    if (pid == 0)
    {
        execvp(sub[0], sub);
    }
    else
    {
        int wstatus;
        waitpid(pid, &wstatus, 0);
        _exit_statut = WEXITSTATUS(wstatus);
    }
    free_str(sub);
    statut->print = 0;
    return (_exit_statut == 0) ? 1 : 0;
}

int delete(char *args[], struct file *file, struct statut *statut)
{
    if (*args && strcmp(*args, "-d"))
    {
        statut->exit_statut = 1;
        fprintf(stderr, "myfind: unknown predicate '%s'\n", *args);
        return 0;
    }
    if (remove(file->path) == 0)
        return 1;
    else
        fprintf(stderr, "myfind: cannot delete '%s'': No such file or directory\n", file->path);
    statut->exit_statut = 1;
    statut->print = 0;
    return 0;
}

int group(char *args[], struct file *file, struct statut *statut)
{
    struct stat file_stat;
    if (stat(file->path, &file_stat) == -1)
    {
        fprintf(stderr, "my_find: Can't have stat from %s\n", file->path);
        statut->exit_statut = 1;
        return 0;
    }
    struct group *grp;
    if ((grp = getgrgid(file_stat.st_gid)))
    {
        if (!strcmp(*args, grp->gr_name))
            return 1;
    }
    else
        statut->exit_statut = 1;
    return 0;
}