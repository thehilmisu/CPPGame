#ifndef TREE_H
#define TREE_H

#include "GameObject.h"

class Tree : public GameObject
{
public:
    Tree(Vector3 startPosition, float scale);
	
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

private:
    void GeneratePosition();
};

#endif // Tree
