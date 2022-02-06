#include<SDL2/SDL.h>
#include<iostream>
#include"GoL-Simulator.hpp"
#include"RandomGen.hpp"
#include"GridColorTexturer.hpp"
#include<thread>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define GRID_WIDTH 3840
#define GRID_HEIGHT 2160
#define SCROLL_SPEED 10

int main(){
    uint32_t width = GRID_WIDTH;
    uint32_t height = GRID_HEIGHT;
    GoLSimulator golSim(width, height);
    
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("GOL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    GridColorTexturer gridTexturer(renderer, width, height);
    
    SDL_Event event;
    bool running = true;
    SDL_Rect viewpos = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    bool paused = true;
    int updateEveryNthFrame = 30;
    int frameCount = 0;
    bool leftMouseDown = false;
    while(running){

        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                running = false;
            }
            if(event.type == SDL_KEYDOWN){
                switch (event.key.keysym.sym)
                {
                case SDLK_DOWN:
                    updateEveryNthFrame += 1;
                    break;
                case SDLK_UP:
                    updateEveryNthFrame -= 1;
                    break;
                case SDLK_SPACE:
                    paused = !paused;
                    break;
                case SDLK_r:
                    {
                        uint8_t* pf = new uint8_t[width * height];
                        for(int i = 0; i < width * height; i++){
                            pf[i] = (ldc::RandomGen::generate(0, 100) < 30) ? 1 : 0;
                        }

                        golSim.setPlayfield(pf);
                    }
                    break;
                case SDLK_c:
                case SDLK_DELETE:
                    golSim.clear();
                    break;
                case SDLK_ESCAPE:
                    viewpos.x = 0;
                    viewpos.y = 0;
                    viewpos.w = WINDOW_WIDTH;
                    viewpos.h = WINDOW_HEIGHT;
                    break;
                default:
                    break;
                }
            }
            if(event.type == SDL_MOUSEBUTTONDOWN){
                if(!leftMouseDown && event.button.button == SDL_BUTTON_LEFT){
                    leftMouseDown = true;
                }
            }
            if(event.type == SDL_MOUSEBUTTONUP){
                if(leftMouseDown && event.button.button == SDL_BUTTON_LEFT){
                    leftMouseDown = false;
                }
            }
            if(event.type == SDL_MOUSEMOTION){
                if(leftMouseDown){
                    viewpos.x += event.motion.xrel;
                    viewpos.y += event.motion.yrel;
                }
            }
            if(event.type == SDL_MOUSEWHEEL){
                int scrollDepth = SCROLL_SPEED * event.wheel.y;
                viewpos.x -= scrollDepth * 2;
                viewpos.y -= scrollDepth;
                viewpos.w += scrollDepth * 4;
                viewpos.h += scrollDepth * 2;
            }
        }
        if(frameCount >= updateEveryNthFrame){
            golSim.update();
            gridTexturer.blackWhite(golSim.getPlayfield());
            frameCount = 0;
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, const_cast<SDL_Texture*>(gridTexturer.getTexture()), NULL, &viewpos);
        SDL_RenderPresent(renderer);
        std::this_thread::sleep_for(std::chrono::milliseconds(13));
        frameCount += 1;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}