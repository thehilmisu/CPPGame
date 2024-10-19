#ifndef CLOUD_H
#define CLOUD_H

#include <string>
#include "GameObject.h"
#include "raymath.h"
#include <vector>

class Cloud : public GameObject
{
public:
    Cloud(Vector3 startPosition, float scale);

    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

private:
    void GeneratePosition();
};

#endif // CLOUD_H
