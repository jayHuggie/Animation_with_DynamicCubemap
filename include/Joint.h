#pragma once

#include "DOF.h"
#include "Tokenizer.h"
#include "Cube.h"

// Project 1

class BallJoint
{
private:
	// The positional offset of this joint from its parent.
	glm::vec3 offset;

	// The maximum and minimum bounds of the joint's rotation box.
	glm::vec3 boxmax, boxmin;

	// Transformation matrix from this joint's local space to its parent's space.
	glm::mat4 loc2Parent = glm::mat4(1.0);

	// Transformation matrix from this joint's local space to the world space.
	glm::mat4 loc2World = glm::mat4(1.0);

	// Degrees of Freedom for rotation around the X, Y, and Z axes.
	DOF* rotX, * rotY, * rotZ;

	// A list of child joints that are attached to this joint.
	std::vector<BallJoint*> children;

	// Pointer to the parent joint. It is NULL if this joint is the root joint.
	BallJoint* parent = NULL;

	// The name of the joint for identification.
	std::string jointName;

	Cube* cube;

public:
	// Constructor and destructor.
	BallJoint();
	~BallJoint();

	// Function to load joint data from a tokenized file.
	//void load(Tokenizer* fileToken);
	void load(Tokenizer* fileToken, std::vector<BallJoint*>& joints);


	// Updates the joint's transformation matrices based on the parent's world transform.
	void update(const glm::mat4& toWorld);

	// Renders the joint
	void draw(const glm::mat4& viewProjMtx, GLuint shader);

	// Adds a child joint to this joint.
	void addChild(BallJoint* child);

	// Sets the parent of this joint.
	void setParent(BallJoint* input_parent) { parent = input_parent; }

	// Project 2

	// Returns the transformation matrix of the joint in world coordinates.
	glm::mat4 getWorldMat() { return loc2World; }

	// Returns the name of the joint.
	std::string getJointName() { return jointName; }

	// Retrieves the current rotation of the joint in Euler angles (X, Y, Z).
	glm::vec3 getRot();

	// Sets the rotation of the joint using Euler angles (X, Y, Z).
	void setRot(glm::vec3& rot);

	// Gets the minimum rotation limits of the joint in Euler angles (X, Y, Z).
	glm::vec3 getRotMin();

	// Gets the maximum rotation limits of the joint in Euler angles (X, Y, Z).
	glm::vec3 getRotMax();
};