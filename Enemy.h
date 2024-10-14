#ifndef ENEMY_H
#define ENEMY_H

#include <string>

#include "raylib.h"
#include "raymath.h"
#include <vector>

class Enemy
{
public:
    Enemy(Vector3 startPosition, Vector3 size);
    ~Enemy() = default;

    void Update(float deltaTime, const Vector3 targetPosition);
    void Draw();

    Vector3 GetPosition() const;
    bool IsActive() const;
    void Deactivate();

private:
    Vector3 position;
    Vector3 size;
    bool active;
};

#endif // ENEMY_H
