//
//  geometry.hpp
//  CSE 167 HW 0
//
//  Created by 曾啸洋 on 2020/2/21.
//  Copyright © 2020 曾啸洋. All rights reserved.
//

#ifndef geometry_h
#define geometry_h
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
#include <list>
#include <stdio.h>


class Geometry
{
private:
    GLuint vao, vbo[2], ebo;
public:
    unsigned int textureID;
    glm::mat4 model;
    glm::vec3 midPoint;
    glm::mat4 view;
    glm::mat4 projection;
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> normal;
    std::vector<glm::ivec3> faces;
    std::vector<std::string> textureName = {
        "/Users/KZ/CSE-167-Final-Project/CSE 167 FInal Project/CSE 167 FInal Project/skybox/base.jpg"
    };
    Geometry(std::string objFilename);
    ~Geometry();
    void draw(GLuint shaderProgram, glm::mat4 View, glm::mat4 Projection, glm::mat4 C);
    void update(glm::mat4 C);
    void loadTexture(std::vector<std::string> faces);
};
#endif /* geometry_hpp */
