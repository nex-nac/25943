#define _XOPEN_SOURCE 600

#include <sys/types.h>
#include <sys/stat.h>
#include <grp.h>
#include <inttypes.h>
#include <libgen.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void mode_string(mode_t mode, char result[11])
{
    // первый символ показывает тип файла
    if (S_ISDIR(mode))
        result[0] = 'd';
    else if (S_ISREG(mode))
        result[0] = '-';
    else
        result[0] = '?';

    // права владельца
    result[1] = (mode & S_IRUSR) ? 'r' : '-';
    result[2] = (mode & S_IWUSR) ? 'w' : '-';
    result[3] = (mode & S_IXUSR) ? 'x' : '-';

    // права группы
    result[4] = (mode & S_IRGRP) ? 'r' : '-';
    result[5] = (mode & S_IWGRP) ? 'w' : '-';
    result[6] = (mode & S_IXGRP) ? 'x' : '-';

    // права остальных
    result[7] = (mode & S_IROTH) ? 'r' : '-';
    result[8] = (mode & S_IWOTH) ? 'w' : '-';
    result[9] = (mode & S_IXOTH) ? 'x' : '-';

    result[10] = '\0';
}

static int print_file(char *path)
{
    struct stat info;
    struct passwd *owner;
    struct group *group;
    struct tm *modified;
    char mode[11];
    char size[32] = "";
    char date[32];
    const char *date_text = "?";

    // lstat получает сведения о самой символической ссылке
    if (lstat(path, &info) == -1) {
        perror(path);
        return -1;
    }

    mode_string(info.st_mode, mode);

    // поле размера заполняем только для обычного файла
    if (S_ISREG(info.st_mode))
        snprintf(size, sizeof size, "%jd", (intmax_t)info.st_size);

    modified = localtime(&info.st_mtime);
    if (modified != NULL &&
        strftime(date, sizeof date, "%Y-%m-%d %H:%M", modified) != 0) {
        date_text = date;
    }

    // число связей выводим только для каталогов
    printf("%-10s ", mode);
    if (S_ISDIR(info.st_mode))
        printf("%4ju ", (uintmax_t)info.st_nlink);
    else
        printf("%4s ", "");

    // если имя владельца или группы не найдено, выводим идентификатор
    owner = getpwuid(info.st_uid);
    if (owner != NULL)
        printf("%-12s ", owner->pw_name);
    else
        printf("%-12ju ", (uintmax_t)info.st_uid);

    group = getgrgid(info.st_gid);
    if (group != NULL)
        printf("%-12s ", group->gr_name);
    else
        printf("%-12ju ", (uintmax_t)info.st_gid);

    // выводим имя без родительских каталогов
    printf("%10s %-16s %s\n", size, date_text, basename(path));
    return 0;
}

int main(int argc, char *argv[])
{
    char current_dir[] = ".";
    int status = EXIT_SUCCESS;
    int i;

    printf("%-10s %4s %-12s %-12s %10s %-16s %s\n",
        "MODE", "LINK", "OWNER", "GROUP", "SIZE", "MODIFIED", "NAME");

    if (argc == 1)
        return print_file(current_dir) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;

    // если один путь не удалось обработать, продолжаем со следующим
    for (i = 1; i < argc; ++i) {
        if (print_file(argv[i]) == -1)
            status = EXIT_FAILURE;
    }

    return status;
}