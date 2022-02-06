#include"GridColorTexturer.hpp"
#include<thread>
#include<vector>

GridColorTexturer::GridColorTexturer(SDL_Renderer* renderer, int width, int height){

    this->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    this->renderer = renderer;
    this->width = width;
    this->height = height;
}
GridColorTexturer::~GridColorTexturer(){

}

void blackWhitePartMapper(uint32_t* pixelbufferPart, const uint8_t* bwArr, size_t size){
    for(uint32_t i = 0; i < size; i++){
        pixelbufferPart[i] = (bwArr[i]) ? 0x00FFFFFFu : 0u;
    }
}
void GridColorTexturer::blackWhite(const u_int8_t* bwArr){
    int32_t pitch = 0;
    uint32_t* pixelbuffer = nullptr;

    if(!SDL_LockTexture(this->texture, NULL, (void**)&pixelbuffer, &pitch)){
        pitch /= sizeof(*pixelbuffer);
        
        uint32_t gridSummary = this->width * this->height;
        uint32_t threadCount = std::thread::hardware_concurrency() - 1;
        std::thread* workers = new std::thread[threadCount];
        uint32_t jpt = gridSummary / threadCount;
        for(int i = 0; i < threadCount; i++){
            workers[i] = std::thread([=](){
                blackWhitePartMapper(&pixelbuffer[i * jpt], &bwArr[i * jpt], jpt);
            });
        }
        for(int i = 0; i < threadCount; i++){
            if(workers[i].joinable())
                workers[i].join();
        }
        delete[] workers;
        for(uint32_t i = 0; i < this->width * this->height; i++){
            pixelbuffer[i] = (bwArr[i]) ? 0x00FFFFFFu : 0u;
        }

        SDL_UnlockTexture(this->texture);
    }
}
const SDL_Texture* GridColorTexturer::getTexture(){
    return this->texture;
}