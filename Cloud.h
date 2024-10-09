#ifndef CLOUD_H
#define CLOUD_H

#include <string>

#include "raylib.h"
#include "raymath.h"
#include <vector>

class Cloud
{
public:
    Cloud(Vector3 startPosition, float scale);
    ~Cloud() = default;

    void Update(float deltaTime);
    void Draw();

    Vector3 GetPosition() const;
    bool IsActive() const;
    void Deactivate();

private:
    Vector3 position;
    float scale;
    bool active;

    Vector3 GeneratePosition();
};

#endif // ENEMY_H
