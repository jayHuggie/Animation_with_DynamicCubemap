#pragma once

#include "Animation.h"
#include "Skeleton.h"
#include <ctime> // Include for clock_t and clock() to measure time

class Character
{
private:
    Skeleton* skel; // Pointer to the skeleton of the character
    Animation* anim; // Pointer to the animation applied to the character

    clock_t prevT; // Stores the previous time snapshot
    float time; // Accumulated time to control the animation

	clock_t interval = 0;

    int FPSCount = 0;
	int FPS = 0;
    
    std::vector<BallJoint*> joints; // Optional: Stores joints for more specific control or access

public:
    // Constructor: Initializes a character with a given skeleton and animation
    Character(Skeleton* _skel, Animation* _anim);

    // Update function to progress the animation based on elapsed time
    void update();

    int GetFPS() { return FPS; }
};