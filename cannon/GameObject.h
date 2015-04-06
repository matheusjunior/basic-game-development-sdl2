#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#import <SDL2/SDL_render.h>
#import <SDL2/SDL_events.h>

/* TODO Document class
* */
class GameObject
{
public:
    SDL_Texture *texture; // GameObject Texture
    SDL_Rect position; // GameObject position in space
    double speedX; // x axis speed in pixels/sec
    double speedY; // y axis speed in pixels/sec
    double timeStart; // TODO Document variable
    double currentTime; // TODO Document variable
    double deltaT; // TODO Document Variable

    GameObject()
    {
    }

    GameObject(int x, int y, int w, int h);

    GameObject(int x, int y, int w, int h, double speed);

    /* Move object along x axis
    * @param y represents the elapsed time in ms to move GameObject according to its speed given in pixels/sec
    * */
    void moveX(double dTime);

    /* Move object along y axis
    * @param y represents the elapsed time in ms to move GameObject according to its speed given in pixels/sec
    * */
    void moveY(double dTime);

    /* Change GameObject axis x speed
    * @param v new speed in pixels/sec
    * */
    void setSpeedX(double v);

    /* Change GameObject axis x speed
    * @param v new speed in pixels/sec
    * */
    void setSpeedY(double v);

    void updateSpeedX(double acceleration, double dTime);

    void updateSpeedY(double acceleration, double dTime);


    /* Set GameObject texture
    * @param tex texture to be applied
    * */
    void setTexture(SDL_Texture *tex);

    /* Draw GameObject using its own texture
    * @param gRenderer SDL_Render to apply the texture
    * */
    virtual void draw(SDL_Renderer *gRenderer);

    /* TODO Document function
    * */
    void Update();

    //private:
    // SDL_Surface surface;
};

#endif // objeto
