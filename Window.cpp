
#include "Window.h"
#include <glm/gtx/string_cast.hpp>

/* 
 * Declare your variables below. Unnamed namespace is used here to avoid 
 * declaring global or static variables.
 */
namespace
{
	int width, height;
	std::string windowTitle("GLFW Starter Project");

	// Boolean switches
	bool glRasterize = true;

	Cube* cube;
	PointCloud* cubePoints; 
	Object* currentObj;  // The object currently displaying.
    Object* dragon;
    Object* bunny;
    Object* bear;
    Object* light_sphere;
	RasterizerQuad* quad;  // Object textured with your rasterization results

	glm::vec3 eye(0, 0, 1); // Camera position.
	glm::vec3 center(0, 0, 0); // The point we are looking at.
	glm::vec3 up(0, 1, 0); // The up direction of the camera.
	float fovy = 60;
	float near = 1;
	float far = 1000;
	glm::mat4 view = glm::lookAt(eye, center, up);// View matrix, defined by eye, center and up.
    glm::mat4 origin_view = view;
	glm::mat4 projection; // Projection matrix.
    glm::vec3 z_rm(0,0,1); // z axis rotation matrix
    glm::vec3 y_rm(0,1,0); // y axis rotation matrix
    glm::vec3 t_bm(2,0,0); // bunny initial translation matrix
    glm::vec3 t_dm(-2,0,0); // dragon intial translation matrix

    int left_press = 0;
    int right_press = 0;
    double cur_x,cur_y;
    glm::vec3 ball_point(0,0,20);
    glm::vec3 lightPos = glm::vec3(1.8f,0,1);
    glm::vec3 pointLightPos = glm::vec3(-2.0f,0,1);
    int change_light = 0;
    int dirLight_switch = 1;
    int object_switch = 0;
    int key_press = 1;
    int object_pointer = 1;
    int start = 0;
    int curve_point_index=0;

	GLuint program; // The shader program id.
	GLuint projectionLoc; // Location of projection in shader.
	GLuint viewLoc; // Location of view in shader.
	GLuint modelLoc; // Location of model in shader.
	GLuint colorLoc; // Location of color in shader.

	GLuint programQuad;

    std::vector<std::string> texture_list
    {   "/Users/yilincai/CSE167/hw0_example/Texture/PalldioPalace_intern_right.jpg",
        "/Users/yilincai/CSE167/hw0_example/Texture/PalldioPalace_intern_left.jpg",
        "/Users/yilincai/CSE167/hw0_example/Texture/PalldioPalace_intern_top.jpg",
        "/Users/yilincai/CSE167/hw0_example/Texture/PalldioPalace_intern_base.jpg",
        "/Users/yilincai/CSE167/hw0_example/Texture/PalldioPalace_intern_front.jpg",
        "/Users/yilincai/CSE167/hw0_example/Texture/PalldioPalace_intern_back.jpg"
    };

    GLuint camera_switch;
    std::vector<Transform*> rotate_trans_list;
    std::vector<Transform*> transform_node_list;
    Transform* root_transform;
    Transform* head_trans;
    Transform* left_arm_trans;
    Transform* right_arm_trans;
    Transform* left_leg_trans;
    Transform* right_leg_trans;
    Transform* left_eye_trans;
    Transform* initial;
    Geometry* robot_body;
    Geometry* robot_head;
    Geometry* robot_limb;
    Geometry* robot_eye;
    glm::mat4 initial_translation;
    glm::mat4 trackball_rotation = glm::mat4(1.0f);
    glm::mat4 left_arm_rotation;
    glm::mat4 right_arm_rotation;
    int swing_direction = 0;
    int timing = 0;
    std::vector<glm::vec3> curve_vertices;
};

bool Window::initializeProgram()
{
	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
	// This shader program is for displaying your rasterizer results
	// DO NOT MODIFY THESE SHADER PROGRAMS
	programQuad = LoadShaders("shaders/RasterizerQuad.vert", "shaders/RasterizerQuad.frag");

	// Check the shader programs.
	if (!program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}
	if (!programQuad)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	// Activate the shader program.
	glUseProgram(program);
	// Get the locations of uniform variables.
	projectionLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");
	colorLoc = glGetUniformLocation(program, "color");
    camera_switch = glGetUniformLocation(program, "camera_switch");
    
    return true;
}

bool Window::initializeObjects()
{
    //Create skybox
    cube = new Cube(0.5f,texture_list);
    
    std::vector<glm::vec3> curve_points = cube->get_list();
    for(int x = 0;x<curve_points.size();x++){
        curve_vertices.push_back(curve_points[x]);
    }
    initial_translation = glm::translate(glm::mat4(1.0f), curve_vertices[curve_point_index]);
    curve_point_index++;
    cube->setModel(glm::scale(cube->getModel(), glm::vec3(5,5,5)));
    //initial_translation = glm::translate(glm::mat4(1.0f),glm::vec3(0,0,-30));
    root_transform = new Transform(glm::mat4(1.0f));
    initial = new Transform(initial_translation);
    head_trans = new Transform(glm::translate(glm::mat4(1.0f),glm::vec3(0,1.25,0)));
    left_arm_trans = new Transform(glm::translate(glm::mat4(1.0f),glm::vec3(-1.5,0,0)));
    right_arm_trans = new Transform(glm::translate(glm::mat4(1.0f),glm::vec3(1.5,0,0)));
    left_leg_trans = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(-0.6,-1.3,0)));
    right_leg_trans = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.6,-1.3,0)));
    left_eye_trans = new Transform(glm::scale(glm::translate(glm::mat4(1.0f),glm::vec3(0,1.5,1)),glm::vec3(2,2,2)));
    robot_body  = new Geometry("/Users/yilincai/CSE167/hw0_example/body_s.obj",modelLoc);
    robot_head = new Geometry("/Users/yilincai/CSE167/hw0_example/head_s.obj",modelLoc);
    robot_limb = new Geometry("/Users/yilincai/CSE167/hw0_example/limb_s.obj",modelLoc);
    robot_eye = new Geometry("/Users/yilincai/CSE167/hw0_example/eyeball_s.obj",modelLoc);
    root_transform->addChild(initial);
    
    Transform* rotation = new Transform(glm::mat4(1.0f));
    initial->addChild(rotation);
    rotate_trans_list.push_back(rotation);
    transform_node_list.push_back(rotation);
    for(int i = -2;i<3;i++){
        for(int j = -2; j<3; j++){
            glm::mat4 right_hand_transf = glm::translate(glm::mat4(1.0f),glm::vec3(i*4,0,j*4));
            Transform* add_right_robot = new Transform(right_hand_transf);
            transform_node_list.push_back(add_right_robot);
            rotation->addChild(add_right_robot);
            add_right_robot->addChild(robot_body); //body position
            add_right_robot->addChild(head_trans);
            head_trans->addChild(robot_head);//head position
            add_right_robot->addChild(left_arm_trans);
            add_right_robot->addChild(right_arm_trans);
            add_right_robot->addChild(left_leg_trans);
            add_right_robot->addChild(right_leg_trans);
            add_right_robot->addChild(left_eye_trans);
            left_eye_trans->addChild(robot_eye);
            left_arm_trans->addChild(robot_limb); //left arm
            right_arm_trans->addChild(robot_limb); // right arm
            left_leg_trans->addChild(robot_limb);
            right_leg_trans->addChild(robot_limb);
            //rotate_trans_list.push_back(rotation);
        }
    }
    
    
	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete cube;
	delete cubePoints;
	delete quad;
    delete robot_body;
    delete robot_head;
    delete robot_limb;
    delete robot_eye;
    delete root_transform;
    delete initial;
    delete head_trans;
    delete left_arm_trans;
    delete right_arm_trans;
    delete left_leg_trans;
    delete right_leg_trans;
    delete left_eye_trans;
    for(int i = 0;i<transform_node_list.size();i++){
        delete transform_node_list[i];
    }
    //Delete the shader programs.
	glDeleteProgram(program);
	glDeleteProgram(programQuad);
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
	//quad = new RasterizerQuad(width, height);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int w, int h)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height);
    cur_x = w;
    cur_y = h;
    w = w * 2;
    h = h * 2;
#endif
	width = w;
	height = h;
	// Resize our CPU rasterizer's pixel buffer and zbuffer
	//quad->updateBufSiz(width, height);

	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	projection = glm::perspective(glm::radians(fovy),
		(float)width / (float)height, near, far);
}

void Window::idleCallback()
{
	// Perform any updates as necessary.
    if(timing==10){
        timing = 0;
        if(swing_direction==0){
            left_arm_rotation = glm::rotate(glm::mat4(1.0f),glm::radians(30.0f),glm::vec3(1,0,0));
            right_arm_rotation = glm::rotate(glm::mat4(1.0f),glm::radians(-30.0f),glm::vec3(1,0,0));
            left_arm_trans->setC(left_arm_trans->getC()*left_arm_rotation);
            right_leg_trans->setC(right_leg_trans->getC()*left_arm_rotation);
            right_arm_trans->setC(right_arm_trans->getC()*right_arm_rotation);
            left_leg_trans->setC(left_leg_trans->getC()*right_arm_rotation);
            swing_direction = 1;
        }
        else if(swing_direction==1){
            left_arm_rotation = glm::rotate(glm::mat4(1.0f),glm::radians(-60.0f),glm::vec3(1,0,0));
            right_arm_rotation = glm::rotate(glm::mat4(1.0f),glm::radians(60.0f),glm::vec3(1,0,0));
            left_arm_trans->setC(left_arm_trans->getC()*left_arm_rotation);
            right_leg_trans->setC(right_leg_trans->getC()*left_arm_rotation);
            right_arm_trans->setC(right_arm_trans->getC()*right_arm_rotation);
            left_leg_trans->setC(left_leg_trans->getC()*right_arm_rotation);
            swing_direction = 2;
        }
        else{
            left_arm_rotation = glm::rotate(glm::mat4(1.0f),glm::radians(60.0f),glm::vec3(1,0,0));
            right_arm_rotation = glm::rotate(glm::mat4(1.0f),glm::radians(-60.0f),glm::vec3(1,0,0));
            left_arm_trans->setC(left_arm_trans->getC()*left_arm_rotation);
            right_leg_trans->setC(right_leg_trans->getC()*left_arm_rotation);
            right_arm_trans->setC(right_arm_trans->getC()*right_arm_rotation);
            left_leg_trans->setC(left_leg_trans->getC()*right_arm_rotation);
            swing_direction=1;
        }
    }
    else{
        timing++;
    }
    glm::vec3 point_difference;
    if(curve_point_index==curve_vertices.size()){
        point_difference = curve_vertices[0]-curve_vertices[curve_point_index-1];
        curve_point_index=1;
    }
    else{
        point_difference = curve_vertices[curve_point_index]-curve_vertices[curve_point_index-1];
        curve_point_index++;
    }
    if(point_difference[0]!=0||point_difference[1]!=0|point_difference[2]!=0){
        initial->setC(initial->getC()*glm::translate(glm::mat4(1.0f), point_difference));
    }
}

void Window::displayCallback(GLFWwindow* window)
{
	// Switch between OpenGL rasterizer and your rasterizer
	//if (glRasterize) {
		// Switch back to using OpenGL's rasterizer
		glUseProgram(program);
		// Clear the color and depth buffers.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Specify the values of the uniform variables we are going to use.
		/*
		 * TODO: Section 3 and 4: Modify the code here to draw both the bunny and
		 * the dragon
		 * Note that the model matrix sent to the shader belongs only
		 * to what object the currentObj ptr is pointing to. You will need to
		 * use another call to glUniformMatrix4fv to change the model matrix
		 * data being sent to the vertex shader before you draw the other object
		 */
    glm::mat4 model = cube->getModel();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3fv(colorLoc,1,glm::value_ptr(glm::vec3(1,0,0)));
    glUniform1f(camera_switch, 0);
    cube->draw();
    glUniform1f(camera_switch,1);
    root_transform->draw(glm::mat4(1.0f));
    glUniform1f(camera_switch,2);
    cube->drawline();
    
    //currentObj->draw();
   // model = glm::scale(model,currentObj->getScaleTrnasformation());
   // model = glm::translate(model,currentObj->getCenterTransformation());
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
   // glUniform3fv(colorLoc, 1, glm::value_ptr(color));
    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Section 4: Modify below to add your key callbacks.
	 */
    glm::mat4 zoom_in;
    glm::mat4 zoom_out;
	 // Check for a key press.
	if (action == GLFW_PRESS)
	{
		// Uppercase key presses (shift held down + key press)
		if (mods == GLFW_MOD_SHIFT) {
			switch (key) {
                /*case GLFW_KEY_P:
                    bunny->increase_point_size();
                    dragon->increase_point_size();
                break;*/
                case GLFW_KEY_Z:
                    /*eye[2] = eye[2]+1;
                    view = glm::lookAt(eye, center, up);
                    cube->setModel(glm::translate(cube->getModel(), glm::vec3(0,0,1)));*/
                    zoom_out = glm::scale(glm::mat4(1.0f),glm::vec3(1.5,1.5,1.5));
                    initial->setC(initial->getC()*zoom_out);
                break;
                case GLFW_KEY_S:
                    currentObj->scale_up();
                    break;
                /*case GLFW_KEY_R:
                    bunny->reset_shape();
                    dragon->reset_shape();*/
			default:
				break;
			}
		}
        else{
		// Deals with lowercase key presses
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_F1:
			// Set currentObj to cube
			currentObj = bunny;
                object_pointer = 1;
			break;
		case GLFW_KEY_F2:
			// Set currentObj to cubePoints
			currentObj = dragon;
                object_pointer = 2;
			break;
        case GLFW_KEY_F3:
            currentObj = bear;
                object_pointer = 3;
            break;
        case GLFW_KEY_Z:
            /*eye[2] = eye[2]-1;
            view = glm::lookAt(eye, center, up);
            cube->setModel(glm::translate(cube->getModel(), glm::vec3(0,0,-2)));*/
            zoom_in = glm::scale(glm::mat4(1.0f),glm::vec3(0.5,0.5,0.5));
            initial->setC(initial->getC()*zoom_in);
            break;
        case GLFW_KEY_S:
                currentObj->scale_down();
                break;
        case GLFW_KEY_R:
                //currentObj->reset_whole();
                view = origin_view;
                //initial->setC(initial_translation);
                for(int i = 0; i<rotate_trans_list.size(); i++){
                    rotate_trans_list[i]->setC(glm::mat4(1.0f));
                }
                break;
            case GLFW_KEY_N:
                if(change_light==0){
                    change_light=1;
                }
                else{
                    change_light = 0;
                }
                break;
            case GLFW_KEY_3:
                if(dirLight_switch==0){
                    printf("dirlight on\n");
                    dirLight_switch = 1;
                }
                else{
                    printf("dirlight off\n");
                    dirLight_switch = 0;
                }
                break;
            case GLFW_KEY_0:
                if(object_switch==0){
                    object_switch = 1;
                }
                else{
                    object_switch = 0;
                }
                break;
            case GLFW_KEY_1:
                printf("Dirlight only\n");
                key_press = 1;
                break;
            case GLFW_KEY_2:
                printf("Dirlight+pointlight\n");
                key_press = 2;
                break;
		default:
			break;
		}
        }
	}
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
    if(action==GLFW_PRESS){
        switch(button){
            case GLFW_MOUSE_BUTTON_LEFT:{
                left_press = 1;
                glfwGetCursorPos(window, &cur_x, &cur_y);
                ball_point = trackBallMapping(glm::vec3(cur_x,cur_y,0));
                break;}
            case GLFW_MOUSE_BUTTON_RIGHT:
                right_press = 1;
                glfwGetCursorPos(window, &cur_x, &cur_y);
                break;
            default:
                break;
        }
    }
    else{
        trackball_rotation = glm::mat4(1.0f);
        left_press=0;
        right_press=0;
    }
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    glfwGetCursorPos(window, &xpos, &ypos);
    if(object_switch==0){
        
    if(right_press==1){
        double x_dif,y_dif;
        x_dif = xpos-cur_x;
        y_dif = ypos-cur_y;
        cur_x = xpos;
        cur_y = ypos;
        currentObj->xyTranslate(x_dif, y_dif);
    }
    if(left_press==1){
        glm::vec3 coor = glm::vec3(xpos,ypos,0);
        glm::vec3 result = trackBallMapping(coor);
        glm::vec3 point_dif = result-ball_point;
        glm::vec3 cross_product = glm::cross(result,ball_point);
        ball_point = result;
        float velocity = glm::length(point_dif);
        if(velocity>0.0001){
            velocity = velocity*1000;
            cross_product[0] = -cross_product[0];
            cross_product[1] = -cross_product[1];
            cross_product[2] = -cross_product[2];
            //currentObj->xyzRotate(velocity, cross_product);
            view = view * glm::rotate(glm::mat4(1.0f),glm::radians(velocity),cross_product);
            
        }
    }
        
    }
    else{
        if(left_press==1){
            glm::vec3 coor = glm::vec3(xpos,ypos,0);
            glm::vec3 result = trackBallMapping(coor);
            glm::vec3 point_dif = result-ball_point;
            glm::vec3 cross_product = glm::cross(result,ball_point);
            ball_point = result;
            float velocity = glm::length(point_dif);
            if(velocity>0.0001){
                velocity = velocity*1000;
                cross_product[0] = -cross_product[0];
                cross_product[1] = -cross_product[1];
                cross_product[2] = -cross_product[2];
               /* glm::mat4 back = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,13));
                glm::mat4 rotation = glm::rotate(glm::mat4(1.0f),glm::radians(velocity),cross_product);
                glm::mat4 ahead =glm::translate(glm::mat4(1.0f), glm::vec3(0,0,-13));
                rotation = ahead * rotation * back;
                glm::mat4 copy_model = robot_limb->getModel();
                copy_model = copy_model*rotation;*/
                trackball_rotation = glm::rotate(glm::mat4(1.0f),glm::radians(velocity),cross_product);
                //initial->setC(initial->getC()*trackball_rotation);
                for(int i = 0;i<rotate_trans_list.size();i++){
                    rotate_trans_list[i]->setC(rotate_trans_list[i]->getC()*trackball_rotation);
                }
                //robot_limb->setModel(copy_model);
                //currentObj->xyzRotate(velocity, cross_product);
            }
        }
    }
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    //currentObj->zTranslate(yoffset);
    view = glm::translate(view, glm::vec3(0,0,yoffset));
}

glm::vec3 Window::trackBallMapping(glm::vec3 point){
    glm::vec3 v;
    float d;
    v[0]=(2.0f*point[0]-width/2) / width/2;
    v[1]=(height/2-2.0f*point[1]) / height/2;
    v[2]=0;
    d = glm::length(v);
    d = (d<1.0f) ? d: 1.0f;
    v[2]=sqrtf(1.001 - d*d);
    v = normalize(v);
    return v;
}
