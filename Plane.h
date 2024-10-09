#ifndef PLANE_H
#define PLANE_H

#include "raylib.h"
#include "raymath.h"
#include <string>

struct FlightInfo {
    float pitch;
    float yaw;
    float roll;
    float altitude;
    float speed;
};

class Plane
{
public:
    // Constructor and Destructor
    Plane(Vector3 startPosition);
    ~Plane();

    // Update and Draw methods
    virtual void Update(float deltaTime);
    virtual void Draw();

    // Position, Rotation, and Scale setters and getters
    void SetPosition(const Vector3& position);
    Vector3 GetPosition();

    void SetRotation(const Quaternion& rotation);
    Quaternion GetRotation() const;

    void SetScale(const float& scale);
    float GetScale() const;

    void SetFlipped(bool flipped);
    bool GetFlipped();

    FlightInfo GetFlightInfo();
    Model GetModel();

    // Movement methods
    void Move(const Vector3& direction, float speed, float deltaTime);
    void Rotate(const Vector3& axis, float angleDegrees);

    // LookAt method (if needed)
    void LookAt(const Vector3& targetPosition);

    // Update rotation based on pitch, yaw, roll
    void UpdateRotation();

    // Handling the user input
    void HandleInput(float deltaTime);

private:
    Vector3 position;     // Player position in the world
    Quaternion rotation;  // Player rotation
    float scale;          // Scale of the model
    bool isFlipped;

    // Movement variables
    FlightInfo flightInfo;
};

#endif // PLANE_H