#ifndef _CLOUD_H_
#define _CLOUD_H_

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
#include <iostream>
#include <string>
#include <math.h>
#include "stb_image.h"
#include "Object.h"


class Cloud
{
public:
    GLuint vao;
    GLuint vbos[2];
    
    float map32[32 * 32];
    float map256[256 * 256];
    
    unsigned int textureID;
    
    // constructor
    Cloud();
    ~Cloud();
    void draw(GLuint shaderProgram);
    float noise(int x, int y, int random);
    
    // assign color to noise map
    void setupNoise(float* map);
    
    // smooth
    float interpolate(float x, float y, float* map);
    
    // adjust frequency
    void octave(float* map32, float* map256);
    
    // filter the cloud
    void filter(float* map);
    
};

#endif
