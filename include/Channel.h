#pragma once

#include "Keyframe.h"

class Channel
{
private:
    int extrap[2]; // Extrapolation rules for before the first and after the last keyframes.
    int index; // Index of the channel, possibly used for identifying the channel.
    float time_start, time_end; // Start and end times of the channel's animation.
    float tangent_start, tangent_end; // Tangents for extrapolation at the start and end.
    float period; // Time duration of the animation cycle.
    float offset; // Value offset for cycle_offset extrapolation.

    std::vector<Keyframe*> keyframes; // Container for all keyframes in the channel.

public:
    // Constants defining extrapolation behavior.
    static const int CONSTANT, LINEAR, CYCLE, CYCLE_OFFSET, BOUNCE, NOT_DEFINE;

    Channel(Tokenizer* fileToken); // Constructor that initializes the channel from tokenized data.
    ~Channel(); // Destructor to clean up keyframes.

    void load(Tokenizer* fileToken); // Loads channel data, including keyframes, from a file.
    void precompute(); // Precomputes necessary data for interpolation between keyframes.
    float evaluate(float t); // Evaluates the channel's value at a given time 't'.
};