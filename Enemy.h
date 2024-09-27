#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "raymath.h"

class Enemy
{
public:
    Enemy(Vector3 startPosition, Vector3 size);
    ~Enemy();

    void Update(float deltaTime, Vector3 targetPosition);
    void Draw();

    Vector3 GetPosition() const;
    bool IsActive() const;
    void Deactivate();

private:
    Vector3 position;
    Vector3 size;
    Color color;
    bool active;
};

#endif // ENEMY_H
