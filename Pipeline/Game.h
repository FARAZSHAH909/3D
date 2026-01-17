#pragma once
#include "Camera.h"
#include "LevelLoader.h"
#include "GameObject.h"
#include "ParticleSystem.h"
#include "StaticMesh.h"
#include "core.h"
#include "maths.h"
#include "window.h"
#include <algorithm>
#include <string>
#include <vector>


class Game {
public:
  Window win;
  Core core;
  Camera cam;
  GamesEngineeringBase::Timer tim;
  ParticleSystem particles;

  // Scene Objects
  std::vector<GameObject> objects;

  // Collectible tracking
  int totalCollectibles = 0;
  int collected = 0;

  // Game State
  int score = 0;
  float time = 0.0f;
  bool isRunning = true;

  // Anti-Gravity State
  float gravityScale = 1.0f; // 1.0 = Normal, 0.5 = Low, 0.0 = Zero
  Vec3 playerVelocity = Vec3(0, 0, 0);
  bool onGround = false;

  void initialize() {
    win.initialize("Anti-Gravity Game", 1024, 768);
    if (!win.hwnd) { isRunning = false; return; }
    core.init(win.hwnd, 1024, 768);
    if (!core.device) { isRunning = false; return; }

    // Initialize Camera
    cam.init(Vec3(0.0f, 2.0f, -10.0f), (float)win.width / (float)win.height);

    // Initialize Particles
    particles.init(&core, 100);

    // Load Level
    LevelLoader loader;
    std::vector<StaticMesh> loadedMeshes;
    loader.load("level.json", &core, loadedMeshes);

    // convert loaded meshes into game objects (use prototype pointers)
    for (auto &m : loadedMeshes) {
      GameObject obj;
      // store prototype in heap to maintain lifetime (simple approach)
      StaticMesh* proto = new StaticMesh(m);
      obj.prototype = proto;
      obj.world = m.worldMatrix;
      obj.type = m.type;
      objects.push_back(obj);
    }

    // count collectibles
    totalCollectibles = 0;
    for (auto &o : objects) if (o.type == "collectible") totalCollectibles++;
  }

  void update(float dt) {
    win.processMessages();
    if (win.keys[VK_ESCAPE] == 1) {
      isRunning = false;
    }

    // Gravity Control
    if (win.keys['1'])
      gravityScale = 1.0f; // Normal
    if (win.keys['2'])
      gravityScale = 0.2f; // Low
    if (win.keys['3'])
      gravityScale = 0.0f; // Zero

    time += dt;

    // --- Player Physics ---
    float gravity = -9.8f * gravityScale;
    if (gravityScale == 0.0f)
      gravity = 0.0f; // True Zero G

    // Apply Gravity
    playerVelocity.y += gravity * dt;

    // Friction / Damping
    playerVelocity.x *= 0.95f;
    playerVelocity.z *= 0.95f;
    if (gravityScale == 0.0f)
      playerVelocity.y *= 0.98f; // Air resistance in zero G

    // Input Force (Simplified)
    float moveSpeed = 20.0f * dt;
    if (win.keys['W'])
      playerVelocity.z += moveSpeed;
    if (win.keys['S'])
      playerVelocity.z -= moveSpeed;
    if (win.keys['A'])
      playerVelocity.x += moveSpeed;
    if (win.keys['D'])
      playerVelocity.x -= moveSpeed;

    // Jump / Up-Thrust
    if (win.keys[VK_SPACE]) {
      if (onGround || gravityScale < 0.5f) {
        playerVelocity.y += 10.0f * dt; // Thrust
      }
    }

    // Apply Velocity
    cam.position.x += playerVelocity.x * dt;
    cam.position.y += playerVelocity.y * dt;
    cam.position.z += playerVelocity.z * dt;

    // Ground Floor (Hardcoded for now)
    if (cam.position.y < 0.5f) {
      cam.position.y = 0.5f;
      playerVelocity.y = 0;
      onGround = true;
    } else {
      onGround = false;
    }

    // Camera Look Logic (Target update)
    // Since we don't have mouse delta easily, use Arrow Keys to rotate view?
    // Or just fixed look at 0,0,0 + offset?
    // Let's make target slightly ahead of player for third person feel or just
    // FPS? Simple: Always look forward (Z+) plus some offset? cam.target =
    // cam.position + Vec3(0, 0, 10.0f); // Look forward Actually, Camera class
    // uses 'target' to build view matrix.
    cam.target = Vec3(0, 5, 0); // Look at center of world (Arena style)

    cam.update(dt);

    // Update Particles
    particles.update(dt);

    // --- Collision & Collection ---
    for (int i = 0; i < objects.size(); i++) {
      GameObject &obj = objects[i];
      if (!obj.prototype) continue;
      if (obj.prototype->checkCollision(cam.position)) {
        // Collision!
        // Check type
        // Note: We need 'type' member in StaticMesh.h (Added!)

        // Assuming we can access 'type' (public)
        // Need to use string compare
        // Wait, I declared 'type' in StaticMesh.h in previous step?
        // Yes, 'std::string type = "static";'

        // If Collectible
        if (obj.type == "collectible") {
          // Collect!
          score++;
          Vec3 pickupPos = obj.world.mulPoint(Vec3(0,0,0));
          particles.spawnAt(pickupPos, 24);
          // Remove object and free prototype
          delete obj.prototype;
          objects[i] = objects.back();
          objects.pop_back();
          i--; // adjust index

          // Check win
          collected++;
          if (collected >= totalCollectibles) {
              MessageBoxA(NULL, "All Energy Cores collected! Portal activated. You win!", "Level Complete", MB_OK | MB_ICONINFORMATION);
              isRunning = false;
          }
        } else {
          // Hit Wall/Static
          // Push back slightly (Simple response)
          cam.position -= playerVelocity * dt * 2.0f; // Revert move
          playerVelocity = Vec3(0, 0, 0);
        }
      }
    }
  }

  void render() {
    core.beginFrame();

    Matrix vp = cam.getViewProjection();

    // Draw GameObjects
    for (auto &obj : objects) {
      if (obj.prototype) obj.draw(&core, vp, time, cam.position);
    }

    // Draw Particles
    particles.draw(&core, vp, time, cam.position);

    core.finishFrame();
  }

  void run() {
    initialize();
    while (isRunning) {
      float dt = tim.dt();
      update(dt);
      render();
    }
    core.flushGraphicsQueue();
  }
};
