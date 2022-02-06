#include<SDL2/SDL.h>

class GridColorTexturer{
private:
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    uint32_t width, height;
public:
    GridColorTexturer(SDL_Renderer* renderer, int width, int height);
    ~GridColorTexturer();
    void blackWhite(const u_int8_t* bwArr);
    const SDL_Texture* getTexture();
};