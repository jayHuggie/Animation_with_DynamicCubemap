#include "Joint.h"

// Constructor: Initializes degrees of freedom (DOF) for rotation.
BallJoint::BallJoint()
{
	rotX = new DOF();
	rotY = new DOF();
	rotZ = new DOF();
}

// Destructor: Deletes all child joints recursively.
BallJoint::~BallJoint()
{
	for (BallJoint* child : children)
	{
		child->~BallJoint();
		delete child;
	}
}

// Loads joint data from a tokenized file
//void BallJoint::load(Tokenizer* fileToken)
void BallJoint::load(Tokenizer* fileToken, std::vector<BallJoint*>& joints)
{
	joints.push_back(this);
	char tokenBuff[20];

	// Get the joint name.
	fileToken->GetToken(tokenBuff);
	jointName = tokenBuff;
	fileToken->GetToken(tokenBuff);

	while (1)
	{
		fileToken->GetToken(tokenBuff);

		// Set the joint's offset.
		if (strcmp(tokenBuff, "offset") == 0)
		{
			offset.x = fileToken->GetFloat();
			offset.y = fileToken->GetFloat();
			offset.z = fileToken->GetFloat();
		}

		// Set the minimum bounding box for joint rotation.
		else if (strcmp(tokenBuff, "boxmin") == 0)
		{
			boxmin.x = fileToken->GetFloat();
			boxmin.y = fileToken->GetFloat();
			boxmin.z = fileToken->GetFloat();
		}

		// Set the maximum bounding box for joint rotation.
		else if (strcmp(tokenBuff, "boxmax") == 0)
		{
			boxmax.x = fileToken->GetFloat();
			boxmax.y = fileToken->GetFloat();
			boxmax.z = fileToken->GetFloat();
		}

		// Set the limits for rotation around the X-axis.
		else if (strcmp(tokenBuff, "rotxlimit") == 0)
		{
			rotX->setMin(fileToken->GetFloat());
			rotX->setMax(fileToken->GetFloat());
		}

		// Set the limits for rotation around the Y-axis.
		else if (strcmp(tokenBuff, "rotylimit") == 0)
		{
			rotY->setMin(fileToken->GetFloat());
			rotY->setMax(fileToken->GetFloat());
		}

		// Set the limits for rotation around the Z-axis.
		else if (strcmp(tokenBuff, "rotzlimit") == 0)
		{
			rotZ->setMin(fileToken->GetFloat());
			rotZ->setMax(fileToken->GetFloat());
		}

		// Set the initial pose for the joint.
		else if (strcmp(tokenBuff, "pose") == 0)
		{
			rotX->setValue(fileToken->GetFloat());
			rotY->setValue(fileToken->GetFloat());
			rotZ->setValue(fileToken->GetFloat());
		}

		// Create and load data for a child joint.
		else if (strcmp(tokenBuff, "balljoint") == 0)
		{
			children.push_back(new BallJoint());
			children.back()->setParent(this);
			children.back()->load(fileToken, joints);
		}

		// End of the joint definition.
		else if (strcmp(tokenBuff, "}") == 0)
		{
			break;
		}
	}

	cube = new Cube(boxmin, boxmax);
}

// Updates the joint's transformation matrices and applies the changes to children.
void BallJoint::update(const glm::mat4& parent2World)
{
	// Compute local transformation matrix using Euler angles and offset.
	loc2Parent = glm::eulerAngleX(rotX->getValue());
	loc2Parent = glm::eulerAngleY(rotY->getValue()) * loc2Parent;
	loc2Parent = glm::eulerAngleZ(rotZ->getValue()) * loc2Parent;
	loc2Parent = glm::translate(offset) * loc2Parent;

	// Compute the world transformation matrix.
	loc2World = parent2World * loc2Parent;

	// Update all children joints.
	for (BallJoint* child : children)
	{
		child->update(loc2World);
	}
}

// Draws the joint and its children.
void BallJoint::draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	// Draw the visual representation of this joint.
	cube->draw(viewProjMtx * loc2World, shader);

	// Draw all children joints.
	for (BallJoint* child : children)
	{
		child->draw(viewProjMtx, shader);
	}
}

// Adds a child joint to this joint.
void BallJoint::addChild(BallJoint* child)
{
	children.push_back(child);
	child->setParent(this);
}

// Project 2

// Returns the current rotation of the joint in Euler angles.
glm::vec3 BallJoint::getRot()
{
    // Each component of the vector represents the rotation around X, Y, and Z axes.
	return glm::vec3(rotX->getValue(),
		rotY->getValue(),
		rotZ->getValue());
}

// Sets the rotation of the joint using Euler angles.
void BallJoint::setRot(glm::vec3& rot)
{
    // Each component of 'rot' sets the rotation around the X, Y, and Z axes respectively.
	rotX->setValue(rot.x);
	rotY->setValue(rot.y);
	rotZ->setValue(rot.z);
}

// Returns the minimum rotational limits of the joint as a vector of Euler angles.
glm::vec3 BallJoint::getRotMin()
{
    // Each component of the returned vector represents the minimum rotation around X, Y, and Z axes.
	return glm::vec3(rotX->getMin(),
		rotY->getMin(),
		rotZ->getMin());
}

// Returns the maximum rotational limits of the joint as a vector of Euler angles.
glm::vec3 BallJoint::getRotMax()
{
    // Each component of the returned vector represents the maximum rotation around X, Y, and Z axes.
	return glm::vec3(rotX->getMax(),
		rotY->getMax(),
		rotZ->getMax());
}