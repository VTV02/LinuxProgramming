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

volatile sig_atomic_t running = 1;

void signal_handler(int signum) {
    running = 0;
}

int main(int argc, char* argv[]) {
    int ret, num_read;
    int fds;
    char read_buff[MAX_SIZE];
   
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
        printf("\nWaiting for message...\n");
        
        // Mở file để đọc
        fds = open(MY_FILE, O_RDONLY);
        if (fds == -1) {
            perror("open() failed");
            continue;
        }

        // Đọc từ FIFO
        memset(read_buff, 0, sizeof(read_buff));  // Xóa buffer
        num_read = read(fds, read_buff, sizeof(read_buff) - 1);
        
        if (num_read == -1) {
            perror("read() failed");
            close(fds);
            continue;
        } else if (num_read == 0) {
            printf("EOF reached\n");
            close(fds);
            continue;
        }
        
        read_buff[num_read] = '\0';  // Đảm bảo chuỗi kết thúc
        printf("Message received: %s\n", read_buff);

        if (close(fds) == -1) {
            perror("close() failed");
        }
    }

    // Cleanup
    unlink(MY_FILE);
    printf("\nCleaning up and exiting...\n");
    return 0;
}