#pragma once
#include<stdint.h>
enum GoLSates{
    GOL_DEAD = 0x00,
    GOL_LIVING = 0x01
};

class GoLSimulator{
private:
    uint32_t width, height;
    uint8_t* playfield = nullptr;
    uint8_t* countingfield = nullptr;

    void countNeightbourSector(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
public:
    GoLSimulator(uint32_t width, uint32_t height);
    ~GoLSimulator();

    void update();
    void clear();
    void setLife(uint32_t posX, uint32_t posY, bool living);
    bool getLife(uint32_t posX, uint32_t posY);

    uint32_t getWidth();
    uint32_t getHeight();
    const uint8_t* getPlayfield();
    void setPlayfield(const uint8_t* field); // debug purposes only
};