#ifndef MISSILE_H
#define MISSILE_H

#include <string>

#include "raylib.h"
#include "raymath.h"
#include <vector>

class Missile
{
public:
    Missile(Vector3 startPosition, float scale);
    ~Missile() = default;

    void Update(float deltaTime);
    void Draw();

    Vector3 GetPosition() const;
    bool IsActive() const;
    void Deactivate();
    void Unload();

private:
    Model model;          // 3D model
    Vector3 position;
    float scale;
    bool active;

    Vector3 GeneratePosition();
};

#endif // Missile
