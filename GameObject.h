#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "raylib.h"

class GameObject 
{
public:
	GameObject() : _position({0.0f,0.0f,0.0f}), _scale(1.0f), active(true) {}
	virtual ~GameObject() = 0;
	
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;

	Vector3 GetPosition();
	void SetPosition(Vector3 position);
	
	float GetScale();
	void SetScale(float scale);

	bool IsActive();
	void Deactivate();

private:
	Vector3 _position;
	float _scale;
	bool active;
};

#endif
