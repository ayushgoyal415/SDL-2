#include <Screen.h>

int main(int argc, char* argv[]) {
    srand(time(NULL));

    Screen screen;
    if (!screen.init()) return 1;

    Swarm swarm;

    while (true) {
        // GETTING RANDOM COLORS
        int elapsed = SDL_GetTicks();
        swarm.update(elapsed);
        unsigned char green = (1 + sin(elapsed * 0.0001)) * 128;
        unsigned char red = (1 + sin(elapsed * 0.0002)) * 128;
        unsigned char blue = (1 + sin(elapsed * 0.0003)) * 128;

        // APPLYING RANDOM COLOURS TO PARTICLES
        const Particle* const pParticles = swarm.getParticles();
        for (int i = 0; i < Swarm::NPARTICLES; i++) {
            Particle particle = pParticles[i];
            int x = (particle.m_x + 1) * Screen::WIDTH / 2;
            int y = (particle.m_y * Screen::WIDTH / 2) + (Screen::HEIGHT / 2);
            screen.setPixel(x, y, red, green, blue);
        }

        screen.boxBlur();
        screen.update();
        if (!screen.processEvents()) break;
    }

    screen.close();

    return 0;
}