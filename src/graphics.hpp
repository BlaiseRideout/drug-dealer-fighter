#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/gl.h>
#include <glm/glm.hpp>

class Graphics {
  public:
    static int width;
    static int height;
    static glm::mat4 projection;
    static glm::mat4 orthographic;

    Graphics();

    static void init(int, int, bool);
    static void init(bool);
    static void clearScreen();
  private:
    static void initGL();
    static void initGlfw(bool fullscreen);
    static void initBuffers();
};

#endif
