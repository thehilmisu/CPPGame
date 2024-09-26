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
    void Update(float deltaTime);
    void Draw();

    // Input handling
    virtual void HandleInput(float deltaTime);

    // Position, Rotation, and Scale setters and getters
    void SetPosition(const Vector3& position);
    Vector3 GetPosition() const;

    void SetRotation(const Quaternion& rotation);
    Quaternion GetRotation() const;

    void SetScale(const Vector3& scale);
    Vector3 GetScale() const;

    // Movement methods
    void Move(const Vector3& direction, float speed, float deltaTime);
    void Rotate(const Vector3& axis, float angleDegrees);

    // LookAt method (if needed)
    void LookAt(const Vector3& targetPosition);

private:
    Model model;          // 3D model of the player
    Texture2D texture;    // Texture applied to the model
    Vector3 position;     // Player position in the world
    Quaternion rotation;  // Player rotation
    Vector3 scale;        // Scale of the model

    // Movement variables
    float pitch;
    float yaw;
    float roll;
    float altitude;
    float speed;

    // Update the model's transform matrix
    void UpdateTransform();

    // Update rotation based on pitch, yaw, roll
    void UpdateRotation();
};

#endif // PLANE_H
