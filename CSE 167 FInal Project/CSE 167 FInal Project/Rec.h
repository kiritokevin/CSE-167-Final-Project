#ifndef _REC_H_
#define _REC_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Object.h"

class Rec
{
private:
    glm::mat4 model;
    GLuint vao;
    GLuint vbos[2];
public:
    //Rec(float size, glm::vec3 min, glm::vec3 max);
    float rec_length;
    float rec_height;
    float rec_width;
    glm::vec3 rec_max;
    Rec(glm::vec3 start_point,float length,float height,float width);
    ~Rec();

    void draw(GLuint shaderProgram, glm::mat4 View, glm::mat4 Projection,glm::vec3 Color,bool check_collision);
    void update(int num);
    void spin(float deg);
    void move(int direction);
};

#endif
