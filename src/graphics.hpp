#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/gl.h>
#include <glm/glm.hpp>

class Graphics {
  public:
    Graphics();

    static void init(int, int, bool);
    static void init(bool);
    static void clearScreen();
  private:
    static int width;
    static int height;

    static void initGL();
    static void initGlfw(bool fullscreen);
    static void initBuffers();
};

#endif
