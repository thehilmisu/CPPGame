#ifndef PLAYER_H
#define PLAYER_H

#include "Plane.h"

class Player : public Plane
{
public:
    // Constructor and Destructor
    explicit Player(const std::string& modelPath, const std::string& texturePath, Vector3 startPosition);
    virtual ~Player();

    // Update and Draw methods
    virtual void Update(float deltaTime) override;
    void Draw();

    // Input handling
    void HandleInput(float deltaTime);

    // Movement methods
    virtual void Move(const Vector3& direction, float speed, float deltaTime) override;
    virtual void Rotate(const Vector3& axis, float angleDegrees) override;


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

#endif // PLAYER_H
