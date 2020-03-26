#include "gui.h"
#include <GL/freeglut.h>
#include <SOIL.h>

GLuint active_texture;

void gui_render()
{
  glClearColor( 0, 0, 0, 1 );  // (In fact, this is the default.)
  glClear( GL_COLOR_BUFFER_BIT );
  
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, active_texture);  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthMask(GL_FALSE);
  glDisable(GL_DEPTH_TEST);  
  glBegin( GL_QUADS );
  glTexCoord2d(0.0,0.0); glVertex2d(-1.0,-1.0);
  glTexCoord2d(1.0,0.0); glVertex2d(+0.5,-1.0);
  glTexCoord2d(1.0,1.0); glVertex2d(+0.5,+0.5);
  glTexCoord2d(0.0,1.0); glVertex2d(-1.0,+0.5);
  glEnd();
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
  glutSwapBuffers(); // Required to copy color buffer onto the screen.
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
