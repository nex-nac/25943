#define _POSIX_C_SOURCE 200112L

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    sigset_t signals;
    int received_signal;
    int error;
    unsigned long count = 0;

    // составляем набор сигналов, которые будем ждать
    sigemptyset(&signals);
    sigaddset(&signals, SIGINT);
    sigaddset(&signals, SIGQUIT);

    // блокируем стандартную обработку этих сигналов
    if (sigprocmask(SIG_BLOCK, &signals, NULL) == -1) {
        perror("sigprocmask");
        return EXIT_FAILURE;
    }

    for (;;) {
        // ждём сигнала без постоянной загрузки процессора
        error = sigwait(&signals, &received_signal);
        if (error != 0) {
            fprintf(stderr, "sigwait: %s\n", strerror(error));
            return EXIT_FAILURE;
        }

        if (received_signal == SIGQUIT) {
            printf("\nКоличество звуковых сигналов: %lu\n", count);
            return EXIT_SUCCESS;
        }

        // отправляем звонок сразу, даже без перевода строки
        putchar('\a');
        fflush(stdout);
        ++count;
    }
}