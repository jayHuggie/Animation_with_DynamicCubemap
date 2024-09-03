#include "Window.h"
#include "core.h"

double deltaTime = 0.0;

void error_callback(int error, const char* description) {
    // Print error.
    std::cerr << description << std::endl;
}

void setup_callbacks(GLFWwindow* window) {
    // Set the error callback.
    glfwSetErrorCallback(error_callback);
    // Set the window resize callback.
    glfwSetWindowSizeCallback(window, Window::resizeCallback);

    // Set the key callback.
    glfwSetKeyCallback(window, Window::keyCallback);

    // Set the mouse and cursor callbacks
    glfwSetMouseButtonCallback(window, Window::mouse_callback);
    glfwSetCursorPosCallback(window, Window::cursor_callback);
}

void setup_opengl_settings() {
    // Enable depth buffering.
    glEnable(GL_DEPTH_TEST);
    // Related to shaders and z value comparisons for the depth buffer.
    glDepthFunc(GL_LEQUAL);
    // Set polygon drawing mode to fill front and back of each polygon.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Set clear color to black.
    glClearColor(0.0, 0.0, 0.0, 0.0);

    // Project 3
    //back culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void print_versions() {
    // Get info of GPU and supported OpenGL version.
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version supported: " << glGetString(GL_VERSION)
              << std::endl;

    // If the shading language symbol is defined.
#ifdef GL_SHADING_LANGUAGE_VERSION
    std::cout << "Supported GLSL version is: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
}

int main(int argc, char* argv[]) {
    
    // Create the GLFW window.
    GLFWwindow* window = Window::createWindow(800, 600);
    if (!window) exit(EXIT_FAILURE);

    /* 
    //Set the name of skel file getting from argv
    if (argc > 1)
    {
        Window::setSkelFile(argv[1]);
    }
    */
    // Project 2
   	//initialize imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// Setup Platform/Renderer backends
	const char* glsl_version = "#version 330 core";
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

        //Set the name of skel and skin file getting from argv
    // Check if there are any command-line arguments passed; if not, set default files
    if (argc == 1) {
        Window::setSkelFile("skelFile/wasp.skel");
        Window::setSkinFile("skinFile/wasp.skin");
        Window::setAnimFile("animFile/wasp_walk.anim");

        Window::setSkelFile2("skelFile/crypto.skel");
        Window::setSkinFile2("skinFile/crypto.skin");
    } /*else {
        for (int i = 1; i < argc; i++) {
            int j = 0;
            while (argv[i][j] != '\0') j++;
            if (strcmp(argv[i] + j - 5, ".skel") == 0) {
                Window::setSkelFile(argv[i]);
            } else if (strcmp(argv[i] + j - 5, ".skin") == 0) {
                Window::setSkinFile(argv[i]);
            } else if (strcmp(argv[i] + j - 5, ".anim") == 0) {
                Window::setAnimFile(argv[i]);
            }
        }
    }*/
    
    // Test!!
    // Set the name of the skel file to "dragon.skel"
    //Window::setSkelFile("skelFile/dragon.skel");


    // Print OpenGL and GLSL versions.
    print_versions();
    // Setup callbacks.
    setup_callbacks(window);
    // Setup OpenGL settings.
    setup_opengl_settings();

    // Initialize the shader program; exit if initialization fails.
    if (!Window::initializeProgram()) exit(EXIT_FAILURE);
    // Initialize objects/pointers for rendering; exit if initialization fails.
    if (!Window::initializeObjects()) exit(EXIT_FAILURE);
    
    
    double lastTime = glfwGetTime();


    // Loop while GLFW window should stay open.
    while (!glfwWindowShouldClose(window)) {

        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Main render display callback. Rendering of objects is done here.
        Window::displayCallback(window);

        // Idle callback. Updating objects, etc. can be done here.
        Window::idleCallback();
    }

    // Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

    Window::cleanUp();
    // Destroy the window.
    glfwDestroyWindow(window);
    // Terminate GLFW.
    glfwTerminate();

    exit(EXIT_SUCCESS);
}