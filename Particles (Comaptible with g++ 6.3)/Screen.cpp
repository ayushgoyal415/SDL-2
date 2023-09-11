#include "Screen.h"

namespace ayush {
// Screen Class
Screen::Screen()
    : m_window(NULL),
      m_renderer(NULL),
      m_texture(NULL),
      m_buffer1(NULL),
      m_buffer2(NULL){};
bool Screen::init() {
    SDL_Init(SDL_INIT_VIDEO);

    m_window = SDL_CreateWindow(
        "Fire Particle Explosion", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if (m_window == NULL) {
        SDL_Quit();
        return false;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);

    if (m_renderer == NULL) {
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
                                  SDL_TEXTUREACCESS_STATIC, WIDTH, HEIGHT);

    if (m_texture == NULL) {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    m_buffer1 = new Uint32[WIDTH * HEIGHT];
    m_buffer2 = new Uint32[WIDTH * HEIGHT];

    memset(m_buffer1, 0, WIDTH * HEIGHT * sizeof(Uint32));
    memset(m_buffer2, 0, WIDTH * HEIGHT * sizeof(Uint32));

    return true;
};
void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;

    Uint32 color = 0;
    color += red;
    color <<= 8;
    color += green;
    color <<= 8;
    color += blue;
    color <<= 8;
    color += 0xFF;

    m_buffer1[(y * WIDTH) + x] = color;
};
void Screen::update() {
    SDL_UpdateTexture(m_texture, NULL, m_buffer1, WIDTH * sizeof(Uint32));
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
    SDL_RenderPresent(m_renderer);
};
void Screen::boxBlur() {
    Uint32* temp = m_buffer1;
    m_buffer1 = m_buffer2;
    m_buffer2 = temp;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int redTotal = 0;
            int greenTotal = 0;
            int blueTotal = 0;
            for (int row = -1; row <= 1; row++) {
                for (int col = -1; col <= 1; col++) {
                    int currX = x + col;
                    int currY = y + row;
                    if (currX >= 0 && currX < WIDTH && currY >= 0 &&
                        currY < HEIGHT) {
                        Uint32 color = m_buffer2[currY * WIDTH + currX];
                        Uint8 red = color >> 24;
                        Uint8 green = color >> 16;
                        Uint8 blue = color >> 8;
                        redTotal += red;
                        greenTotal += green;
                        blueTotal += blue;
                    }
                }
            }
            Uint8 red = redTotal / 9;
            Uint8 green = greenTotal / 9;
            Uint8 blue = blueTotal / 9;
            setPixel(x, y, red, green, blue);
        }
    }
};
bool Screen::processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) return false;
    }
    return true;
};
void Screen::close() {
    delete[] m_buffer1;
    delete[] m_buffer2;
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyTexture(m_texture);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
};

// Particle Class
Particle::Particle() { init(); };
void Particle::init() {
    m_x = 0;
    m_y = 0;
    m_direction = (2 * M_PI * rand()) / RAND_MAX;
    m_speed = (0.04 * rand()) / RAND_MAX;
    m_speed *= m_speed;
}
void Particle::update(int interval) {
    m_direction += interval * 0.0003;

    double x_speed = m_speed * cos(m_direction);
    double y_speed = m_speed * sin(m_direction);

    m_x += x_speed * interval;
    m_y += y_speed * interval;

    if (m_x < -1 || m_x > 1 || m_y < -1 || m_y > 1) init();
    if (rand() < RAND_MAX / 100) init();
}

}  // namespace ayush
