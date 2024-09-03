#pragma once

#include "core.h"

#include "Tokenizer.h"

class Keyframe
{
private:
    float time; // The timestamp of the keyframe.
    float value; // The value of the animation property at this keyframe.
    float tangent[2] = {0}; // Tangents for interpolation. [0] is in-tangent, [1] is out-tangent.
    int rule[2]; // Interpolation rules for each tangent. Uses constants defined below.

    // Coefficients for cubic interpolation.
    float A, B, C, D;
    float deltaT; // Time difference to the next keyframe.

public:
    // Constants defining tangent rules.
    static const int FLAT, LINEAR, SMOOTH, FIXED, NOT_DEFINE;

    Keyframe(Tokenizer* fileToken); // Constructor that initializes keyframe from tokenized data.
    ~Keyframe(); // Destructor.

    // Initializes tangents based on neighboring keyframes.
    void iniTangent(Keyframe* prevFrame, Keyframe* nextFrame);
    // Calculates cubic interpolation coefficients with the next keyframe.
    void iniCubic(Keyframe* nextFrame);

    // Accessors.
    float getTime() { return time; }
    float getValue() { return value; }
    float getTangentIn() { return tangent[0]; }
    float getTangentOut() { return tangent[1]; }

    // Evaluates the value of the animation property at a given time using interpolation.
    float evaluate(float t);
};