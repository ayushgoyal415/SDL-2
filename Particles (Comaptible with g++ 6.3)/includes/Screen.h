#include <SDL2/SDL.h>
#include <bits/stdc++.h>
using namespace std;

#ifndef SCREEN_H
#define SCREEN_H

#pragma once
namespace ayush {

class Screen {
   public:
    const static int WIDTH = 800, HEIGHT = 600;

   private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Texture* m_texture;
    Uint32* m_buffer1;
    Uint32* m_buffer2;

   public:
    Screen();
    bool init();
    void setPixel(int, int, Uint8, Uint8, Uint8);
    void update();
    void boxBlur();
    bool processEvents();
    void close();
    ~Screen(){};
};

class Particle {
   public:
    double m_x;
    double m_y;

   private:
    double m_speed;
    double m_direction;

   private:
    void init();

   public:
    Particle();
    void update(int);
    ~Particle(){};
};

class Swarm {
   public:
    const static int NPARTICLES = 5000;

   private:
    Particle* m_pParticles;
    int lastTime;

   public:
    Swarm() : lastTime(0) { m_pParticles = new Particle[NPARTICLES]; };
    const Particle* const getParticles() { return m_pParticles; };
    ~Swarm() { delete[] m_pParticles; };
    void update(int elapsed) {
        int interval = elapsed - lastTime;
        for (int i = 0; i < NPARTICLES; i++) m_pParticles[i].update(interval);
        lastTime = elapsed;
    };
};

}  // namespace ayush

#endif