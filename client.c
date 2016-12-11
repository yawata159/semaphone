#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

int main() {
    int key = ftok("makefile", 22);
    int semid = semget(key, 1, 0);
    int shmid = shmget(key, sizeof(int), 0);
    int *shmval = shmat(shmid, 0, 0);

    printf("Checking for other people in the line...\n");

    // Down
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_flg = SEM_UNDO;
    sb.sem_op = -1;
    semop(semid, &sb, 1);
    
    int fd = open("semaphone.txt", O_RDWR | O_APPEND);
    int prev_len = *shmval;
    lseek(fd, -1 * prev_len, SEEK_END);

    char prev_message[prev_len + 1];
    prev_message[prev_len] = 0;
    read(fd, prev_message, prev_len);
    
    printf("Previous message: %s\n", prev_message);
    
    printf("What's your message?\n> ");
    char new_message[1024];
    new_message[1023] = 0;
    fgets(new_message, 1023, stdin);
    write(fd, new_message, strlen(new_message));
    close(fd);
    printf("Your message has been submitted\n");

    *shmval = strlen(new_message);
    shmdt(shmval);

    sb.sem_op = 1;
    semop(semid, &sb, 1);

    return 0;
}

