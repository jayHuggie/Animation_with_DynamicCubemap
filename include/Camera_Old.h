////////////////////////////////////////
// Camera.h
////////////////////////////////////////

#pragma once

#include "core.h"
#include "glm/gtx/euler_angles.hpp"

// The Camera class provides a simple means to controlling the 3D camera. It could
// be extended to support more interactive controls. Ultimately. the camera sets the
// GL projection and viewing matrices.

class Camera {
public:
    Camera();

    void Update();
    void Reset();

    
    // Setter methods
    void SetAspect(float aspect);
    void SetDistance(float distance);
    void SetAzimuth(float azimuth);
    void SetIncline(float incline);

    // Getter methods
    float GetDistance() const;
    float GetAzimuth() const;
    float GetIncline() const;

    glm::mat4 GetViewMatrix() const; // New method
    glm::mat4 GetProjectionMatrix() const; // New method
    glm::mat4 GetViewProjectionMatrix() const; // This combines both view and projection matrices
    glm::vec3 GetPosition() const;

    void PanHorizontal(float amount);
    void PanVertical(float amount);

private:
    // Camera settings
    float FOV;        // Field of View (in degrees)
    float Aspect;     // Aspect ratio
    float NearClip;   // Near clipping distance
    float FarClip;    // Far clipping distance

    // Camera position parameters
    float Distance;   // Distance from camera to its target point
    float Azimuth;    // Rotation around the Y axis
    float Incline;    // Angle above the ground plane

    glm::vec3 Position; // Camera position in world space
    glm::mat4 ViewMatrix; // Cached view matrix
    glm::mat4 ProjectionMatrix; // Cached projection matrix

    void UpdatePosition();
};
