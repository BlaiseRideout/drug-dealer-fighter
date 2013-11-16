#ifndef SCREEN_H
#define SCREEN_H

class Screen {
  public:
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
};

#endif