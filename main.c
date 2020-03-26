#include <GL/freeglut.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
#include <string.h>
#include <time.h>
#include "config_file_reader.h"
#include "gui.h"

#define STRING_MAXLENGTH 80

#define PRINTF_CL_YELLOW    "\033[01;33m"
#define PRINTF_CL_DEFAULT   "\033[0m"
#define PRINTF_CL_RED       "\033[1;31m"
#define PRINTF_CL_GREEN     "\033[1;32m"

#define CMD_STRING_PRE "gphoto2 --capture-image-and-download --filename "
#define CMD_STRING_POST ".%C"
 
static timelapse_config_t	m_local_config;

static int picture_success_counter = 0;
static int picture_failure_counter = 0;

static char pic_name[64];
static int pic_found;

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
  
  printf(PRINTF_CL_GREEN);
  printf("\nTaking picture at %sPics taken %i, pics failed %i\n", asctime(current_time),
          picture_success_counter, picture_failure_counter);
  printf(PRINTF_CL_DEFAULT);
  printf("\n");
  
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
    picture_success_counter++;
	  printf(PRINTF_CL_YELLOW); 
	  printf("JPG found: %s\n", pic_name);
	  printf(PRINTF_CL_DEFAULT);
  }
  else
  {
    picture_failure_counter++;
    printf(PRINTF_CL_RED);
    printf("No image stored!\n");
  }
	  
  return NULL; 
} 

static void cameraIntervalGlutTimer(int value)
{
  glutTimerFunc(1000, cameraIntervalGlutTimer, value + 1);
  if((value % m_local_config.interval_s) == 0)
  {
    pic_found = 0;
    pthread_create(&camera_shooting_thread_id, NULL, cameraShootingThread, NULL); 
  } 
  if(pic_found)
  {
    pic_found = 0;
    gui_set_display_image(pic_name);
  }
  //printf("Secs: %i\n", value);
}

static void InitializeGlutCallbacks()
{
    glutDisplayFunc(gui_render);
}
   
int main(int argc, char** argv) 
{ 
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
  glutInitWindowSize(600, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("CamControl");

  InitializeGlutCallbacks();
  
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  
  load_config_from_file();
  printf("Camera control started\n"); 
  
  glutTimerFunc(1000, cameraIntervalGlutTimer, 0);
  printf("PIKK\n");
  
  glutMainLoop();
  exit(0); 
}
