#pragma once

#include "core.h"
#include "glm/gtx/euler_angles.hpp"

// Defines several possible options for camera movement.
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed = 100.0f;
    float MouseSensitivity;
    float Zoom = ZOOM;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(-3.8f, 3.0f, 1.5f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();

    glm::vec3 GetPosition() const;

    // Processes input received from any keyboard-like input system.
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    // Processes input received from a mouse input system.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // Processes input received from a mouse scroll-wheel event.
    void ProcessMouseScroll(float yoffset);

        void Reset() {
            Position = glm::vec3(-3.8f, 3.0f, 1.5f); // Reset position
            WorldUp = glm::vec3(0.0f, 1.0f, 0.0f); // Reset world up vector
            Yaw = YAW; // Reset yaw
            Pitch = PITCH; // Reset pitch
            MovementSpeed = SPEED; // Reset movement speed to default
            MouseSensitivity = SENSITIVITY; // Reset mouse sensitivity to default
            Zoom = ZOOM; // Reset zoom (field of view) to default

            updateCameraVectors();
    }

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};
