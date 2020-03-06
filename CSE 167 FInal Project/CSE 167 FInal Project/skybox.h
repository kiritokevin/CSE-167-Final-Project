#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


class skybox
{
private:
    GLuint vao;
    GLuint vbos[2];
public:
    // hard coded skybox images' names
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 model;
    std::vector<std::string> textureName = {
        "/Users/KZ/Desktop/CSE 167/CSE 167 HW 0/CSE 167 HW 0/skybox/right.jpg",
        "/Users/KZ/Desktop/CSE 167/CSE 167 HW 0/CSE 167 HW 0/skybox/left.jpg",
        "/Users/KZ/Desktop/CSE 167/CSE 167 HW 0/CSE 167 HW 0/skybox/top.jpg",
        "/Users/KZ/Desktop/CSE 167/CSE 167 HW 0/CSE 167 HW 0/skybox/base.jpg",
        "/Users/KZ/Desktop/CSE 167/CSE 167 HW 0/CSE 167 HW 0/skybox/front.jpg",
        "/Users/KZ/Desktop/CSE 167/CSE 167 HW 0/CSE 167 HW 0/skybox/back.jpg"
    };
    unsigned int textureID;
    
    skybox(float size, glm::mat4 View, glm::mat4 Projection);
    ~skybox();

    void draw(GLuint shaderProg, glm::mat4 View);
    // void update(glm::mat4 C);
    void loadTexture(std::vector<std::string> faces);
};


#endif /* skybox_h */
