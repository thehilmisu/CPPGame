#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "raylib.h"

class GameObject
{
public:
	GameObject(Vector3 startPosition, float scale);
	~GameObject() = default;

	void Update(float deltaTime);
};

#endif
