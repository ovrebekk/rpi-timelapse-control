#ifndef __CONFIG_FILE_READER_H
#define __CONFIG_FILE_READER_H

#define KEYWORD_INTERVAL        "interval"
#define KEYWORD_FILENAME        "filename"
#define KEYWORD_CREATE_DIRS_DAY "subdir_pr_day"
#define KEYWORD_CAM_DIM_WIDTH   "cam_img_width"
#define KEYWORD_CAM_DIM_HEIGHT  "cam_img_height"

class TimelapseConfig 
{
private:
  int interval_s;
  int subdir_pr_day_enable;
  int camera_image_width;
  int camera_image_height;
  char name[80];
public:
  TimelapseConfig(void);
  int getConfigFromFile(const char *fileName);
}

//int get_config_from_file(const char *filename, timelapse_config_t *config);

#endif
