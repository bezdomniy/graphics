#pragma once

#include <SDL.h>
#include <vector>
#include <cmath>

#include "camera.h"
#include "renderer.h"
#include "world.h"

class Window
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    static const int PIXELS_PER_BATCH = 20000;
    SDL_Event event;
    bool running = false;

    int width;
    int height;

    std::string sceneDesc;
    std::shared_ptr<Camera> camera;

    bool somethingChanged = false;

    Renderer rayTraceRenderer;

    World world;
    std::vector<uint8_t> byteBuffer;
    size_t bufferLength;

    void handleEvents();
public:
    Window(const std::string &sceneDesc);
    ~Window();

    void initWindow();
    void update();
    void draw();
    void run();
};



