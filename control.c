#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

int main(int argc, char *argv[]){
    if (argc != 2) 
        return 0;
    int semid, semc, shmid, shmc, fd;
    int key = ftok("makefile" , 22);
    if (!strcmp(argv[1], "-c")){
        // Create semaphore
        semid = semget(key, 1, IPC_CREAT | 0644);
        printf("semaphore created: %d\n", semid);

        // Set up semaphore
        union semun su;
        su.val = 1;
        semc = semctl(semid, 0, SETVAL, su);
       
        // Create shared memory
        shmid = shmget(key, sizeof(int), IPC_CREAT | 0644);
        printf("shared memory at: %d\n", shmid);

        // Create file
        fd = open("semaphone.txt", O_CREAT | O_TRUNC, 0644);
        write(fd, 0, 0);
        close(fd);
    }
    else if (!strcmp(argv[1], "-v")){
        // Display file
        fd = open("semaphone.txt", O_RDONLY);
        struct stat fileinfo;
        stat("semaphone.txt", &fileinfo);
        int size = fileinfo.st_size;
        char content[size + 1];
        content[size] = 0;
        read(fd, content, size);
        printf("%s", content);
        close(fd);
    }
    else if(!strcmp(argv[1], "-r")){
        // Remove semaphore
        semid = semget(key, 1, 0);
        semc = semctl(semid, 0, IPC_RMID);

        // Remove shared memory
        shmid = shmget(key, sizeof(int), 0);
        shmc = shmctl(shmid, IPC_RMID, 0);

        // Display file
        fd = open("semaphone.txt", O_RDONLY);
        struct stat fileinfo;
        stat("semaphone.txt", &fileinfo);
        int size = fileinfo.st_size;
        char content[size + 1];
        content[size] = 0;
        read(fd, content, size);
        printf("%s", content);
        close(fd);
    }
    else {
        printf("Not a valid argument\n");
    }
    return 0;
}
