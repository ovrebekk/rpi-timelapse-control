#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details. 
#include <pthread.h> 
#include <string.h>

pthread_t camera_trigger_thread_id;

static void trigger_camera(void)
{
  
}
  
void *myThreadFun(void *vargp) 
{ 
  sleep(1); 
  system("ls");
  return NULL; 
} 
   
int main() 
{ 
    pthread_t thread_id; 
    printf("Before Thread\n"); 
    pthread_create(&thread_id, NULL, myThreadFun, NULL); 
    pthread_join(thread_id, NULL); 
    printf("After Thread\n"); 
    exit(0); 
}
