#ifndef MISSILE_H
#define MISSILE_H

#include <string>
#include "GameObject.h"
#include "raymath.h"
#include <vector>

class Missile : public GameObject
{
public:
    Missile(Vector3 startPosition, float scale);

    virtual void Update(float deltaTime);
    virtual void Draw();

private:
};

#endif // Missile
