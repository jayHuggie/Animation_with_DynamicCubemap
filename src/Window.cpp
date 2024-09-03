#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Window.h"
#include "Sphere.h"

static float nearPlane = 0.1f;
static float farPlane = 100.0f;

GLuint loadSimpleTexture() {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("Texture/mars.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return textureID;
}



unsigned int Window::skyboxVAO = 0;
unsigned int Window::skyboxVBO = 0;

unsigned int sphereVAO, sphereVBO, sphereEBO;
std::vector<float> sphereVertices;
std::vector<unsigned int> sphereIndices;
int latitudeCount = 20; // Increase for higher quality
int longitudeCount = 20; // Increase for higher quality

unsigned int groundVAO, groundVBO;//////
//std::vector<float> groundVertices;

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Animation with Dynamic Cubemap";

// Objects to render
//Cube* Window::cube;

// Project 1
Skeleton* Window::skel;
Skeleton* Window::skel2;

// Project 2
Skin* Window::skin = nullptr;
Skin* Window::skin2 = nullptr;
bool Window::drawSkelFlag = false;

// Project 3
Character* Window::character = nullptr;

// Camera Properties
Camera* Cam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;
GLuint Window::skinShaderProgram;
GLuint Window::skyboxShaderProgram;
GLuint Window::SphereShaderProgram;
GLuint Window::boxShaderProgram;

GLuint Window::fbo;
GLuint Window::fboTexture;
GLuint Window::depthRBO;
GLuint Window::dynamicCubeMapFBO;
GLuint Window::dynamicCubeMapTextureID;
int cubeMapResolution = 2000; // Example resolution

GLuint Window::quadVAO;
GLuint Window::quadVBO;
GLuint Window::quadShaderProgram;
GLuint Window::simpleTextureID;




// Project 1 Default Skel
//std::string Window::skelFileName = "skelFile/test.skel";

// Project 1
std::string Window::skelFileName = "";
std::string Window::skelFileName2 = "skelFile/crypto.skel";

// Project 2
std::string Window::skinFileName = "";
std::string Window::skinFileName2 = "skinFile/crypto.skin";
// Project 3
std::string Window::animFileName = "";

GLuint Window::cubemapTexture; // Define cubemapTexture

//gui
std::vector<std::string> Window::jointsName;
std::vector<glm::vec3> Window::jointsRot;
std::vector<glm::vec3> Window::jointsRotMin;
std::vector<glm::vec3> Window::jointsRotMax;


// Constructors and desctructors
bool Window::initializeProgram() {
    // Create a shader program with a vertex shader and a fragment shader.
    ///////shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    
    shaderProgram = LoadShaders("shaders/pbr.vert", "shaders/pbr.frag");

    SphereShaderProgram = LoadShaders("shaders/sphere.vert", "shaders/sphere.frag");


    // Project 5
    skyboxShaderProgram = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");

    quadShaderProgram = LoadShaders("shaders/quad.vert", "shaders/quad.frag");

    boxShaderProgram = LoadShaders("shaders/box.vert", "shaders/box.frag");

    
    // Check the shader program.
    if (!shaderProgram) {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

    if (!skyboxShaderProgram) {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }
    if (!boxShaderProgram) {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

	return true;

}

GLuint Window::LoadCubemap(std::vector<std::string> faces) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (GLuint i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}


GLenum targets[] = {
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
};

// TODO: Get resolution
GLuint createDynamicCubeMap(int resolution) {
    
    glGenFramebuffers(1, &Window::dynamicCubeMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, Window::dynamicCubeMapFBO);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (GLuint i = 0; i < 6; ++i) {
        glTexImage2D(targets[i], 0, GL_RGB, resolution, resolution, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    }

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



    
    glGenRenderbuffers(1, &Window::depthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, Window::depthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolution, resolution);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, Window::dynamicCubeMapFBO);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, targets[0], textureID, 0);

    return textureID;
}


bool Window::initializeObjects() {
    // Create a cube
    // cube = new Cube();
    // cube = new Cube(glm::vec3(-1, 0, -2), glm::vec3(1, 1, 1));
    std::vector<std::string> faces{
    "Cubemap/right.jpg",   // Right face
    "Cubemap/left.jpg",    // Left face
    "Cubemap/top.jpg",     // Top face
    "Cubemap/bottom.jpg",  // Bottom face
    "Cubemap/front.jpg",   // Front face
    "Cubemap/back.jpg"  
    };

Window::cubemapTexture = LoadCubemap(faces);


//GLuint dynamicCubeMapFBO;


Window::dynamicCubeMapTextureID = createDynamicCubeMap(cubeMapResolution);
// The FBO is created inside the createDynamicCubeMap function and should be stored globally or in a class member

float skyboxVertices[] = {
    // positions 
    // Back         
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    // Left
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    // Right
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    // Front
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    // Top
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    // Bottom
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

// Generate VAO and VBO for the skybox
glGenVertexArrays(1, &skyboxVAO);
glBindVertexArray(skyboxVAO);

glGenBuffers(1, &skyboxVBO);
glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

// Vertex attribute configuration
glEnableVertexAttribArray(0); // Assuming location 0 is set for position attribute in your vertex shader
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

glBindVertexArray(0);


///////// Making a sample sphere
generateSphereData(sphereVertices, sphereIndices, latitudeCount, longitudeCount);

   // Generate VAO, VBO, and EBO for the sphere
    glGenVertexArrays(1, &sphereVAO);
    glBindVertexArray(sphereVAO);

    glGenBuffers(1, &sphereVBO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), sphereVertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &sphereEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), sphereIndices.data(), GL_STATIC_DRAW);

// Position attribute
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// Normal attribute
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);

// Texture coordinate attribute
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
glEnableVertexAttribArray(2);

    // Unbind the VAO
    glBindVertexArray(0);


    // Project 1
    /*skel = new Skeleton(skelFileName, glm::mat4(1.0));

    return true;
    */

   // Project 2
   	if (!skelFileName.empty())
	{
		skel = new Skeleton(skelFileName, shaderProgram);

        // Project 3
        int numOfJoint = skel->getNumOfJoint();
		jointsRot.resize(numOfJoint);
		jointsRotMin.resize(numOfJoint);
		jointsRotMax.resize(numOfJoint);
		for (int i = 0; i < numOfJoint; i++)
		{
			std::string jointName = skel->getJointName(i);
			jointsName.push_back(jointName);
		}
	}
	
	if (!skinFileName.empty())
	{
        #ifdef SHADER_SKINING
                skin = new Skin(skinFileName, skel, skinShaderProgram);
        #else
                skin = new Skin(skinFileName, skel, shaderProgram);
                //skin2 = new Skin(skinFileName2, skel2, shaderProgram);
        #endif // SHADER_SKINING
    
    if (!animFileName.empty())
		{
			Animation* anim = new Animation(animFileName);
			character = new Character(skel, anim);
		}
	}

		skel2 = new Skeleton(skelFileName2, shaderProgram);

        // Project 3
        int numOfJoint = skel2->getNumOfJoint();
		jointsRot.resize(numOfJoint);
		jointsRotMin.resize(numOfJoint);
		jointsRotMax.resize(numOfJoint);
		for (int i = 0; i < numOfJoint; i++)
		{
			std::string jointName = skel2->getJointName(i);
			jointsName.push_back(jointName);
		}

        skin2 = new Skin(skinFileName2, skel2, shaderProgram);

    // Unbind the VAO to avoid accidentally modifying it
    glBindVertexArray(0); 

    float quadVertices[] = {
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};

    //GLuint quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);

    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // Texture coordinate attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);


Window::simpleTextureID = loadSimpleTexture();



float groundVertices[] = {
    // Back face
    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
     1.0f, -1.0f, -1.0f, 1.0f, 0.0f, // Bottom-right
     1.0f,  1.0f, -1.0f, 1.0f, 1.0f, // Top-right
     1.0f,  1.0f, -1.0f, 1.0f, 1.0f, // Top-right
    -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, // Top-left
    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
    
    // Front face
    -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, // Bottom-left
     1.0f, -1.0f,  1.0f, 1.0f, 0.0f, // Bottom-right
     1.0f,  1.0f,  1.0f, 1.0f, 1.0f, // Top-right
     1.0f,  1.0f,  1.0f, 1.0f, 1.0f, // Top-right
    -1.0f,  1.0f,  1.0f, 0.0f, 1.0f, // Top-left
    -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, // Bottom-left     

    
};

glGenVertexArrays(1, &groundVAO);
glGenBuffers(1, &groundVBO);

glBindVertexArray(groundVAO);
glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), &groundVertices, GL_STATIC_DRAW);


// Position attribute
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);




	return true;
}

void Window::cleanUp() {
    // Deallcoate the objects.
    // delete cube;
    delete skel;
    delete skin;

    // Delete the shader program.
    glDeleteProgram(shaderProgram);
    glDeleteProgram(skyboxShaderProgram);
}
void Window::resizeCallback(GLFWwindow* window, int width, int height) {
    // Query the actual framebuffer size.
    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    Window::width = framebufferWidth;
    Window::height = framebufferHeight;

    // Use framebuffer size for the viewport to handle high-DPI displays correctly.
    glViewport(0, 0, framebufferWidth, framebufferHeight);

    // Recalculate the projection matrix with the new window dimensions
    float aspectRatio = static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight);
    glm::mat4 projection = glm::perspective(glm::radians(Cam->Zoom), aspectRatio, nearPlane, farPlane);

    // Update the projection matrix for your shaders
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glUseProgram(SphereShaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(SphereShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glUseProgram(skyboxShaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(skyboxShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

}



/* 
void Window::resizeCallback(GLFWwindow* window, int width, int height) {
    Window::width = width;
    Window::height = height;
    // Set the viewport size.
    glViewport(0, 0, width, height);
}
*/
// for the Window
GLFWwindow* Window::createWindow(int width, int height) {
    // Initialize GLFW.
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }

    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

    // Check if the window could not be created.
    if (!window) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }

    // Make the context of the window.
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewInit();

    // Set swap interval to 1.
    glfwSwapInterval(0);

    // set up camera default position!!!!
    Cam = new Camera(glm::vec3(-3.8f, 3.0f, 1.5f), glm::vec3(0.0f, 1.0f, 0.0f));


    // initialize the interaction variables
    LeftDown = RightDown = false;
    MouseX = MouseY = 0;

    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);

    return window;
}


// update and draw functions
void Window::idleCallback() {

    //cube->update();

    // Project 1
    // skel->update(glm::mat4(1.0));

    // Project 3
    if (character != nullptr) character->update();

    // Project 2
    // ELSE IF !!!! for fixed camera!!!!!
    else if (skel != nullptr)skel->update(glm::mat4(1.0));
    //else if (skel2 != nullptr)skel2->update(glm::mat4(1.0));
	if (skin != nullptr)skin->update();
	//if (skin2 != nullptr)skin2->update();
}

void setCommonUniforms(GLuint program, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& viewPos) {
    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3fv(glGetUniformLocation(program, "viewPos"), 1, glm::value_ptr(viewPos));
}

void Window::renderSkybox(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    glUseProgram(skyboxShaderProgram);
    // Remove translation component from the view matrix for skybox
    glm::mat4 skyboxView = glm::mat4(glm::mat3(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(skyboxShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(skyboxView));
    glUniformMatrix4fv(glGetUniformLocation(skyboxShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDepthFunc(GL_LEQUAL); // Change depth function so skybox is rendered in the background
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS); // Reset depth function to default
    glBindVertexArray(0);
}

/**/

bool isSecondSphereVisible = true;


void Window::displayCallback(GLFWwindow* window) {

    int currentWidth, currentHeight;
    glfwGetFramebufferSize(window, &currentWidth, &currentHeight);

    float aspectRatio = static_cast<float>(currentWidth) / static_cast<float>(currentHeight);


    glm::mat4 viewMatrix = Cam->GetViewMatrix(); // View matrix from the camera
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(Cam->Zoom), aspectRatio, nearPlane, farPlane); // Projection matrix based on current window aspect ratio
    glm::vec3 viewPos = Cam->GetPosition(); // Camera position for view-dependent effects
    //glm::vec3 spherePOS =glm::vec3(-4.0f, 0.0f, -5.0f);
    glm::vec3 spherePOS =glm::vec3(4.0f, 1.0f, -4.0f);
 glm::vec3 test = glm::normalize(spherePOS - viewPos);
// Presets for cube map face directions and up vectors
glm::vec3 directions[6] = {
    glm::vec3(1.0, 0.0, 0.0),  // GL_TEXTURE_CUBE_MAP_POSITIVE_X
    glm::vec3(-1.0, 0.0, 0.0), // GL_TEXTURE_CUBE_MAP_NEGATIVE_X
    glm::vec3(0.0, 1.0, 0.0),  // GL_TEXTURE_CUBE_MAP_POSITIVE_Y
    glm::vec3(0.0, -1.0, 0.0), // GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
    glm::vec3(0.0, 0.0, 1.0),  // GL_TEXTURE_CUBE_MAP_POSITIVE_Z
    glm::vec3(0.0, 0.0, -1.0)  // GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

glm::vec3 ups[6] = {
    glm::vec3(0.0, -1.0, 0.0), // Positive X
    glm::vec3(0.0, -1.0, 0.0), // Negative X
    glm::vec3(0.0, 0.0, 1.0),  // Positive Y 
    glm::vec3(0.0, 0.0, -1.0), // Negative Y 
    glm::vec3(0.0, -1.0, 0.0), // Positive Z
    glm::vec3(0.0, -1.0, 0.0)  // Negative Z
};


    glBindFramebuffer(GL_FRAMEBUFFER, Window::dynamicCubeMapFBO);

    glViewport(0, 0, 2000, 2000); 

    isSecondSphereVisible = false;

    for(int face = 0; face < 6; ++face) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, dynamicCubeMapTextureID, 0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     
        //glm::mat4 view = glm::lookAt(cubeMapPosition, cubeMapPosition + adjustedDirection, ups[face]);
        glm::mat4 view = glm::lookAt(spherePOS, spherePOS + directions[face], ups[face]);
        glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, nearPlane, farPlane);
        
        //renderScene(view, cubeMapPosition , 1.0f, projection, true);
        renderScene(view, spherePOS, 1.0f, projection, true);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Reset viewport back to the window size
    glViewport(0, 0, currentWidth, currentHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //renderSkybox(viewMatrix, projectionMatrix);

    isSecondSphereVisible = true;

    // Render the main scene
    renderScene(viewMatrix, viewPos, aspectRatio, projectionMatrix, true);

    
    plotImGUI();

    glfwSwapBuffers(window);
    glfwPollEvents();
}
    //glm::mat4 viewMatrix = Cam->GetViewMatrix();
//     glm::vec3 viewPos = Cam->GetPosition();
// glm::mat4 projectionMatrix = glm::perspective(glm::radians(Cam->Zoom), aspectRatio, nearPlane, farPlane);
void Window::renderScene(const glm::mat4& viewMatrix, const glm::vec3 viewPos, float aspectRatio, const glm::mat4& projectionMatrix, bool isDynamicCubemapRendering) {
    if (isDynamicCubemapRendering) {
        // Render skybox here for dynamic cube map
        renderSkybox(viewMatrix, projectionMatrix);
 
    }
    
    glUseProgram(shaderProgram);
    setCommonUniforms(shaderProgram, viewMatrix, projectionMatrix, viewPos);

    // Set the model matrix for the sphere, positioning it in the world
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)); // Adjust position as needed
    model = glm::scale(model, glm::vec3(0.3f)); // Adjust scale if needed
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // Light 0
    glm::vec3 lightPosition0 = glm::vec3(0.0f, 7.0f, 0.0f);
    glm::vec3 lightColor0 = glm::vec3(1.0f, 1.0f, 1.0f) * 4.0f; // White light

    // Light 1
    glm::vec3 lightPosition1 = glm::vec3(-2.0f, -7.0f, -5.0f);
    glm::vec3 lightColor1 = glm::vec3(0.0f, 0.5f, 1.0f) * 2.5f; // Blueish light

    // Pass light properties to the shader
    glUniform3fv(glGetUniformLocation(shaderProgram, "lights[0].position"), 1, glm::value_ptr(lightPosition0));
    glUniform3fv(glGetUniformLocation(shaderProgram, "lights[0].color"), 1, glm::value_ptr(lightColor0));

    glUniform3fv(glGetUniformLocation(shaderProgram, "lights[1].position"), 1, glm::value_ptr(lightPosition1));
    glUniform3fv(glGetUniformLocation(shaderProgram, "lights[1].color"), 1, glm::value_ptr(lightColor1));

    // Example material properties
    glm::vec3 materialAlbedo = glm::vec3(1.0f, 0.2f, 0.5f); // Reddish material
    float materialMetallic = 0.1f; 
    float materialRoughness = 0.6f; // Fairly rough
    float ambientOcclusion = 0.6f; 

    // Set the PBR material properties
    glUniform3fv(glGetUniformLocation(shaderProgram, "albedo"), 1, glm::value_ptr(materialAlbedo));
    glUniform1f(glGetUniformLocation(shaderProgram, "metallic"), materialMetallic);
    glUniform1f(glGetUniformLocation(shaderProgram, "roughness"), materialRoughness);
    glUniform1f(glGetUniformLocation(shaderProgram, "ao"), ambientOcclusion);

    // Bind the VAO for the sphere
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, dynamicCubeMapTextureID); 
    glUniform1i(glGetUniformLocation(shaderProgram, "skybox"), 1);

    // Reflection strength can be adjusted here if needed
    glUniform1f(glGetUniformLocation(shaderProgram, "reflectionStrength"), 1.0f);

    // Draw your objects here
    if (skin != nullptr) {
        skin->draw(viewMatrix, projectionMatrix); // Pass the view projection matrix to the draw method
    }


    if (isSecondSphereVisible) {
        glUseProgram(SphereShaderProgram);

        glBindVertexArray(sphereVAO);
        // Set common uniforms (view, projection, camera position) for the SphereShaderProgram
        setCommonUniforms(SphereShaderProgram, viewMatrix, projectionMatrix, viewPos);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, dynamicCubeMapTextureID);
        glUniform1i(glGetUniformLocation(SphereShaderProgram, "skybox"), 0);
            

        // Dynamic Mirror Sphere!!!
        glm::mat4 sphereModel = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 1.0f, -7.0f)); // New position
        glUniformMatrix4fv(glGetUniformLocation(SphereShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(sphereModel));
        // Draw the sphere
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(sphereIndices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

    }

    // Second Sphere 
    glUseProgram(shaderProgram);
    glBindVertexArray(sphereVAO);
    setCommonUniforms(shaderProgram, viewMatrix, projectionMatrix, viewPos);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, dynamicCubeMapTextureID);
    glUniform1i(glGetUniformLocation(shaderProgram, "skybox"), 0);

    // Set the model matrix for the sphere, positioning it in the world
    glm::mat4 sphereModel2 = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, -5.0f)); // Adjust position as needed
    sphereModel2 = glm::scale(sphereModel2, glm::vec3(0.5f)); // Adjust scale if needed
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(sphereModel2));


    // Material properties
    glm::vec3 materialAlbedo2 = glm::vec3(0.2f, 0.7f, 0.5f); // Reddish material
    float materialMetallic2 = 0.0f; 
    float materialRoughness2 = 0.6f; // Fairly rough
    float ambientOcclusion2 = 0.5f; 

    // Light properties
    glm::vec3 lightPosition2 = glm::vec3(-5.0f, 5.0f, -5.0f); // Position in the scene
    glm::vec3 lightColor2 = glm::vec3(1.0f, 1.0f, 1.0f) * 2.0f; // White light

    // Set the PBR material properties
    glUniform3fv(glGetUniformLocation(shaderProgram, "albedo"), 1, glm::value_ptr(materialAlbedo2));
    glUniform1f(glGetUniformLocation(shaderProgram, "metallic"), materialMetallic2);
    glUniform1f(glGetUniformLocation(shaderProgram, "roughness"), materialRoughness2);
    glUniform1f(glGetUniformLocation(shaderProgram, "ao"), ambientOcclusion2);

    // Set the light properties
    glUniform3fv(glGetUniformLocation(shaderProgram, "lights[0].position"), 1, glm::value_ptr(lightPosition2));
    glUniform3fv(glGetUniformLocation(shaderProgram, "lights[0].color"), 1, glm::value_ptr(lightColor2));


    // Draw the sphere
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(sphereIndices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    // Third Sphere!! (Back pinkish metallic)
    glUseProgram(shaderProgram);
    glBindVertexArray(sphereVAO);
    setCommonUniforms(shaderProgram, viewMatrix, projectionMatrix, viewPos);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, dynamicCubeMapTextureID);
    glUniform1i(glGetUniformLocation(shaderProgram, "skybox"), 0);

    // Set the model matrix for the sphere, positioning it in the world
    glm::mat4 sphereModel3 = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, -2.0f, 6.0f)); // Adjust position as needed
    sphereModel3 = glm::scale(sphereModel3, glm::vec3(1.0f)); // Adjust scale if needed
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(sphereModel3));


    // Material properties
    glm::vec3 materialAlbedo3 = glm::vec3(0.8f, 0.5f, 0.8f); 
    float materialMetallic3 = 0.1f; 
    float materialRoughness3 = 0.7f; // Fairly rough
    float ambientOcclusion3 = 0.5f; 

    // Light properties
    glm::vec3 lightPosition3 = glm::vec3(5.0f, 2.0f, 10.0f); // Position in the scene
    glm::vec3 lightColor3 = glm::vec3(1.0f, 1.0f, 1.0f) * 2.0f; // White light

    // Set the PBR material properties
    glUniform3fv(glGetUniformLocation(shaderProgram, "albedo"), 1, glm::value_ptr(materialAlbedo3));
    glUniform1f(glGetUniformLocation(shaderProgram, "metallic"), materialMetallic3);
    glUniform1f(glGetUniformLocation(shaderProgram, "roughness"), materialRoughness3);
    glUniform1f(glGetUniformLocation(shaderProgram, "ao"), ambientOcclusion3);

    // Set the light properties
    glUniform3fv(glGetUniformLocation(shaderProgram, "lights[0].position"), 1, glm::value_ptr(lightPosition3)); 
    glUniform3fv(glGetUniformLocation(shaderProgram, "lights[0].color"), 1, glm::value_ptr(lightColor3)); 


    // Draw the sphere
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(sphereIndices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    // Fourth Sphere!! (Back Yellow)
    glUseProgram(shaderProgram);
    glBindVertexArray(sphereVAO);
    setCommonUniforms(shaderProgram, viewMatrix, projectionMatrix, viewPos);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, dynamicCubeMapTextureID);
    glUniform1i(glGetUniformLocation(shaderProgram, "skybox"), 0);

    // Set the model matrix for the sphere, positioning it in the world
    glm::mat4 sphereModel4 = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 5.0f)); // Adjust position as needed
    sphereModel4 = glm::scale(sphereModel4, glm::vec3(1.0f)); // Adjust scale if needed
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(sphereModel4));

    // Light properties for this sphere
    glm::vec3 lightPosition4 = glm::vec3(10.0f, 10.0f, 10.0f);
    glm::vec3 lightColor4 = glm::vec3(1.0f, 1.0f, 1.0f) * 2.5f;

    // Set this light's properties
    glUniform3fv(glGetUniformLocation(shaderProgram, "lights[0].position"), 1, glm::value_ptr(lightPosition4));
    glUniform3fv(glGetUniformLocation(shaderProgram, "lights[0].color"), 1, glm::value_ptr(lightColor4));


    // Material properties
    glm::vec3 materialAlbedo4 = glm::vec3(0.8f, 0.8f, 0.2f); 
    float materialMetallic4 = 0.0f; 
    float materialRoughness4 = 0.6f; // Fairly rough
    float ambientOcclusion4 = 0.5f; 

    // Set the PBR material properties
    glUniform3fv(glGetUniformLocation(shaderProgram, "albedo"), 1, glm::value_ptr(materialAlbedo4));
    glUniform1f(glGetUniformLocation(shaderProgram, "metallic"), materialMetallic4);
    glUniform1f(glGetUniformLocation(shaderProgram, "roughness"), materialRoughness4);
    glUniform1f(glGetUniformLocation(shaderProgram, "ao"), ambientOcclusion4);

    // Draw the sphere
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(sphereIndices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}
// helper to reset the camera
void Window::resetCamera() {
    Cam->Reset();
}


//float DELTATIME =  Window::getDeltaTime(deltaTime);

// callbacks - for Interaction
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Check for a key press.
        switch (key) {
            case GLFW_KEY_W:
                Cam->ProcessKeyboard(UP, deltaTime* 10.0);
                break;
            case GLFW_KEY_S:
                Cam->ProcessKeyboard(DOWN, deltaTime* 10.0);
                break;
            case GLFW_KEY_A:
                Cam->ProcessKeyboard(LEFT, deltaTime* 10.0);
                break;
            case GLFW_KEY_D:
                Cam->ProcessKeyboard(RIGHT, deltaTime * 10.0);
                break;

            case GLFW_KEY_Q:
                Cam->ProcessKeyboard(FORWARD, deltaTime * 10.0);
                break;
            case GLFW_KEY_E:
                Cam->ProcessKeyboard(BACKWARD, deltaTime * 10.0);
                break;

            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;

            case GLFW_KEY_R:
                resetCamera();
                break;

            case GLFW_KEY_Z: // Zoom in
                Cam->Zoom = std::max(Cam->Zoom - 1.0f, 1.0f);
                break;

            case GLFW_KEY_X: // Zoom out
                Cam->Zoom += 1.0f;
                break;

            case GLFW_KEY_K:
                drawSkelFlag ^= true;
                break;

            default:
                break;
        }

}



void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        LeftDown = (action == GLFW_PRESS);
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        RightDown = (action == GLFW_PRESS);
    }
}
void Window::cursor_callback(GLFWwindow* window, double xpos, double ypos) {
    static float lastX = 800.0f / 2.0;
    static float lastY = 600.0f / 2.0;
    static bool firstMouse = true;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    if (LeftDown) { // Only process mouse movement if LeftDown is true
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        Cam->ProcessMouseMovement(xoffset, yoffset);
    }
    else { // Reset lastX and lastY when the mouse is released
        firstMouse = true;
    }
}



void Window::plotImGUI()
{
	// Start a new ImGui frame. (To begin rendering the GUI elements)
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	//bool show_demo_window = true;
	//ImGui::ShowDemoWindow(&show_demo_window);

    // Define window flags for the ImGui window. These flags disable the title bar and prevent resizing.
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoResize;

	// Create a new ImGui window named "DOFs" (Degrees of Freedom) with the specified flags.
	ImGui::Begin("DOFs", NULL, window_flags);
    // Set the position and size of the ImGui window to the top-right corner of the application window.
	ImGui::SetWindowPos(ImVec2(Window::width - 190, 0));
	ImGui::SetWindowSize(ImVec2(190, Window::height));

    // Display the current frames per second (FPS).
    std::string fps = "FPS: " + std::to_string(character->GetFPS());
    ImGui::Text(fps.c_str());


    // Project 3

    // Determine the number of joints if a skeleton is present.
	int numOfJoint = 0;
	if(skel != nullptr) numOfJoint = skel->getNumOfJoint();

    // Iterate through all the joints in the skeleton.
	for (int i = 0; i < numOfJoint; i++)
	{
        // Create a collapsible tree node for each joint using its name.
		if (ImGui::TreeNode(jointsName[i].c_str()))
		{
			glm::vec3 rot, rotMin, rotMax;
			skel->getJointRot(i, rot);
			skel->getJointRotMin(i, rotMin);
			skel->getJointRotMax(i, rotMax);

            // Fetch and display draggable sliders for rotation in X, Y, Z axes.
            // The sliders are constrained by the joint's minimum and maximum rotation values.
            // Updating a slider will modify the joint's rotation accordingly.
            if (ImGui::DragFloat("RotX", &jointsRot[i].x, 0.005f, jointsRotMin[i].x, jointsRotMax[i].x, "%.3f", ImGuiSliderFlags_AlwaysClamp))
            {
                skel->setJointRot(i, jointsRot[i]); // Update the skeleton's joint rotation when the slider is changed.
            }
            if (ImGui::DragFloat("RotY", &jointsRot[i].y, 0.005f, jointsRotMin[i].y, jointsRotMax[i].y, "%.3f", ImGuiSliderFlags_AlwaysClamp))
            {
                skel->setJointRot(i, jointsRot[i]); // As above, for Y axis.
            }
            if (ImGui::DragFloat("RotZ", &jointsRot[i].z, 0.005f, jointsRotMin[i].z, jointsRotMax[i].z, "%.3f", ImGuiSliderFlags_AlwaysClamp))
            {
                skel->setJointRot(i, jointsRot[i]); // As above, for Z axis.
            }

            ImGui::TreePop(); // End the tree node for this joint.
		}
	}

	ImGui::End(); // End the ImGui window.

    // Render the ImGui elements that have been defined.
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}