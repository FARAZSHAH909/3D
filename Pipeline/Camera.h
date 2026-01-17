#pragma once
#include "maths.h"
#include "window.h" // For input handling
#include <cmath>

class Camera
{
public:
    Vec3 position;
    Vec3 target;
    Vec3 up;

    float yaw;
    float pitch;
    
    // Projection parameters
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    // Computed matrices
    Matrix viewMatrix;
    Matrix projectionMatrix;

    Camera() 
        : position(0.0f, 0.0f, -10.0f), 
          target(0.0f, 0.0f, 0.0f), 
          up(0.0f, 1.0f, 0.0f),
          yaw(0.0f), pitch(0.0f),
          fov(60.0f), aspectRatio(1.0f), nearPlane(0.1f), farPlane(1000.0f)
    {
    }

    void init(Vec3 startPos, float aspect)
    {
        position = startPos;
        aspectRatio = aspect;
        updateMatrices();
    }

    void update(float dt)
    {
        // Placeholder for advanced update logic if needed
        updateMatrices();
    }

    void handleInput(const GamesEngineeringBase::Window& win, float dt)
    {
        float speed = 10.0f * dt;
        float rotSpeed = 2.0f * dt;

        // Basic WASD Movement (Flat plane for now)
        if (win.keyPressed('W')) position.z += speed; // Move forward (Z+)
        if (win.keyPressed('S')) position.z -= speed; // Move backward (Z-)
        if (win.keyPressed('A')) position.x += speed; // Move left
        if (win.keyPressed('D')) position.x -= speed; // Move right
        if (win.keyPressed('Q')) position.y += speed; // Up
        if (win.keyPressed('E')) position.y -= speed; // Down

        // Simple look at updating or rotating could go here
        // For now, we just keep looking at 0,0,0 or a fixed offset logic
        // But with a real Free/FPS camera, we would update target based on yaw/pitch
        
        // Example orbit or look behavior could be added here
    }

    void updateMatrices()
    {
        // Recalculate Projection
        // Using the instance-method style found in main.cpp: Matrix p = p.perspectiveProjection(...)
        Matrix p; 
        projectionMatrix = p.perspectiveProjection(aspectRatio, fov, nearPlane, farPlane);

        // Recalculate View
        // Using lookAtMatrix style
        Matrix v;
        viewMatrix = v.lookAtMatrix(position, target, up);
    }

    Matrix getViewProjection()
    {
        return projectionMatrix.multiply(viewMatrix);
    }
    
    void setAspectRatio(float ratio) {
        aspectRatio = ratio;
        updateMatrices();
    }
};
