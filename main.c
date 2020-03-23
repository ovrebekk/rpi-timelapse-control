#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
#include <string.h>
#include <time.h>

#define STRING_MAXLENGTH 80

#define CMD_STRING "gphoto2 --capture-image-and-download --filename \"%Y%m%d-%H%M%S.%C\"" 
pthread_t camera_trigger_thread_id;

struct 
{
  int interval_s;
  char name[STRING_MAXLENGTH];
}timelapse_config;

static int load_config_from_file(void)
{
  FILE *fptr;
  char work_string[128], config_type[32];
  if((fptr = fopen("config.txt", "r")) == NULL)
  {
    printf("Error: Config file not found\r\n");
    return -1;
  }
  while(fgets(work_string, 128, fptr) != NULL)
  {
    char *div_ptr = strchr(work_string, ':');
    if(div_ptr != NULL && div_ptr > work_string)
    {
      memcpy(config_type, work_string, div_ptr - work_string);
      config_type[div_ptr - work_string] = 0;
      if(strcmp(config_type, "interval") == 0)
      {
        printf("Setting Interval\n");
      }
      else if(strcmp(config_type, "filename") == 0)
      {
        printf("Setting Filename\n");
      }
    }
  }
  fclose(fptr);
  return 0;
}

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
    
  load_config_from_file();

  pthread_create(&camera_trigger_thread_id, NULL, cameraShootingThread, NULL); 
  pthread_join(camera_trigger_thread_id, NULL); 
  exit(0); 
}
