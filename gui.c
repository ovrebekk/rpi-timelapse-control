#include "gui.h"
#include <GL/freeglut.h>
#include <SOIL.h>

GLuint active_texture;
static void draw_quad(float x, float y, float x2, float y2)
{
  glBegin( GL_QUADS );
  glTexCoord2d(0.0,0.0); glVertex2d(x, y);
  glTexCoord2d(1.0,0.0); glVertex2d(x2, y);
  glTexCoord2d(1.0,1.0); glVertex2d(x2, y2);
  glTexCoord2d(0.0,1.0); glVertex2d(x, y2);
  glEnd();
}
void gui_render()
{
  glClearColor( 0, 0, 0, 1 );  // (In fact, this is the default.)
  glClear( GL_COLOR_BUFFER_BIT );
  glLoadIdentity();
  gluOrtho2D(0, 100, 0, 100);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, active_texture);  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glDepthMask(GL_FALSE);
  glDisable(GL_DEPTH_TEST);  
  draw_quad(-100, -100, 200, 200);
  draw_quad(0, 0, 50, 50);
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
  glutSwapBuffers(); // Required to copy color buffer onto the screen.
}

void gui_resize(int width, int height)
{
  
}

int gui_set_display_image(const char *fileName)
{
  active_texture = SOIL_load_OGL_texture(fileName, 
                                        SOIL_LOAD_AUTO, 
                                        SOIL_CREATE_NEW_ID,
                                        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
  glutPostRedisplay();
  return (active_texture != 0);
}
