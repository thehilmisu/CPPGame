#ifndef PLANE_H
#define PLANE_H

#include "raylib.h"
#include "raymath.h"
#include <string>

class Plane
{
public:
    // Constructor and Destructor
    Plane(const std::string& modelPath, const std::string& texturePath, Vector3 startPosition);
    ~Plane();

    // Update and Draw methods
    virtual void Update(float deltaTime);
    virtual void Draw();

    // Position, Rotation, and Scale setters and getters
    void SetPosition(const Vector3& position);
    Vector3 GetPosition() const;

    void SetRotation(const Quaternion& rotation);
    Quaternion GetRotation() const;

    void SetScale(const float& scale);
    float GetScale() const;

    // Movement methods
    void Move(const Vector3& direction, float speed, float deltaTime);
    void Rotate(const Vector3& axis, float angleDegrees);

    // LookAt method (if needed)
    void LookAt(const Vector3& targetPosition);

    // Update the model's transform matrix
    void UpdateTransform();

    // Update rotation based on pitch, yaw, roll
    void UpdateRotation();

    void HandleInput(float deltaTime);

private:
    Model model;          // 3D model of the player
    Texture2D texture;    // Texture applied to the model
    Vector3 position;     // Player position in the world
    Quaternion rotation;  // Player rotation
    float scale;        // Scale of the model

    // Movement variables
    float pitch;
    float yaw;
    float roll;
    float altitude;
    float speed;


};

#endif // PLANE_H
