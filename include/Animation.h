#pragma once

#include "channel.h"
#include "skeleton.h"

class Animation
{
private:
    std::string fileName; // Name of the file from which the animation is loaded.
    float time_start, time_end; // Start and end times of the animation.

    std::vector<Channel*> channels; // Animation channels that hold keyframed data.

public:
    Animation(std::string _filename); // Constructor that initializes the animation with a file name.
    ~Animation(); // Destructor to clean up allocated channels.

    void load(); // Loads the animation data from the specified file.
    void update(float& t, glm::mat4& T, Skeleton* skel); // Updates the skeleton based on the animation time 't'.

};