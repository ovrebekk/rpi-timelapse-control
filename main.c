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
 
static TimelapseConfig m_local_config;

static int picture_success_counter = 0;
static int picture_failure_counter = 0;

static char pic_name[64];
static int pic_found;

pthread_t camera_shooting_thread_id, camera_interval_thread_id;

static int load_config_from_file(void)
{
  // Try to load parameters from file (otherwise default parameters will be used)
  m_local_config.getConfigFromFile("config.txt");
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
  strcat(cmd_string, m_local_config.getName());
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
	  strftime(pic_name, 80, m_local_config.getName(), current_time);
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
  static time_t last_shot_time;
  static time_t current_time;
  current_time = time(NULL);
  if(value == 0 || (current_time - last_shot_time) >= m_local_config.getIntervalS())
  {
    pic_found = 0;
    last_shot_time = current_time;
    pthread_create(&camera_shooting_thread_id, NULL, cameraShootingThread, NULL); 
  } 
  if(pic_found)
  {
    pic_found = 0;
    gui_free_current_image();
    gui_set_display_image(pic_name);
  }
  //printf("Secs: %i\n", value);
  glutTimerFunc(100, cameraIntervalGlutTimer, value + 1);
}

static void InitializeGlutCallbacks()
{
    glutDisplayFunc(gui_render);
    glutReshapeFunc(gui_resize);
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
  
  glutTimerFunc(100, cameraIntervalGlutTimer, 0);
  
  glutMainLoop();
  exit(0); 
}
