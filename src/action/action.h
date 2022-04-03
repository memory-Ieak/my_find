#ifndef ACTION_H
#define ACTION_H

#include <dirent.h>
#include <fnmatch.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <grp.h>

#include "utils.h"
#include "file.h"
#include "statut.h"

int name(char *args[], struct file *file, struct statut *statut);
int type(char *args[], struct file *file, struct statut *statut);
int newer(char *args[], struct file *file, struct statut *statut);

int exec(char *args[], struct file *file, struct statut *statut);
int exec_dir(char *args[], struct file *file, struct statut *statut);

int delete(char *args[], struct file *file, struct statut *statut);

int group(char *args[], struct file *file, struct statut *statut);

#endif /* ! ACTION_H */