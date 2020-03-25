#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
#include <string.h>
#include <time.h>

#define STRING_MAXLENGTH 80

#define CMD_STRING_PRE "gphoto2 --capture-image-and-download --filename "
#define CMD_STRING_POST ".%C"
 
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
  // Setting default settings in case of missing config parameters
  timelapse_config.interval_s = 30;
  strcpy(timelapse_config.name, "%Y%m%d-%H%M%S");
  
  // Trying to open the config file
  if((fptr = fopen("config.txt", "r")) == NULL)
  {
    printf("Error: Config file not found\r\n");
    return -1;
  }
  // Going through the config file line for line
  while(fgets(work_string, 128, fptr) != NULL)
  {
    char *div_ptr = strchr(work_string, ':');
    if(div_ptr != NULL && div_ptr > work_string)
    {
      memcpy(config_type, work_string, div_ptr - work_string);
      config_type[div_ptr - work_string] = 0;
      div_ptr++;
      while(*div_ptr == ' ') div_ptr++;
      div_ptr[strchr(div_ptr, '\n')-div_ptr]=0;
      if(strcmp(config_type, "interval") == 0)
      {
        int interval = atoi(div_ptr);
        printf("Setting interval: %is\n", interval);
        timelapse_config.interval_s = interval;
      }
      else if(strcmp(config_type, "filename") == 0)
      {
        printf("Setting filename: %s\n", div_ptr);
        strcpy(timelapse_config.name, div_ptr);
      }
    }
  }
  // Close the config file
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
  strftime(time_str_buf, 80, "%Y%m%d-%H%M%S", current_time);
  printf("Taking picture at %s - %s\n", asctime(current_time), time_str_buf);
  
  static char cmd_string[128];
  strcpy(cmd_string, CMD_STRING_PRE);
  strcat(cmd_string, timelapse_config.name);
  strcat(cmd_string, CMD_STRING_POST);
  system(cmd_string);
  
}
  
void *cameraShootingThread(void *vargp) 
{ 
  while(1)
  {
    trigger_camera();
    sleep(timelapse_config.interval_s); 
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
