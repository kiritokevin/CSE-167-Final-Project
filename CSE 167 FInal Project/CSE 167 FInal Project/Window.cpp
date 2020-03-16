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
    int freeCamera = 0;
    int FPVcount = 0;
    int FreeCameraCount = 0;
    // first person view on/off
    // 0: Third person view
    // 1: First person view
    int FPV = 0;
    
    // collision bounding on/off
    bool debugCollision = false;
    
    // rotating light factor
    float degree = 0.0;
    glm::vec3 prevPositionSphere = glm::vec3(1.0f);
    glm::vec3 prevPosition = glm::vec3(1.0f);
	// Boolean switches
	bool glRasterize = true;

    GLfloat pointSize(3);

    // Bunny and Dragon
    skybox* sky;

    // constant TPV eye posision
    glm::vec3 TPV_angle = glm::vec3(0, 10, 50);
    
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
    GLuint colorLoc;

    // Geometry nodes
    Geometry* sphere;
    
    // transform nodes

    
    // temp transformation matrices

    
    // boundings
    Cube* c;
    Rec* base;
    Rec* buildingA;
    std::vector<Rec*> rec_list;
    // Roadmap:
    //randomize: https://www.geeksforgeeks.org/rand-and-srand-in-ccpp/
//    1. a 2D arrary to store the city map
//    2. initialize the roadmap (initializeRoadmap)
//
//    3.
//    1: randomize intersections
//    2: build roads
//    3: randomize buildings
//    4: randomize one point; check the surrounding; build court
//
//    4. draw the city
//    1: displayCallBack: run through the city map
//    2: draw the block based on the number in roadmap
//
//    5. update city
//    1: redo the initialization (based on some key press)
    // initialize road map 2D array
    int roadmap [8][8] = {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0}
    };
    
};

// initialize the roadmap to build the city
// we have six kinds of blocks
// intersection, road(row), road(column) and three different types of building.
// 1: intersection.
// 2: road(row)
// 3: road(column)
// 4,5,6: different types of building
// 7: baskerball court

void Window::initialize_roadmap()
{
    // choose the intersections (only find two in this case)
    int count = 0;
    while(count < 2)
    {
        // TODO: Some problems with random generator
        // select seed based on system time
        srand(time(0));
        
        // some random number
        int row = rand() % 8;
        
        //srand(13);
        int col = rand() % 8;
        
        // std::cout << row << " " << col << std::endl;
        
        // check whether current position is valid
        // if valid set it to intersection and assign
        if(roadmap[row][col] == 0 || roadmap[row][col] == 2 || roadmap[row][col] == 3)
        {
            // assign intersection
            roadmap[row][col] = 1;
            
            // assign road (row) and road (col)
            for(int i = 0; i < 8; i++)
            {
                if(i != col)
                {
                    // if not intersection
                    if(roadmap[row][i] != 1)
                    {
                        // add a intersection if current position is 3 previously
                        if(roadmap[row][i] == 3)
                        {
                            roadmap[row][i] = 1;
                        }
                        else
                        {
                            roadmap[row][i] = 2;
                        }
                    }
                }
                if(i != row)
                {
                    // if not intersection
                    if(roadmap[i][col] != 1)
                    {
                        // add a intersection if current position is 2 previously
                        if(roadmap[i][col] == 2)
                        {
                            roadmap[i][col] = 1;
                        }
                        else
                        {
                            roadmap[i][col] = 3;
                        }
                    }
                }
            }

            // increase counter
            count += 1;
        }
    }
    
    // go through the loop and assign to the other building
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            // empty position
            if(roadmap[i][j] == 0)
            {
                // 4 ~ 6
                roadmap[i][j] = rand() % 3 + 4;
            }
        }
    }
    
    // two blocks' court
    
    // print out the road
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            // print the number
            std::cout << roadmap[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

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
    if(!programCube)
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
    colorLoc = glGetUniformLocation(program, "color");
    return true;
}

bool Window::initializeObjects()
{
    // create skybox object
    sky = new skybox(5.0f, view, projection);
    //sphere = new Geometry("/Users/KZ/CSE-167-Final-Project/CSE 167 FInal Project/CSE 167 FInal Project/shaders/obj/sphere.obj");
	sphere = new Geometry("/Users/yilincai/CSE167/CSE-167-Final-Project/CSE 167 FInal Project/CSE 167 FInal Project/shaders/obj/sphere.obj");
    
    // initialize boundings
    c = new Cube(1.0f, sphere->min, sphere->max);
    
    // initialize different types of buildings (and roads)
    buildingA = new Rec(sphere -> min + glm::vec3(-22,0,20), 10.0f, 80.0f, 10.0f);
    //base= new Rec(sphere->min+glm::vec3(-22,0,20),50.0f,1.0f,60.0f);
    
    // initialize road map
    initialize_roadmap();
    
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
	// The quad must be made with the window
    
	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int w, int h)
{
#ifdef __APPLE__
    width = w;
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
    if(FPV == 1)
    {
        FPVcount++;
    }
    
    if(freeCamera == 1)
    {
        FreeCameraCount++;
    }
}

// draw the city
//    4. draw the city
//    1: run through the city map
//    2: draw the block based on the number in roadmap
void Window::drawCity()
{
    if(rec_list.size()!=0){
        for(int x = 0; x<rec_list.size();x++){
            delete rec_list[x];
        }
        rec_list={};
    }
     glm::vec3 starting_point = sphere->min+glm::vec3(-8,0,20);
    //buildingA ->draw(programCube, glm::vec3(0.67,1,0.5), debugCollision, view, projection);
    for(int i = 0;i<8;i++){
        for(int j = 0;j<8;j++){
            glm::vec3 current_point = glm::vec3(starting_point[0]+2*i,0,starting_point[2]+2*j);
            Rec* new_rec;
            glm::vec3 block_color;
            if(roadmap[i][j]==4){
                new_rec = new Rec(current_point,2,-3,2);
            }
            else if(roadmap[i][j]==5){
                new_rec = new Rec(current_point,2,-6,2);
            }
            else if(roadmap[i][j]==6){
                new_rec = new Rec(current_point,2,-9,2);
            }
            else{
                new_rec = new Rec(current_point,2,-1,2);
            }
            if(roadmap[i][j]==1||roadmap[i][j]==2||roadmap[i][j]==3){
                block_color = glm::vec3(0.39,0.35,0.35);
            }
            else{
                block_color = glm::vec3(0.67,1,0.5);
            }
            new_rec->draw(programCube,block_color, debugCollision,view,projection);
            rec_list.push_back(new_rec);
        }
    }
}

void Window::displayCallback(GLFWwindow* window)
{
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // free camera, FPV and TPV switch
    if(freeCamera == 1)
    {
        FPVcount = 0;
        if(FreeCameraCount == 1)
        {
            eye = glm::vec3(0, 10, 50);
        }
        view = glm::lookAt(eye, eye + center, up);
        
        // draw sphere
        sphere -> draw(programSphere, glm::mat4(1.0f), view, projection);
        
        // draw the bounding cube
        if(debugCollision)
        {
            c -> draw(programCube, view, projection);
        }

    }
    
    else if(FPV == 1)
    {
        FreeCameraCount = 0;
        eye = sphere->midPoint;
        
        // always look at world center after switching to the FPV
        if(FPVcount == 1)
        {
            center = glm::vec3(0,0,0);
        }
        view = glm::lookAt(eye, eye + center, up);
        
    }
    else
    {
        FreeCameraCount = 0;
        FPVcount = 0;
        // reset eye position
        eye = sphere -> midPoint + TPV_angle;
        
        // reset center position, keep looking at the ball
        center = sphere -> midPoint;
        view = glm::lookAt(eye, center, up);
        

        
        // draw sphere
        sphere -> draw(programSphere, glm::mat4(1.0f), view, projection);
        
        // draw base
        /*for(int i = 0;i<rec_list.size();i++){
            rec_list[i]->draw(programCube, glm::vec3(0.67,1,0.5), false, view, projection);
        }*/
        
        // draw the bounding cube
        if(debugCollision)
        {
            c -> draw(programCube, view, projection);
        }
    }
    // draw skybox
    sky -> draw(programSkybox, view);
    
    // draw city
    drawCity();
    
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
            // get current cursor position
            rightPressed = 1;
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            
            // get previous mouse position on the sphere
            prevPosition = glm::vec3(xpos,ypos,0.0f);
            prevPositionSphere = trackingBallMapping(xpos, ypos);
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
            // get current cursor position
            leftPressed = 1;
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            
            // get previous
            prevPosition = glm::vec3(xpos,ypos,0.0f);
            prevPositionSphere = trackingBallMapping(xpos, ypos);
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
    
    if(leftPressed == 1)
    {
        rotateCamera(prevPositionSphere, v);
    }
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
            //view = glm::lookAt(eye, center, up);
        }
}

void Window::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
}

// check collision
bool Window::check_collision(Cube* box, Rec* building)
{
    // if both x, y, z overlapped, we then have a collision
    bool collisionX = box -> cube_max.x >= building -> rec_max.x - building -> rec_length
    && building -> rec_max.x >= box -> cube_max.x - box -> length;
    bool collisionY = box -> cube_max.y >= building -> rec_max.y - building -> rec_height
    && building -> rec_max.y >= box -> cube_max.y - box -> length;
    bool collisionZ = box -> cube_max.z >= building -> rec_max.z - building -> rec_width
    && building -> rec_max.z >= box -> cube_max.z - box -> length;
    
    return collisionX && collisionY && collisionZ;
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
            
            // free view
            case GLFW_KEY_F1:
                if(freeCamera == 0)
                {
                    freeCamera = 1;
                }
                else
                {
                    freeCamera = 0;
                }
                break;
                    
            case GLFW_KEY_F2:

                break;
            case GLFW_KEY_F3:
                break;
            
            // re-calculate the roadmap
            case GLFW_KEY_N:
                // clean the old road map
                for (int i = 0; i < 8; i++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        // empty position
                        roadmap[i][j] = 0;
                    }
                }
                std::cout << std::endl;
                    
                // initalize again
                initialize_roadmap();
                break;
                
            // move left
            case GLFW_KEY_A:
                if(FPV == 1)
                {
                    // TODO: Not really work here
                    glm::vec3 direction = glm::vec3((center.x - sphere -> midPoint.x), 0.0f, (center.z - sphere -> midPoint.z));
                    direction = glm::normalize(glm::cross(direction, up));
                    sphere -> moveFPV(glm::vec3(-direction.x, 0, -direction.z));
                    c -> moveFPV(glm::vec3(-direction.x, 0, -direction.z));
                }
                else
                {
                    sphere -> move(2);
                    c -> move(2);
                }

                break;
                    
            // move right
            case GLFW_KEY_D:
                if(FPV == 1)
                {
                    // TODO: Not really work here
                    glm::vec3 direction = glm::vec3((center.x - sphere -> midPoint.x), 0.0f, (center.z - sphere -> midPoint.z));
                    direction = glm::normalize(glm::cross(direction, up));
                    sphere -> moveFPV(glm::vec3(direction.x, 0, direction.z));
                    c -> moveFPV(glm::vec3(direction.x, 0, direction.z));
                }
                else
                {
                    sphere -> move(3);
                    c -> move(3);
                }
                break;
                    
            // forward
            case GLFW_KEY_W:
                if(FPV == 1)
                {
                    // find the direction that the object should move
                    glm::vec3 direction = glm::vec3((center.x - sphere -> midPoint.x), 0.0f, (center.z - sphere -> midPoint.z));
                    direction = glm::normalize(direction);
                    sphere -> moveFPV(direction);
                    c -> moveFPV(direction);
                }
                else
                {
                    sphere -> move(0);
                    c -> move(0);
                }
                break;
                    
            // backward
            case GLFW_KEY_S:
                if(FPV == 1)
                {
                    // find the direction that the object should move
                    glm::vec3 direction = glm::vec3((center.x - sphere -> midPoint.x), 0.0f, (center.z - sphere -> midPoint.z));
                    direction = glm::normalize(direction);
                    sphere -> moveFPV(-direction);
                    c -> moveFPV(-direction);
                }
                else
                {
                    sphere -> move(1);
                    c -> move(1);
                }
                break;
            
            // move the camera freely with direction keys
            case GLFW_KEY_UP:
                if(freeCamera == 1)
                {
                    glm::vec3 direction = center - eye;
                    direction = glm::normalize(direction);
                    eye = eye + direction;
                }
                break;
            case GLFW_KEY_DOWN:
                if(freeCamera == 1)
                {
                    glm::vec3 direction = center - eye;
                    direction = glm::normalize(direction);
                    eye = eye - direction;
                }
                break;
            case GLFW_KEY_LEFT:
                if(freeCamera == 1)
                {
                    glm::vec3 direction = center - eye;
                    direction = glm::normalize(glm::cross(direction, up));
                    eye = eye - direction;
                }
                break;
            case GLFW_KEY_RIGHT:
                if(freeCamera == 1)
                {
                    glm::vec3 direction = center - eye;
                    direction = glm::normalize(glm::cross(direction, up));
                    eye = eye + direction;
                }
                break;
                    
            // debug collision
            case GLFW_KEY_C:
                if(debugCollision)
                {
                    debugCollision = false;
                }
                else
                {
                    debugCollision = true;
                }
                break;
                    
            // switch to FPV
            case GLFW_KEY_V:
                if (FPV == 0)
                {
                    FPV = 1;
                }
                else
                {
                    FPV = 0;
                }
                break;
                    
            default:
                break;
            }
        }
	}
}
