#include "Plane.h"
#include "GameSettings.h"
#include <ResourceManager.h>

Plane::Plane(Vector3 startPosition)
    : position(startPosition), rotation(QuaternionIdentity()), scale(1.0f), isFlipped(true),
      flightInfo{0.0f, 0.0f, 0.0f, startPosition.y, 5.0f}
{
    // Initialize the transform
    UpdateRotation();
}

Plane::~Plane()
{
}

void Plane::SetPosition(const Vector3& newPosition)
{
    position = newPosition;
    UpdateRotation();
}

Vector3 Plane::GetPosition()
{
    return position;
}

void Plane::SetScale(const float& newScale)
{
    scale = newScale;
    UpdateRotation();
}

float Plane::GetScale() const
{
    return scale;
}

void Plane::SetFlipped(bool flipped)
{
    isFlipped = flipped;
    UpdateRotation();
}

FlightInfo Plane::GetFlightInfo()
{
    return flightInfo;
}

void Plane::HandleInput(float deltaTime)
{
    // Adjust pitch (X-axis rotation)
    if (IsKeyDown(KEY_UP)) {
        flightInfo.pitch -= ROLL_PITCH_CONSTANT * deltaTime;
        if(flightInfo.pitch <= MIN_ROLL_PITCH_VALUE)
            flightInfo.pitch = MIN_ROLL_PITCH_VALUE;
        flightInfo.altitude += flightInfo.speed * deltaTime;
    }
    else if (IsKeyDown(KEY_DOWN)) {
        flightInfo.pitch += ROLL_PITCH_CONSTANT * deltaTime;
        if(flightInfo.pitch >= MAX_ROLL_PITCH_VALUE)
            flightInfo.pitch = MAX_ROLL_PITCH_VALUE;
        flightInfo.altitude -= flightInfo.speed * deltaTime;
    }
    else
    {
        if (flightInfo.pitch > 3.0f) flightInfo.pitch -= ROLL_PITCH_CONSTANT * deltaTime;
        else if (flightInfo.pitch < -3.0f) flightInfo.pitch += ROLL_PITCH_CONSTANT * deltaTime;
    }


    // Adjust roll (Z-axis rotation) and horizontal movement
    float turningValue = 0.0f;
    if (IsKeyDown(KEY_LEFT)) { 
        flightInfo.roll -= ROLL_PITCH_CONSTANT * deltaTime; 
        if(flightInfo.roll <= MIN_ROLL_PITCH_VALUE)
            flightInfo.roll = MIN_ROLL_PITCH_VALUE;
        turningValue -= flightInfo.speed * deltaTime; 
    }
    else if (IsKeyDown(KEY_RIGHT)) { 
        flightInfo.roll += ROLL_PITCH_CONSTANT * deltaTime;
        if(flightInfo.roll >= MAX_ROLL_PITCH_VALUE)
            flightInfo.roll = MAX_ROLL_PITCH_VALUE; 
        turningValue += flightInfo.speed * deltaTime; 
    }
    else
    {
        if (flightInfo.roll > 6.0f * deltaTime) flightInfo.roll -= ROLL_PITCH_CONSTANT * deltaTime;
        else if (flightInfo.roll < -6.0f * deltaTime) flightInfo.roll += ROLL_PITCH_CONSTANT * deltaTime;
    }

    // Update position based on turning value
    position.x += turningValue;

    // Update altitude
    position.y = flightInfo.altitude;

    // Update the player's rotation based on pitch, yaw, and roll
    UpdateRotation();
}

void Plane::Update(float deltaTime)
{
    // Handle input and update plane state
    HandleInput(deltaTime);
}

void Plane::UpdateRotation()
{
    // Convert pitch, yaw, and roll from degrees to radians
    float pitchRad = DEG2RAD * flightInfo.pitch;
    float yawRad = DEG2RAD * flightInfo.yaw;
    float rollRad = DEG2RAD * flightInfo.roll;

    // Create rotation matrices for pitch, yaw, and roll
    Matrix pitchMat = MatrixRotateX(pitchRad);
    Matrix yawMat = MatrixRotateY(yawRad);
    Matrix rollMat = MatrixRotateZ(rollRad);

    // Combine rotations: roll * pitch * yaw
    Matrix rotationMat = MatrixMultiply(rollMat, MatrixMultiply(pitchMat, yawMat));

    if(isFlipped) {
        Matrix flipMat = MatrixRotateY(PI); // Rotate 180 degrees around Y-axis
        rotationMat = MatrixMultiply(rotationMat, flipMat);
    }

    // Create scaling matrix
    Matrix scaleMat = MatrixScale(scale, scale, scale);

    // Combine scaling and rotation
    Matrix transformMat = MatrixMultiply(scaleMat, rotationMat);

    // Create translation matrix
    Matrix translationMat = MatrixTranslate(position.x, position.y, position.z);

    // Combine all transformations: translation * rotation * scaling
    ResourceManager::GetPlayerModel().transform = MatrixMultiply(transformMat, translationMat);
}

void Plane::Draw()
{
    // Draw the model at the origin; transformations are applied via model.transform
    DrawModel(ResourceManager::GetPlayerModel(), { 0.0f, 0.0f, 0.0f }, 1.0f, WHITE);
}

void Plane::Move(const Vector3& direction, float speed, float deltaTime)
{
    Vector3 movement = Vector3Scale(direction, speed * deltaTime);
    position = Vector3Add(position, movement);
    UpdateRotation();
}
