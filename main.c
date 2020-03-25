#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
#include <string.h>
#include <time.h>

#define STRING_MAXLENGTH 80

#define PRINTF_CL_YELLOW 	"\033[01;33m"
#define PRINTF_CL_DEFAULT 	"\033[0m"
#define PRINTF_CL_RED		"\033[1;31m"

#define CMD_STRING_PRE "gphoto2 --capture-image-and-download --filename "
#define CMD_STRING_POST ".%C"
 
pthread_t camera_shooting_thread_id, camera_interval_thread_id;

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

static int file_exists(char *file_name)
{
	return access(file_name, F_OK) != -1;
}

static time_t trigger_camera(void)
{
  static struct tm * current_time;
  static time_t lt;
  static char time_str_buf[80];
  lt = time(NULL);  
  current_time = localtime(&lt);
  strftime(time_str_buf, 80, timelapse_config.name, current_time);
  printf(PRINTF_CL_DEFAULT);
  printf("Taking picture at %s - %s\n", asctime(current_time), time_str_buf);
  
  static char cmd_string[128];
  strcpy(cmd_string, CMD_STRING_PRE);
  strcat(cmd_string, timelapse_config.name);
  strcat(cmd_string, CMD_STRING_POST);
  system(cmd_string);
  
  return lt;
}

void *cameraShootingThread(void *vargp) 
{ 
  static struct tm * current_time;
  time_t trigger_time = trigger_camera();
  
  // Check if file exists
  static char pic_name[64];
  static int pic_found;
  for(int i = 0; i < 10; i++)
  {
	  current_time = localtime(&trigger_time);
	  strftime(pic_name, 80, timelapse_config.name, current_time);
	  strcat(pic_name, ".jpg");
	  if((pic_found = file_exists(pic_name))) break;
	  trigger_time++;
  }
  if(pic_found)
  {
	  printf(PRINTF_CL_YELLOW); 
	  printf("JPG found: %s\n", pic_name);
	  printf(PRINTF_CL_DEFAULT);
  }
	  
  return NULL; 
} 

void *cameraIntervalThread(void *vargp)
{
	while(1)
	{
		pthread_create(&camera_shooting_thread_id, NULL, cameraShootingThread, NULL); 
		sleep(timelapse_config.interval_s); 
	}
}
   
int main() 
{ 
  printf("Camera control started\n"); 
    
  load_config_from_file();

  pthread_create(&camera_interval_thread_id, NULL, cameraIntervalThread, NULL); 
  pthread_join(camera_interval_thread_id, NULL); 
  exit(0); 
}
