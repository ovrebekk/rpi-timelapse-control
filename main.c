#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
#include <string.h>
#include <time.h>
#include "config_file_reader.h"

#define STRING_MAXLENGTH 80

#define PRINTF_CL_YELLOW 	"\033[01;33m"
#define PRINTF_CL_DEFAULT 	"\033[0m"
#define PRINTF_CL_RED		"\033[1;31m"

#define CMD_STRING_PRE "gphoto2 --capture-image-and-download --filename "
#define CMD_STRING_POST ".%C"
 
static timelapse_config_t	m_local_config;

pthread_t camera_shooting_thread_id, camera_interval_thread_id;

static int load_config_from_file(void)
{
  // Setting default settings in case of missing config parameters
  if(get_config_from_file("config.txt", &m_local_config) != 0)
  {
    m_local_config.interval_s = 30;
    m_local_config.subdir_pr_day_enable = 0;
    strcpy(m_local_config.name, "%Y%m%d-%H%M%S");
  }
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
  strftime(time_str_buf, 80, m_local_config.name, current_time);
  printf(PRINTF_CL_DEFAULT);
  printf("Taking picture at %s - %s\n", asctime(current_time), time_str_buf);
  
  static char cmd_string[128];
  strcpy(cmd_string, CMD_STRING_PRE);
  strcat(cmd_string, m_local_config.name);
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
	  strftime(pic_name, 80, m_local_config.name, current_time);
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
		sleep(m_local_config.interval_s); 
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
