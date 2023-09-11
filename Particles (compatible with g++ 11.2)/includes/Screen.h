#include <SDL.h>
#include <bits/stdc++.h>
using namespace std;

#ifndef SCREEN_H
#define SCREEN_H

#pragma once

class Screen {
   public:
    const static int WIDTH = 800, HEIGHT = 600;

   private:
    SDL_Window* m_window = NULL;
    SDL_Renderer* m_renderer = NULL;
    SDL_Texture* m_texture = NULL;
    Uint32* m_buffer1 = NULL;
    Uint32* m_buffer2 = NULL;

   public:
    bool init();
    void setPixel(int, int, Uint8, Uint8, Uint8);
    void update();
    void boxBlur();
    bool processEvents();
    void close();
};

class Particle {
   public:
    double m_x, m_y;

   private:
    double m_speed, m_direction;

   public:
    Particle() {init();};

   private:
    void init() {
        m_x = 0, m_y = 0;
        m_direction = 2 * M_PI * rand() / RAND_MAX;
        m_speed = pow((0.04 * rand() / RAND_MAX), 2);
    };

   public:
    void update(int interval) {
        m_direction += interval * 0.0003;

        double x_speed = m_speed * cos(m_direction);
        double y_speed = m_speed * sin(m_direction);

        m_x += x_speed * interval;
        m_y += y_speed * interval;

        if (m_x < -1 || m_x > 1 || m_y < -1 || m_y > 1) init();
        if (rand() < RAND_MAX / 100) init();
    };
};

class Swarm {
   public:
    const static int NPARTICLES = 5000;

   private:
    Particle* m_pParticles;
    int lastTime = 0;

   public:
    Swarm() { m_pParticles = new Particle[NPARTICLES]; };
    ~Swarm() { delete[] m_pParticles; };

    public:
    const Particle* const getParticles() {
        return m_pParticles;
    };
    void update(int elapsed) {
        int interval = elapsed - lastTime;
        for (int i = 0; i < NPARTICLES; i++) m_pParticles[i].update(interval);
        lastTime = elapsed;
    };
};


#endif