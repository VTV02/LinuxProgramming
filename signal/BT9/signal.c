#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <errno.h>

void signal_handler1(int num) {
    printf("\nSIGINT received\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]) {
    if (signal(SIGINT, signal_handler1) == SIG_ERR) {
        printf("\nCannot handle SIGINT\n");
        exit(EXIT_FAILURE);
    }

    sigset_t block, sigset;
    
    // Kiểm tra trạng thái block hiện tại
    if (sigprocmask(SIG_BLOCK, NULL, &sigset) == 0) {
        if (sigismember(&sigset, SIGINT))
            printf("\nSIGINT is currently blocked\n");
        else
            printf("\nSIGINT is not blocked\n");
    } else {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    // Block SIGINT
    if (sigemptyset(&block) == -1) {
        perror("sigemptyset");
        exit(EXIT_FAILURE);
    }
    if (sigaddset(&block, SIGINT) == -1) {
        perror("sigaddset");
        exit(EXIT_FAILURE);
    }
    if (sigprocmask(SIG_BLOCK, &block, NULL) == -1) {
        perror("sigprocmask block");
        exit(EXIT_FAILURE);
    }

    // Kiểm tra xem đã block thành công chưa
    if (sigprocmask(SIG_BLOCK, NULL, &sigset) == 0) {
        if (sigismember(&sigset, SIGINT))
            printf("\nSIGINT has been successfully blocked\n");
            sleep(5);
    }

    printf("\nUnblocking SIGINT...\n");
    if (sigprocmask(SIG_UNBLOCK, &block, NULL) == -1) {
        perror("sigprocmask unblock");
        exit(EXIT_FAILURE);
    }

    // Kiểm tra xem đã unblock thành công chưa
    if (sigprocmask(SIG_BLOCK, NULL, &sigset) == 0) {
        if (!sigismember(&sigset, SIGINT))
            printf("\nSIGINT has been successfully unblocked\n");
    }

    printf("\nWaiting for SIGINT (Ctrl+C)...\n");
    pause();
    return 0;
}