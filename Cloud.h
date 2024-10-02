#ifndef CLOUD_H
#define CLOUD_H

#include <string>

#include "raylib.h"
#include "raymath.h"
#include <vector>

class Cloud
{
public:
    Cloud(const std::string& modelPath, const std::string& texturePath,Vector3 startPosition, Vector3 size);
    ~Cloud();

    void Update(float deltaTime, Vector3 targetPosition);
    void Draw();

    Vector3 GetPosition() const;
    bool IsActive() const;
    void Deactivate();
    void Unload();

private:
    Model model;          // 3D model
    Texture2D texture;    // Texture applied to the model
    Vector3 position;
    Vector3 size;
    bool active;
};

#endif // ENEMY_H
