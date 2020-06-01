#ifndef __GUI_H
#define __GUI_H

void gui_render();

void gui_resize(int width, int height);

int gui_set_display_image(const char *fileName);

int gui_free_current_image(void);

#endif

