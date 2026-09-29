#include <stdio.h>
#include <time.h>

int main(void)
{
    time_t current_time;
    struct tm *california_time;

    current_time = time(NULL);

    current_time -= 8 * 60 * 60;

    california_time = gmtime(&current_time);

    printf("california time: %02d.%02d.%04d %02d:%02d:%02d\n",
           california_time->tm_mday,
           california_time->tm_mon + 1,
           california_time->tm_year + 1900,
           california_time->tm_hour,
           california_time->tm_min,
           california_time->tm_sec);

    return 0;
}
