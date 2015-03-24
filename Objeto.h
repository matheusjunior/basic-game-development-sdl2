#ifndef objeto
#define objeto

class Objeto {
public:

    SDL_Texture *texture;
    SDL_Rect posicao;
    double velX;
    double velY;
    double timeStart;
    double currentTime;
    double deltaT;

    Objeto(int x, int y, int w, int h);
    Objeto(int x, int y, int w, int h, double vel);

    void moverX(double tempo);
    void moverY(double tempo);

    void setVelocidadeX(double v);
    void setVelocidadeY(double v);

    void setTexture(SDL_Texture *tex);

    void desenha(SDL_Renderer *gRenderer);

    void Update();
private:
    SDL_Surface surface;
};


Objeto::Objeto(int x, int y, int w, int h) {
    posicao.x = x;
    posicao.y = y;
    posicao.w = w;
    posicao.h = h;

    velX = 10;
    velY = 10;

    texture = NULL;

    timeStart =0;
    currentTime =0;
    deltaT = 0;
}

Objeto::Objeto(int x, int y, int w, int h, double vel) {
    posicao.x = x;
    posicao.y = y;
    posicao.w = w;
    posicao.h = h;

    velX = vel;
    velY = vel;

    texture = NULL;

    timeStart =0;
    currentTime =0;
    deltaT = 0;
}


void Objeto::setVelocidadeX(double v) {
    velX = v;
}

void Objeto::setVelocidadeY(double v) {
    velY = v;
}

void Objeto::setTexture(SDL_Texture *tex) {
    texture = tex;
}

void Objeto::moverX(double tempo) {
    posicao.x += velX * tempo;
}

void Objeto::moverY(double tempo) {
    posicao.y += velY * tempo;
}

void Objeto::Update() {
    posicao.x += velX * (currentTime - timeStart) / 1000;
}

void Objeto::desenha(SDL_Renderer *gRenderer) {
    currentTime = SDL_GetTicks();
    Update();
    SDL_RenderCopy(gRenderer, texture, NULL, &posicao);
    timeStart = currentTime;
}

#endif // objeto
