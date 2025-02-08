#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#define STUDENT_FILE "./studentManagement.txt"
#define MAX_STUDENT 30

typedef struct {
    char name[30];
    char birthday[20];
    char homeTown[50];
} student_t;

typedef struct {
    student_t student[MAX_STUDENT];
    int count;
} list_student_t;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int student_number;

void remove_newline(char* str) {
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\t';
    }
}

void* thread_handle1(void* arg) {
    list_student_t* student_list = (list_student_t*)arg;

    pthread_mutex_lock(&mutex);
    for (int i = 0; i < student_number; i++) {
        printf("\nEnter Student's Name: ");
        fgets(student_list->student[student_list->count].name, sizeof(student_list->student[student_list->count].name), stdin);
        remove_newline(student_list->student[student_list->count].name);

        printf("Enter Student's Birthday: ");
        fgets(student_list->student[student_list->count].birthday, sizeof(student_list->student[student_list->count].birthday), stdin);
        remove_newline(student_list->student[student_list->count].birthday);

        printf("Enter Student's HomeTown: ");
        fgets(student_list->student[student_list->count].homeTown, sizeof(student_list->student[student_list->count].homeTown), stdin);
        remove_newline(student_list->student[student_list->count].homeTown);

        student_list->count++;
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* thread_handle2(void* arg) {
    list_student_t* student_list = (list_student_t*)arg;
    int fd;

    pthread_mutex_lock(&mutex);
    fd = open(STUDENT_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Open file failure");
        pthread_mutex_unlock(&mutex);
        pthread_exit(NULL);
    }

    for (int i = 0; i < student_list->count; i++) {
        write(fd, student_list->student[i].name, sizeof(student_list->student[i].name));
        write(fd, "\n", 1);
        write(fd, student_list->student[i].birthday, sizeof(student_list->student[i].birthday));
        write(fd, "\n", 1);
        write(fd, student_list->student[i].homeTown, sizeof(student_list->student[i].homeTown));
        write(fd, "\n", 1);
    }
    close(fd);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* thread_handle3(void* arg) {
    list_student_t* student_list = (list_student_t*)arg;
    int fd;
    char buffer[100];

    pthread_mutex_lock(&mutex);
    fd = open(STUDENT_FILE, O_RDONLY);
    if (fd == -1) {
        perror("Open file failure");
        pthread_mutex_unlock(&mutex);
        pthread_exit(NULL);
    }

    student_list->count = 0;
    while (read(fd, buffer, sizeof(student_list->student[student_list->count].name)) > 0) {
        remove_newline(buffer);
        strcpy(student_list->student[student_list->count].name, buffer);

        read(fd, buffer, 1); // Đọc ký tự '\n'

        read(fd, buffer, sizeof(student_list->student[student_list->count].birthday));
        remove_newline(buffer);
        strcpy(student_list->student[student_list->count].birthday, buffer);

        read(fd, buffer, 1); // Đọc ký tự '\n'

        read(fd, buffer, sizeof(student_list->student[student_list->count].homeTown));
        remove_newline(buffer);
        strcpy(student_list->student[student_list->count].homeTown, buffer);

        read(fd, buffer, 1); // Đọc ký tự '\n'

        printf("\nStudent Info: Name: %s | Date of Birth: %s | HomeTown: %s\n",
               student_list->student[student_list->count].name,
               student_list->student[student_list->count].birthday,
               student_list->student[student_list->count].homeTown);

        student_list->count++;
    }
    close(fd);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    list_student_t student_list = {0};
    pthread_t thread1, thread2, thread3;

    printf("How many students do you want to access: ");
    scanf("%d", &student_number);
    getchar(); // Loại bỏ ký tự '\n' còn lại trong bộ đệm

    if (pthread_create(&thread1, NULL, thread_handle1, &student_list) != 0) {
        perror("Create Thread1 failure");
        exit(EXIT_FAILURE);
    }
    pthread_join(thread1, NULL);

    if (pthread_create(&thread2, NULL, thread_handle2, &student_list) != 0) {
        perror("Create Thread2 failure");
        exit(EXIT_FAILURE);
    }
    pthread_join(thread2, NULL);

    if (pthread_create(&thread3, NULL, thread_handle3, &student_list) != 0) {
        perror("Create Thread3 failure");
        exit(EXIT_FAILURE);
    }
    pthread_join(thread3, NULL);

    return 0;
}
