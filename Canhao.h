
#ifndef canhao
#define canhao

class Canhao : public Objeto {
public:
    Canhao(int x, int y, int w, int h, double vel);

    void handleInput(SDL_Event e);
};

Canhao::Canhao(int x, int y, int w, int h, double vel) : Objeto(x,y,w,h,vel) {
}

void Canhao::handleInput(SDL_Event e) {

    switch (e.key.keysym.sym) {

    case SDLK_SPACE:

        break;
    case SDLK_RIGHT:
        posicao.x += 7;
        if (posicao.x + posicao.w > SCREEN_WIDTH) posicao.x = SCREEN_WIDTH - posicao.w;
        break;
    case SDLK_LEFT:
        posicao.x -= 7;
        if (posicao.x + posicao.w > SCREEN_WIDTH) posicao.x = SCREEN_WIDTH - posicao.w;
        break;
    default:
        break;
    }
}
#endif // canhao
