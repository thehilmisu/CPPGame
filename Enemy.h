#ifndef ENEMY_H
#define ENEMY_H

#include <string>

#include "raylib.h"
#include "raymath.h"
#include <vector>

class Enemy
{
public:
    Enemy(const std::string& modelPath, const std::string& texturePath,Vector3 startPosition, Vector3 size);
    ~Enemy();

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
    float enemySpawnTimer = 0.0f;
    std::vector<Enemy> enemies;
};

#endif // ENEMY_H
