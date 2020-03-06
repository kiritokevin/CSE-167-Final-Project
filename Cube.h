#ifndef _CUBE_H_
#define _CUBE_H_

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>

#include "Object.h"
using namespace std;

class Cube : public Object
{
private:
	GLuint vao;
	GLuint vbos[2];
    GLuint skyboxVAO;
    GLuint vbo;
    GLuint vbo2;
    GLuint textureID;
    std::vector<float> vertices_position;
    std::vector<glm::vec3> vertices_list;
public:
    Cube(float size,std::vector<std::string> texture_list);
	~Cube();

	void draw();
	void update();
    void drawline();

	void spin(float deg);
    std::vector<float> get_vertices(){return vertices_position;}
    std::vector<glm::vec3> get_list(){return vertices_list;}
    void add(glm::vec3 point){
        vertices_position.push_back(point[0]);
        vertices_position.push_back(point[1]);
        vertices_position.push_back(point[2]);
        vertices_list.push_back(point);
    }
    void add_a_curve(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3){
        add(p0);
        while(t<=1){
            glm::vec3 new_point = Lerp(t, p0, p1, p2, p3);
            add(new_point);
            add(new_point);
            t=t+0.0067;
        }
        add(p3);
    }
};

#endif

