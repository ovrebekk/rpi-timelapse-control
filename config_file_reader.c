#include "config_file_reader.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>

int get_config_from_file(const char *filename, timelapse_config_t *config)
{
  FILE *fptr;
  char work_string[128], config_type[32];

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
      if(strcmp(config_type, KEYWORD_INTERVAL) == 0)
      {
        int interval = atoi(div_ptr);
        printf("Setting interval: %is\n", interval);
        config->interval_s = interval;
      }
      else if(strcmp(config_type, KEYWORD_FILENAME) == 0)
      {
        printf("Setting filename: %s\n", div_ptr);
        strcpy(config->name, div_ptr);
      }
      else if(strcmp(config_type, KEYWORD_CREATE_DIRS_DAY) == 0)
      {
        config->subdir_pr_day_enable = atoi(div_ptr);
        printf("Adding pics to daily sub directories: %i\n", config->subdir_pr_day_enable);
      }
    }
  }
  
  // Close the config file
  fclose(fptr);
  return 0;
}
