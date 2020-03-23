#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
#include <string.h>
#include <time.h>

#define CMD_STRING "gphoto2 --capture-image-and-download --filename \"%Y%m%d-%H%M%S.%C\"" 
pthread_t camera_trigger_thread_id;

static void trigger_camera(void)
{
  static struct tm * current_time;
  static time_t lt;
  static char time_str_buf[80];
  lt = time(NULL);  
  current_time = localtime(&lt);
  strftime(time_str_buf, 80, "%Y%m%d-%H%M%S.%C", current_time);
  printf("Taking picture at %s - %s\n", asctime(current_time), time_str_buf);
  system(CMD_STRING);
  
}
  
void *cameraShootingThread(void *vargp) 
{ 
  while(1)
  {
    trigger_camera();
    sleep(30); 
  }
  return NULL; 
} 
   
int main() 
{ 
    printf("Camera control started\n"); 
    pthread_create(&camera_trigger_thread_id, NULL, cameraShootingThread, NULL); 
    pthread_join(camera_trigger_thread_id, NULL); 
    exit(0); 
}
