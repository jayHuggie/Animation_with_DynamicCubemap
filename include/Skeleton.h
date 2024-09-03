#pragma once

#include "Joint.h"

class Skeleton
{
private:
	// Root joint of the skeleton hierarchy.
	BallJoint* jointRoot;

	// File name of the skeleton data.
	std::string fileName;

	// Transformation matrix that represents the skeleton's position and orientation in the world.
	glm::mat4 toWorld;

	// Project 2
	std::vector<BallJoint*> joints;

	GLuint shaderID;

public:
	// Constructor and Destructor.
	//Project 1
	//Skeleton(std::string _fileName, const glm::mat4& _toWorld);
	
	Skeleton(std::string _fileName, GLuint _shaderID);
	~Skeleton();

	// Loads the skeleton data from a file.
	void load();

	// Updates the skeleton's pose based on the given world transformation.
	void update(const glm::mat4& _toWorld);

	// Draws the skeleton using the given view projection matrix and shader.
	//void draw(const glm::mat4& viewProjMtx, GLuint shader);

	void draw(const glm::mat4& viewProjMtx);
	// Project 2
	glm::mat4 getWorldMat(int jointNum);

	size_t getNumOfJoint() { return joints.size(); }

	std::string getJointName(int jointNum);
	void getJointRot(int jointNum, glm::vec3& rot);
	void getJointRotMin(int jointNum, glm::vec3& rotMin);
	void getJointRotMax(int jointNum, glm::vec3& rotMax);
	void setJointRot(int jointNum, glm::vec3& rot);
};