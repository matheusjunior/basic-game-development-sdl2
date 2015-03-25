
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

}
#endif // canhao
