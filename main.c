#include <EGL/egl.h>
#include <glad/glad.h>
#include <cstdio>
#include <cstring>
#include "png.h"

#define WIDTH 50
#define HEIGHT 50

// These are implemented in separate file
extern void compileShaders();
extern void renderScene();

static const EGLint configAttribs[] = {
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_NONE
};

static const int pbufferWidth = WIDTH; // 9;
static const int pbufferHeight = HEIGHT; // 9;

static const EGLint pbufferAttribs[] = {
      EGL_WIDTH, pbufferWidth,
      EGL_HEIGHT, pbufferHeight,
      EGL_NONE,
};

const EGLint context_attrib_list[] = {
        // request a context using Open GL ES 2.0
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
};

void checkError() {
  GLenum err;
  while((err = glGetError()) != GL_NO_ERROR)
  {
    fprintf(stderr, "OPenGL Error: %d", err);
  }
}

void printScreen() {
  GLint viewPortParams[4];
  glGetIntegerv(GL_VIEWPORT, viewPortParams);
  const int width = viewPortParams[2];
  const int height = viewPortParams[3];

  printf("viewPort is %d x %d\n", width, height);

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadBuffer(GL_FRONT);

  unsigned char buffer[WIDTH*HEIGHT*4];
  glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &buffer);

  if(0) {
    for(int y=height-1;y>=0;y--) {
      for(int x=0;x<width;x++) {
        printf("%02x%02x%02x%02x ", buffer[4*(y*width+x)+0], buffer[4*(y*width+x)+1], buffer[4*(y*width+x)+2], buffer[4*(y*width+x)+3]);
      }
      printf("\n");
    }
  } else {
    for(int y=height-1;y>=0;y--) {
      for(int x=0;x<width;x++) {
        unsigned char r,g,b,a;
        r=buffer[4*(y*width+x)+0];
        g=buffer[4*(y*width+x)+1];
        b=buffer[4*(y*width+x)+2];
        a=buffer[4*(y*width+x)+3];

        if(r!=0 || g!=0 || b!=0) {
          printf("*");
        } else {
          printf(".");
        }
      }
      printf("\n");
    }
  }
  checkError();
  printf("printScreen done\n");
}

int writeImage(char* filename, int width, int height, unsigned char *buffer, char* title);

void writeScreen(char *filename) {
  GLint viewPortParams[4];
  glGetIntegerv(GL_VIEWPORT, viewPortParams);
  const int width = viewPortParams[2];
  const int height = viewPortParams[3];

  printf("viewPort is %d x %d\n", width, height);

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadBuffer(GL_FRONT);

  unsigned char buffer[WIDTH*HEIGHT*4];
  glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &buffer);

  writeImage(filename, width, height, buffer, "Headless OpenGL");
}

int main(int argc, char *argv[])
{
  EGLint major, minor;
  char filename[100];

  if(argc>1) {
    strcpy(filename, argv[1]);
  } else {
    strcpy(filename, "image.png");
  }

  // Initialize EGL
  EGLDisplay eglDpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if (eglDpy == EGL_NO_DISPLAY) {
        fprintf(stderr, "Failed to get EGL display\n");
        return -1;
  }

  eglInitialize(eglDpy, &major, &minor);
  if (eglInitialize(eglDpy, &major, &minor) == EGL_FALSE) {
        fprintf(stderr, "Failed to initialize EGL display: %x\n", eglGetError());
        return -1;
  }
  printf("EGL version %d.%d\n", major, minor);

  // Select a configuration
  EGLint numConfigs;
  EGLConfig eglCfg;

  eglChooseConfig(eglDpy, configAttribs, &eglCfg, 1, &numConfigs);
  if (numConfigs != 1) {
      fprintf(stderr, "Failed to choose exactly 1 config, chose %d\n", numConfigs);
      return -1;
  }

  // Create a surface
  EGLSurface eglSurf = eglCreatePbufferSurface(eglDpy, eglCfg, pbufferAttribs);

  // Bind the OpenGL API
  eglBindAPI(EGL_OPENGL_API);

  // Create a context and make it current
  EGLContext eglCtx = eglCreateContext(eglDpy, eglCfg, EGL_NO_CONTEXT, context_attrib_list);
  printf("eglCreateContext eglGetError: %d (EGL_SUCCESS=%d)\n", eglGetError(), EGL_SUCCESS);

  eglMakeCurrent(eglDpy, eglSurf, eglSurf, eglCtx);
  printf("eglMakeCurrent eglGetError: %d (EGL_SUCCESS=%d)\n", eglGetError(), EGL_SUCCESS);

  printf("EGL EGL_CLIENT_APIS: %s\n", eglQueryString(eglDpy, EGL_CLIENT_APIS));
  printf("EGL EGL_VENDOR: %s\n", eglQueryString(eglDpy, EGL_VENDOR));
  printf("EGL EGL_VERSION: %s\n", eglQueryString(eglDpy, EGL_VERSION));
  printf("EGL EGL_EXTENSIONS: %s\n", eglQueryString(eglDpy, EGL_EXTENSIONS));

  // Inititliaze OpenGL
  if(!gladLoadGL()) {
    printf("gladLoadGL Error: \n");
  }
  printf("OpenGL version: %s\n", glGetString(GL_VERSION));

  GLint viewPortParams[2];
  glGetIntegerv(GL_MAX_VIEWPORT_DIMS, viewPortParams);
  printf("Max viewport dimensions: %d x %d\n", viewPortParams[0], viewPortParams[1]);

  glViewport(0, 0, WIDTH, HEIGHT);
  checkError();

  // Render scene and write framebuffer to file
  compileShaders();
  render();
  eglSwapBuffers(eglDpy,eglSurf);
  printScreen();
  writeScreen(argv[1]);

  // Terminate EGL
  eglTerminate(eglDpy);
  return 0;
}
