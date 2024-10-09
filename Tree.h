#ifndef TREE_H
#define TREE_H

#include "raylib.h"

class Tree
{
public:
    Tree(Vector3 startPosition, float scale);
    ~Tree() = default;

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

#endif // Tree
