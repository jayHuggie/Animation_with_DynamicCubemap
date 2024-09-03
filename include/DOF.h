#pragma once

#include "core.h"

class DOF
{
private:
	float value = 0;
	float min = -360;
	float max = 260;

public:
	// Default constructor for the DOF class.
	DOF() {}

	// Parameterized constructor for the DOF class.
	// This constructor initializes a DOF object with specified min and max values.
	DOF(float min_input, float max_input)
		:min(min_input), max(max_input) {}
	
	// Destructor for the DOF class.
	~DOF() {}

	void setValue(float value_input)
	{
		value = value_input;

		// To ensure the value is inside min and max
		value = std::max(min, value);
		value = std::min(max, value);
	}


	// Project 2
	float getValue() { return value; }
	void setMin(float min_input) { min = min_input; }
	void setMax(float max_input) { max = max_input; }

	float getMin() {
		return min;
	}
	float getMax() {
		return max;
	}
};