#include "Keyframe.h"

// Initialize constants for interpolation rules.
const int Keyframe::NOT_DEFINE = 0;
const int Keyframe::FLAT = 1;
const int Keyframe::LINEAR = 2;
const int Keyframe::SMOOTH = 3;
const int Keyframe::FIXED = 4;

// Constructor that loads keyframe data from a tokenizer.
Keyframe::Keyframe(Tokenizer* fileToken)
{
    time = fileToken->GetFloat(); // Read the keyframe time.
    value = fileToken->GetFloat(); // Read the keyframe value.

    // Read and interpret tangent rules for in and out tangents.
	for (int i = 0; i < 2; i++)
	{
		char tokenBuff[20];
		fileToken->GetToken(tokenBuff);
		if (strcmp(tokenBuff, "flat") == 0)
		{
			rule[i] = FLAT;
			tangent[i] = 0.0f;
		}
		else if (strcmp(tokenBuff, "linear") == 0)
		{
			rule[i] = LINEAR;
		}
		else if (strcmp(tokenBuff, "smooth") == 0)
		{
			rule[i] = SMOOTH;
		}
		else
		{
			try
			{
				tangent[i] = std::stof(tokenBuff);
				rule[i] = FIXED;
			}
			catch (std::invalid_argument)
			{
				rule[i] = NOT_DEFINE;
				std::cout << "Invalid argument for rule" << std::endl;
			}
		}

	}
}

// Destructor.
Keyframe::~Keyframe()
{
	// No dynamic memory to free (but defined for completeness!)
}

// Initializes tangents based on the rules and the values of adjacent keyframes.
void Keyframe::iniTangent(Keyframe* prevFrame, Keyframe* nextFrame)
{
	if (prevFrame == nullptr && nextFrame == nullptr)
	{
		if (rule[0] == LINEAR || rule[0] == SMOOTH)
			tangent[0] = 0;

		if (rule[1] == LINEAR || rule[1] == SMOOTH)
			tangent[1] = 0;
		
	}
	else if (prevFrame == nullptr)
	{
		if (rule[1] == LINEAR || rule[1] == SMOOTH)
		{
			tangent[1] = (nextFrame->getValue() - value)
				/ (nextFrame->getTime() - time);
		}
	}
	else if (nextFrame == nullptr)
	{
		if (rule[0] == LINEAR || rule[0] == SMOOTH)
		{
			tangent[0] = (value - prevFrame->getValue())
				/ (time - prevFrame->getTime());
		}
	}
	else
	{
		if (rule[0] == LINEAR)
		{
			tangent[0] = (value - prevFrame->getValue())
				/ (time - prevFrame->getTime());
		}
		else if (rule[0] == SMOOTH)
		{
			tangent[0] = (nextFrame->getValue() - prevFrame->getValue())
				/ (nextFrame->getTime() - prevFrame->getTime());
			tangent[1] = tangent[0];
		}

		if (rule[1] == LINEAR)
		{
			tangent[1] = (nextFrame->getValue() - value)
				/ (nextFrame->getTime() - time);
		}
	}
}

// Calculates the coefficients for cubic Hermite interpolation with the next keyframe.
void Keyframe::iniCubic(Keyframe* nextFrame)
{
	if (nextFrame != nullptr)
	{
		deltaT = nextFrame->getTime() - time;

		A = 2 * value - 2 * nextFrame->getValue()
			+ tangent[1] * deltaT + nextFrame->getTangentIn() * deltaT;
		B = -3 * value + 3 * nextFrame->getValue()
			- 2 * tangent[1] * deltaT - nextFrame->getTangentIn() * deltaT;
		C = tangent[1] * deltaT;
		D = value;
	}
	else
	{
		deltaT = 1.0f;
		A = 0.0f;
		B = 0.0f;
		C = 0.0f;
		D = value;
	}
}

// Evaluates the keyframe's value at a given time using cubic interpolation.
float Keyframe::evaluate(float t)
{
	// Implementation of cubic Hermite interpolation to evaluate the animation value at time 't'.
    // 'u' is the normalized time between the current and the next keyframe.
    // Returns the interpolated value.
	float u = (t - time) / deltaT;
	return D + u * (C + u * (B + u * A));
}