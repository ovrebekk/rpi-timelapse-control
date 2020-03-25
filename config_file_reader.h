#ifndef __CONFIG_FILE_READER_H
#define __CONFIG_FILE_READER_H

#define KEYWORD_INTERVAL        "interval"
#define KEYWORD_FILENAME        "filename"
#define KEYWORD_CREATE_DIRS_DAY "subdir_pr_day"
typedef struct 
{
  int interval_s;
  int subdir_pr_day_enable;
  char name[80];
}timelapse_config_t;

int get_config_from_file(char *filename, timelapse_config_t *config);

#endif
