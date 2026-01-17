#pragma once
#include "Cube.h"
#include <vector>
#include <cstdlib>

struct Particle {
    Vec3 pos;
    Vec3 velocity;
    float life;
};

class ParticleSystem {
public:
    Cube cubeRenderer;
    std::vector<Particle> particles;

    void init(Core* core, int count) {
        cubeRenderer.init(core);
        particles.clear();
        particles.reserve(count);
        for (int i = 0; i < count; ++i) {
            Particle p;
            float x = (float)(rand() % 400 - 200) / 10.0f; // -20 to 20
            float y = (float)(rand() % 400 - 200) / 10.0f;
            float z = (float)(rand() % 400 - 200) / 10.0f;
            p.pos = Vec3(x, y, z);
            p.velocity = Vec3(((rand() % 100) - 50) / 50.0f, ((rand() % 100) - 50) / 50.0f, ((rand() % 100) - 50) / 50.0f);
            p.life = (rand() % 100) / 100.0f;
            particles.push_back(p);
        }
    }

    void update(float dt) {
        for (auto &p : particles) {
            p.pos.y += p.velocity.y * dt;
            if (p.pos.y > 20.0f) {
                p.pos.y = -20.0f;
                p.pos.x = (float)(rand() % 400 - 200) / 10.0f;
                p.pos.z = (float)(rand() % 400 - 200) / 10.0f;
            }
        }
    }

    void spawnAt(const Vec3& position, int count = 16) {
        for (int i = 0; i < count; ++i) {
            Particle p;
            p.pos = position;
            p.velocity = Vec3(((rand() % 200) - 100) / 50.0f, ((rand() % 200)) / 50.0f, ((rand() % 200) - 100) / 50.0f);
            p.life = 1.0f; // could implement life-based fade
            particles.push_back(p);
        }
    }

    void draw(Core* core, Matrix& vp, float time, const Vec3& camPos) {
        for (auto &p : particles) {
            Matrix w;
            w.translation(p.pos);
            w.scaling(Vec3(0.05f, 0.05f, 0.05f));
            cubeRenderer.draw(core, w, vp, time, camPos);
        }
    }
};
