#include "GameObject.h"

GameObject::~GameObject(){}

Vector3 GameObject::GetPosition() {
	return _position;
}

void GameObject::SetPosition(Vector3 position){
	_position = position;
}

bool GameObject::IsActive() {
	return active;
}

void GameObject::Deactivate(){
	active = false;
}

void GameObject::SetScale(float scale){
	_scale = scale;
}

float GameObject::GetScale(){
	return _scale;
}
