#include "Animation.h"

// Constructor: Loads animation data from a file.
Animation::Animation(std::string _fileName)
{
    fileName = _fileName;
    load(); // Load the animation data immediately upon creation.
}

// Destructor: Cleans up dynamically allocated channel objects.
Animation::~Animation()
{
    for (auto& it : channels)
    {
        delete it; // Delete each channel pointer to prevent memory leaks.
    }
}

// Load animation data from file.
void Animation::load()
{
    std::cout << "Loading " + fileName << std::endl; // Debug print to indicate loading.

    Tokenizer* fileToken = new Tokenizer();
    fileToken->Open(fileName.c_str()); // Open the animation file for tokenization.

    char tokenBuff[20];
    fileToken->GetToken(tokenBuff);
    if (strcmp(tokenBuff, "animation") == 0) // Confirm that the file specifies an animation.
    {
        fileToken->GetToken(tokenBuff); // Skip "{" token.
        
        // Parse the animation range (start and end times).
        fileToken->GetToken(tokenBuff);
        if (strcmp(tokenBuff, "range") == 0)
        {
            time_start = fileToken->GetFloat();
            time_end = fileToken->GetFloat();
            
            int numOfChannel = 0;
            fileToken->GetToken(tokenBuff);
            if (strcmp(tokenBuff, "numchannels") == 0)
            {
                numOfChannel = fileToken->GetInt(); // Read the number of channels.
            }
            
            // Load each channel.
			while (numOfChannel > 0)
            {
                fileToken->GetToken(tokenBuff);
                if (strcmp(tokenBuff, "channel") != 0)
                    continue; // Skip any tokens until "channel" is found.
                
                Channel* currChannel = new Channel(fileToken);
                channels.push_back(currChannel); // Add the new channel to the list.
    			numOfChannel--;
			}
        }
    }

    fileToken->Close(); // Close the file and clean up.
    delete fileToken;
}

// Update the animation state based on the current time 't'.
void Animation::update(float& t, glm::mat4& T, Skeleton* skel)
{
    // Clamp 't' to the animation range to prevent out-of-bounds behavior.
    if (t < time_start || t > time_end) t = time_start;

    // Evaluate position from the first three channels (assuming they are for position).
    glm::vec3 pos;
    pos.x = channels[0]->evaluate(t);
    pos.y = channels[1]->evaluate(t);
    pos.z = channels[2]->evaluate(t);
    T = glm::translate(pos); // Apply translation to the transformation matrix 'T'.

    // Evaluate and set rotations for each joint in the skeleton.
    int jointsNum = skel->getNumOfJoint();
    for (int i = 0; i < jointsNum; i++)
    {
        glm::vec3 rot;
        rot.x = channels[3 * i + 3]->evaluate(t);
        rot.y = channels[3 * i + 4]->evaluate(t);
        rot.z = channels[3 * i + 5]->evaluate(t);
        skel->setJointRot(i, rot); // Set the rotation for joint 'i'.
    }
}