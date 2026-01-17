#pragma once
#include "StaticMesh.h"

struct GameObject {
    StaticMesh* prototype = nullptr; // pointer to shared mesh data
    Matrix world;
    std::string type = "static";

    // convenience access
    void draw(Core* core, Matrix& vp, float time, const Vec3& camPos) {
        if (prototype) prototype->draw(core, world, vp, time, camPos);
    }
};
