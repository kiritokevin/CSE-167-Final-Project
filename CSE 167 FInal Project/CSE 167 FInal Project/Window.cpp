#include "Window.h"

/* 
 * Declare your variables below. Unnamed namespace is used here to avoid 
 * declaring global or static variables.
 */
namespace
    {
    int width, height;
    std::string windowTitle("GLFW Starter Project");
    // variable for mouse and scroll
    int leftPressed = 0;
    int rightPressed = 0;
    int controlMode = 1;
    int debugCollision = 1;
    
    // rotating light factor
    float degree = 0.0;
    glm::vec3 prevPositionSphere = glm::vec3(1.0f);
    glm::vec3 prevPosition = glm::vec3(1.0f);
	// Boolean switches
	bool glRasterize = true;

    GLfloat pointSize(3);

    // Bunny and Dragon
    skybox* sky;

	glm::vec3 eye(0, 10, 50); // Camera position.
	glm::vec3 center(0, 0, 0); // The point we are looking at.
	glm::vec3 up(0, 1, 0); // The up direction of the camera.
	float fovy = 60;
	float near = 1;
	float far = 1000;
	glm::mat4 view = glm::lookAt(eye, center, up); // View matrix, defined by eye, center and up.
	glm::mat4 projection; // Projection matrix.
    glm::vec3 currentPosition;

	GLuint program; // The shader program id.
    GLuint programSphere;
    GLuint programSkybox;
    GLuint programCube;
	GLuint projectionLoc; // Location of projection in shader.

	GLuint viewLoc; // Location of view in shader.
	GLuint modelLoc; // Location of model in shader.
    GLuint viewPosLoc; // eye position

    // Geometry nodes
    Geometry* sphere;
    
    // transform nodes

    
    // temp transformation matrices

    
    // boundings
    Cube* c;
};

bool Window::initializeProgram()
{
	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    
	// Check the shader programs.
	if (!program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}
    
    // shader for skybox
    programSkybox = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");
    
    // check
    if(!programSkybox)
    {
        std::cerr << "Failed to initialize skybox shader program" << std::endl;
        return false;
    }
    
    programSphere = LoadShaders("shaders/sphere.vert", "shaders/sphere.frag");
    
    // check
    if(!programSphere)
    {
        std::cerr << "Failed to initialize skybox shader program" << std::endl;
        return false;
    }
    
    programCube = LoadShaders("shaders/cube.vert", "shaders/cube.frag");
    
    // check
    if(!programSphere)
    {
        std::cerr << "Failed to initialize skybox shader program" << std::endl;
        return false;
    }

	// Activate the shader program.
	glUseProgram(program);
	// Get the locations of uniform variables.
	projectionLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");
    return true;
}

bool Window::initializeObjects()
{
    // create skybox object
    sky = new skybox(5.0f, view, projection);
    sphere = new Geometry("/Users/KZ/CSE-167-Final-Project/CSE 167 FInal Project/CSE 167 FInal Project/shaders/obj/sphere.obj");
	
    // initialize boundings
    c = new Cube(1.0f, sphere->min, sphere->max);
    return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
    delete sky;

	// Delete the shader programs.
	glDeleteProgram(program);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);
	
	// Initialize the quad that will be textured with your image
//	// The quad must be made with the window
    
	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int w, int h)
{
#ifdef __APPLE__
    width = w ;
    height = h;
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height);

#endif

	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	projection = glm::perspective(glm::radians(fovy),
		(float)width / (float)height, near, far);
}

void Window::idleCallback()
{

}

void Window::displayCallback(GLFWwindow* window)
{
    // Switch back to using OpenGL's rasterizer
    //glUseProgram(program);
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw skybox
    sphere -> draw(programSphere, glm::mat4(1.0f), view, projection);
    sky -> draw(programSkybox, view);
    if(debugCollision == 1)
    {
        c -> draw(programCube, view, projection);
    };

    
    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);

}

/* Part 4: Mouse and scroll callbacks */
void Window::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        // check press or release
        if(action == GLFW_PRESS)
        {

        }
        else if(action == GLFW_RELEASE)
        {
            rightPressed = 0;
        }
    }
    else if(button == GLFW_MOUSE_BUTTON_LEFT)
    {
        // check press or release
        if(action == GLFW_PRESS)
        {

        }
        else if(action == GLFW_RELEASE)
        {
            leftPressed = 0;
        }
    }
}

// get the mapping of cursor movement
glm::vec3 Window::trackingBallMapping(double xpos, double ypos)
{
    glm::vec3 v;
    float d;
    v.x = (2.0 * xpos - width/2) / width/2;
    v.y = (height/2 - 2.0 * ypos) / height/2;
    v.z = 0.0f;

    d = glm::length(v);

    v.z = sqrtf(1.001f - d*d);

    v = glm::normalize(v);

    return v;
    
}

void Window::cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    // get the new cursor position on sphere
    glm::vec3 v = trackingBallMapping(xpos, ypos);
    glm::vec3 curr = glm::vec3(xpos, ypos, 0.0f);
    
    // update previous position
    prevPosition = curr;
    prevPositionSphere = v;
}



// rotate the lookat position
void Window::rotateCamera(glm::vec3 prev, glm::vec3 after)
{
        // get the unit value of direction
        glm::vec3 direction = after - prev;
        float velocity = glm::length(direction);

        // if little movement, do nothing
        if (velocity > 0.0001)
        {
            // scale up velocity
    //        velocity = velocity * 10;
            float angle = 0.3f * glm::acos(glm::min(1.0f, glm::dot(prev, after)));
            glm::vec3 normalVec = glm::cross(after, prev);
            glm::vec3 rotAxis = glm::normalize(normalVec);
            // calculate angle
            glm::mat4 rotateMatrix(1);
            rotateMatrix = glm::rotate(rotateMatrix, glm::degrees(angle), rotAxis);
            // glm::mat4 model = skybox.model * rotateMatrix;
            center = glm::vec3(rotateMatrix * glm::vec4(center - eye, 0));
            view = glm::lookAt(eye, center, up);
        }
}

void Window::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Section 4: Modify below to add your key callbacks.
	 */

	 // Check for a key press.
	if (action == GLFW_PRESS)
	{
		// Uppercase key presses (shift held down + key press)
		if (mods == GLFW_MOD_SHIFT) {
			switch (key) {
                case GLFW_KEY_S:
                    break;
                    
                default:
                    break;
			}
		}
        else
        {

            // Deals with lowercase key presses
            switch (key)
            {
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_1:
                break;
            case GLFW_KEY_2:
                break;
                    
            case GLFW_KEY_3:
                break;
            case GLFW_KEY_0:
                break;
                    
            case GLFW_KEY_F1:
                break;
            case GLFW_KEY_F2:

                break;
            case GLFW_KEY_F3:
                break;
                    
            case GLFW_KEY_N:
                break;
                
            // move left
            case GLFW_KEY_A:
                sphere -> move(2);
                c -> move(2);
                break;
                    
            // move right
            case GLFW_KEY_D:
                sphere -> move(3);
                c -> move(3);
                break;
                    
            // forward
            case GLFW_KEY_W:
                sphere -> move(0);
                c -> move(0);
                break;
                    
            // backward
            case GLFW_KEY_S:
                sphere -> move(1);
                c -> move(1);
                break;
                    
            // debug collision
            case GLFW_KEY_C:
                if(debugCollision == 1)
                {
                    debugCollision = 0;
                }
                else
                {
                    debugCollision = 1;
                }
                break;
                    
            default:
                break;
            }
        }
	}
}
