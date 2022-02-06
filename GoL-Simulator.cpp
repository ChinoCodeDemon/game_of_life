#include"GoL-Simulator.hpp"
#include<memory.h>
#include<iostream>
#include<thread>
GoLSimulator::GoLSimulator(uint32_t width, uint32_t height){
    this->width = width;
    this->height = height;
    this->playfield = new uint8_t[width * height];
    this->countingfield = new uint8_t[width * height];
}
GoLSimulator::~GoLSimulator(){
    delete[] this->playfield;
    delete[] this->countingfield;
}

void GoLSimulator::countNeightbourSector(uint32_t posX, uint32_t posY, uint32_t w, uint32_t h){
    int8_t inner_x;
    int8_t inner_y;
    for(uint32_t y = posY; y < h; y++){
        for(uint32_t x = posX; x < w; x++){
            this->countingfield[y * this->width + x] = 0;
            for(inner_y = -1; inner_y <= 1; inner_y++){
                for(inner_x = -1; inner_x <= 1; inner_x++){
                    if((y + inner_y) * this->width + (x + inner_x) > this->width * this->height)
                        continue;

                    if(inner_x == 0 && inner_y == 0)
                        continue;

                    if(static_cast<int>(x) + inner_x < 0 || x + inner_x > this->width)
                        continue;

                    if(static_cast<int>(y) + inner_y < 0 || y + inner_y > this->height)
                        continue;

                    this->countingfield[y * this->width + x] += this->playfield[(y + inner_y) * this->width + (x + inner_x)];
                }
            }
        }
    }
}

void GoLSimulator::update(){
    memset(this->countingfield, 0, this->width * this->height);
    uint8_t tempresult;
    
    std::thread upperLeftHalf([=](){
        this->countNeightbourSector(
            0, 
            0, 
            this->getWidth() * 0.5, 
            this->getHeight() * 0.5);
    });
    std::thread upperRightHalf([=](){
        this->countNeightbourSector(
            this->getWidth() * 0.5, 
            0, 
            this->getWidth(), 
            this->getHeight() * 0.5);
    });
    std::thread bottomLeftHalf([=](){
        this->countNeightbourSector(0, 
        this->getHeight() * 0.5, 
        this->getWidth()*0.5, 
        this->getHeight());
    });
    this->countNeightbourSector(this->width*0.5, this->height*0.5, this->width, this->height);
    
    if(upperLeftHalf.joinable())
        upperLeftHalf.join();
    if(upperRightHalf.joinable())
        upperRightHalf.join();
    if(bottomLeftHalf.joinable())
        bottomLeftHalf.join();
    
    for(uint32_t y = 0; y < this->height; y++){
        for(uint32_t x = 0; x < this->width; x++){
            tempresult = this->countingfield[y * this->width + x];
            switch (tempresult)
            {
            case 3:
                tempresult = 1;
                break;
            case 2:
                tempresult = this->playfield[y * this->width + x];
                break;
            case 0:
                break;
            default:
                tempresult = 0;
                break;
            }
            this->playfield[y * this->width + x] = tempresult;
        }
    }
}

void GoLSimulator::clear(){
    memset(this->playfield, 0, this->width * this->height);
}
void GoLSimulator::setLife(uint32_t posX, uint32_t posY, bool living){
    this->playfield[posY * this->width + posX] = (living)? 1 : 0;
}
bool GoLSimulator::getLife(uint32_t posX, uint32_t posY){
    return this->playfield[posY * this->width + posX];
}

uint32_t GoLSimulator::getWidth(){
    return this->width;
}
uint32_t GoLSimulator::getHeight(){
    return this->height;
}
const uint8_t* GoLSimulator::getPlayfield(){
    return this->playfield;
}

void GoLSimulator::setPlayfield(const uint8_t* field){
    memcpy(this->playfield, field, this->width * this->height * sizeof(*this->playfield));
}