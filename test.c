#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>

//DIS METHOD JUST ADDS ONE TO THE SEMAPHORE


int main(){

  int semid;
  int key = ftok("makefile" , 22);
  int sc;

  struct sembuf sb;

  sb.sem_op = 1;
  sb.sem_flg = 0;
  sb.sem_num = 0;
  
  semid = semget(key, 1, 0); // IF WE PUT -1 in instead of 1 its down 1

  semop(semid, &sb, 1);
  
  return 0;
}
