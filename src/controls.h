#ifndef CONTROLS_H
#define CONTROLS_H

class controls{

  public:
    controls();
    void render();
    float getPlayerX();
    float getPlayerY();
    float getPlayerZ();
    

  private:
    float playerX;
    float playerY;
    float playerZ;
    float x;
    float y;
    float z;
};

#endif
