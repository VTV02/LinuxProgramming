#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define MY_FILE "./myfifo"
#define MAX_SIZE 200

// Biến toàn cục để xử lý tín hiệu
volatile sig_atomic_t running = 1;

void signal_handler(int signum) {
    running = 0;
}

int main(int argc, char* argv[]) {
    int ret, num_write;
    int fds;
    char write_buff[MAX_SIZE];

    // Thiết lập xử lý tín hiệu
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    // Tạo FIFO
    ret = mkfifo(MY_FILE, 0666);
    if (ret == -1 && errno != EEXIST) {
        perror("mkfifo() failed");
        return 1;
    }

    while (running) {
        printf("\nMessage to reader: ");
        //Khi bên writer mở FIFO với flag O_WRONLY (chỉ ghi), 
        //nó sẽ bị block (chờ) cho đến khi có một process khác mở FIFO để đọc (O_RDONLY).
        fds = open(MY_FILE, O_WRONLY | O_APPEND);
        if (fds == -1) {
            perror("open() failed");
            continue;
        }
        
        printf("\nWhat's the message you want to send? ");
        if (fgets(write_buff, sizeof(write_buff), stdin) == NULL) {
            perror("fgets() failed");
            close(fds);
            continue;
        }
        
        num_write = write(fds, write_buff, strlen(write_buff));
        if (num_write == -1) {
            perror("write() failed");
        }

        if (close(fds) == -1) {
            perror("close() failed");
        }
    }

    // Cleanup
    //unlink(MY_FILE);
    printf("\nCleaning up and exiting...\n");
    return 0;
}