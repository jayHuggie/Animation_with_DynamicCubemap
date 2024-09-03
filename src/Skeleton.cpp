#include "Skeleton.h"

// Constructor: Initializes the skeleton and loads its data from the specified file.
/* Skeleton::Skeleton(std::string _fileName, const glm::mat4& _toWorld)
{
	fileName = _fileName;
	toWorld = _toWorld;
	load();
}
*/

Skeleton::Skeleton(std::string _fileName, GLuint _shaderID)
{
	fileName = _fileName;
	toWorld = glm::mat4(1.0f);
	shaderID = _shaderID;
	load();
}

// Destructor: Deletes the root joint, which recursively deletes all child joints.
Skeleton::~Skeleton()
{
	delete jointRoot;
}

// Loads the skeleton data from the file specified in the constructor.
void Skeleton::load()
{
	std::cout << "Loading " + fileName << std::endl;
	Tokenizer* fileToken = new Tokenizer();
	fileToken->Open(fileName.c_str());

	char tokenBuff[20];
	fileToken->GetToken(tokenBuff);

	// Checks if the first token in the file indicates the start of a joint hierarchy.
	if (strcmp(tokenBuff, "balljoint") == 0)
	{
		jointRoot = new BallJoint();
		//jointRoot->load(fileToken);
		jointRoot->load(fileToken, joints);
	}

	fileToken->Close();
	delete fileToken;

	// Update the joint hierarchy to the initial pose.
	jointRoot->update(toWorld);
}


// Updates the skeleton's transformation matrix and propagates the update through the joint hierarchy.
void Skeleton::update(const glm::mat4& input_toWorld)
{
	toWorld = input_toWorld;
	jointRoot->update(toWorld);
}

// Draws the skeleton (starting from the root joint) using the provided view projection matrix and shader.
/* 
void Skeleton::draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	jointRoot->draw(viewProjMtx * toWorld, shader);
}
*/
void Skeleton::draw(const glm::mat4& viewProjMtx)
{
	jointRoot->draw(viewProjMtx * toWorld, shaderID);
}


// Project2

glm::mat4 Skeleton::getWorldMat(int jointNum)
{
	return joints[jointNum]->getWorldMat();
}

std::string Skeleton::getJointName(int jointNum)
{
	return joints[jointNum]->getJointName();
}

void Skeleton::getJointRot(int jointNum, glm::vec3& rot)
{
	glm::vec3 currRot = joints[jointNum]->getRot();
	rot.x = currRot.x;
	rot.y = currRot.y;
	rot.z = currRot.z;
}

void Skeleton::setJointRot(int jointNum, glm::vec3& rot)
{
	joints[jointNum]->setRot(glm::vec3(rot[0], rot[1], rot[2]));
}

void Skeleton::getJointRotMin(int jointNum, glm::vec3& rotMin)
{
	glm::vec3 currRotMin = joints[jointNum]->getRotMin();
	rotMin.x = currRotMin.x;
	rotMin.y = currRotMin.y;
	rotMin.z = currRotMin.z;
}

void Skeleton::getJointRotMax(int jointNum, glm::vec3& rotMax)
{
	glm::vec3 currRotMax = joints[jointNum]->getRotMax();
	rotMax.x = currRotMax.x;
	rotMax.y = currRotMax.y;
	rotMax.z = currRotMax.z;
}