#include "Character.h"

// Constructor: Initializes the character with skeleton and animation, and resets time
Character::Character(Skeleton* _skel, Animation* _anim)
{
    skel = _skel; // Assign the skeleton
    anim = _anim; // Assign the animation
    time = 0.0f; // Start time at zero

    prevT = clock(); // Initialize prevT with the current clock time
}

// Update function to advance the character's animation based on elapsed time
void Character::update()
{
    clock_t currT = clock(); // Get current clock time
    
    FPSCount++; // Increment the frame-per-second count.
    interval += currT - prevT; // Accumulate the time since the last frame.
    // Update the FPS counter once every second.
    if (interval >= 1000)
    {
        FPS = FPSCount; // Set the FPS value.
        interval = 0; // Reset the time interval.
        FPSCount = 0; // Reset the frame count.
    }
    
    // Calculate elapsed time since last update, convert from clock ticks to seconds
    time += ((float)currT - (float)prevT) / CLOCKS_PER_SEC;
    prevT = currT; // Update prevT for the next call

    glm::mat4 T; // Transformation matrix

    // Update the animation state based on the accumulated time
    anim->update(time, T, skel);

    // Update the skeleton with the new transformation matrix (if used)
    skel->update(T);
}