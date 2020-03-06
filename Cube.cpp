#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Cube.h"


Cube::Cube(float size,std::vector<std::string> texture_list)
{
	// Model matrix. Since the original size of the cube is 2, in order to
	// have a cube of some size, we need to scale the cube by size / 2.
	model = glm::scale(glm::vec3(size / 2.f));

	// The color of the cube. Try setting it to something else!
	color = glm::vec3(1.0f, 0.95f, 0.1f);

	/*
	 * Cube indices used below.
	 *    4----7
	 *   /|   /|
	 *  0-+--3 |
	 *  | 5--+-6
	 *  |/   |/
	 *  1----2
	 *
	 */

	 // The 8 vertices of a cube.
	/*std::vector<glm::vec3> test_vertices
	{
		glm::vec3(-1, 1, 1),
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(-1, 1, -1),
		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(1, 1, -1)
	};*/
    std::vector<float> vertices_p{
        0.0, 0.0, -30,
        5, 0.0, -30,
        
        5, 0, -30,
        5, 5, -30,
        
        5, 5, -30,
        0, 5, -30,
        
        0.0, 5, -30,
        -5, 5, -30,
        
        -5, 5, -30,
        -5, 0.0, -30,
        
        -5, 0, -30,
        5, 0, -30,
    };
    glm::vec3 p0(0,-10,-30);
    glm::vec3 p1(10,-10,-30);
    glm::vec3 p2(10,10,-30);
    glm::vec3 p3(0,10,-30);
    float t = 0;
    add_a_curve(t, p0, p1, p2, p3);
    p0=glm::vec3(-0,10,-30);
    p1=glm::vec3(-6,10,-30);
    p2=glm::vec3(-8,10,-30);
    p3=glm::vec3(-10,10,-30);
    add_a_curve(t, p0, p1, p2, p3);
    p0=glm::vec3(-10,10,-30);
    p1=glm::vec3(-15,5,-40);
    p2=glm::vec3(-7,5,-40);
    p3=glm::vec3(-15,-10,-40);
    add_a_curve(t, p0, p1, p2, p3);
    p0=glm::vec3(-15,-10,-40);
    p1=glm::vec3(-5,-5,-100);
    p2=glm::vec3(0,0,-100);
    p3=glm::vec3(5,-10,-30);
    add_a_curve(t, p0, p1, p2, p3);
    p0=glm::vec3(5,-10,-30);
    p1=glm::vec3(7,0,-30);
    p2=glm::vec3(5,10,-30);
    p3=glm::vec3(0,-20,-30);
    add_a_curve(t, p0, p1, p2, p3);
    p0=glm::vec3(0,-20,-30);
    p1=glm::vec3(1,-30,-50);
    p2=glm::vec3(-30,-15,-20);
    p3=glm::vec3(0,-10,-30);
    add_a_curve(t, p0, p1, p2, p3);
    
    
    
    float vertices[] = {
         // positions
         -1.0f,  1.0f, -1.0f,
         -1.0f, -1.0f, -1.0f,
          1.0f, -1.0f, -1.0f,
          1.0f, -1.0f, -1.0f,
          1.0f,  1.0f, -1.0f,
         -1.0f,  1.0f, -1.0f,

         -1.0f, -1.0f,  1.0f,
         -1.0f, -1.0f, -1.0f,
         -1.0f,  1.0f, -1.0f,
         -1.0f,  1.0f, -1.0f,
         -1.0f,  1.0f,  1.0f,
         -1.0f, -1.0f,  1.0f,

          1.0f, -1.0f, -1.0f,
          1.0f, -1.0f,  1.0f,
          1.0f,  1.0f,  1.0f,
          1.0f,  1.0f,  1.0f,
          1.0f,  1.0f, -1.0f,
          1.0f, -1.0f, -1.0f,

         -1.0f, -1.0f,  1.0f,
         -1.0f,  1.0f,  1.0f,
          1.0f,  1.0f,  1.0f,
          1.0f,  1.0f,  1.0f,
          1.0f, -1.0f,  1.0f,
         -1.0f, -1.0f,  1.0f,

         -1.0f,  1.0f, -1.0f,
          1.0f,  1.0f, -1.0f,
          1.0f,  1.0f,  1.0f,
          1.0f,  1.0f,  1.0f,
         -1.0f,  1.0f,  1.0f,
         -1.0f,  1.0f, -1.0f,

         -1.0f, -1.0f, -1.0f,
         -1.0f, -1.0f,  1.0f,
          1.0f, -1.0f, -1.0f,
          1.0f, -1.0f, -1.0f,
         -1.0f, -1.0f,  1.0f,
          1.0f, -1.0f,  1.0f
     };


	// Each ivec3(v1, v2, v3) define a triangle consists of vertices v1, v2 
	// and v3 in counter-clockwise order.
	std::vector<glm::ivec3> indices
	{
		// Front face.
		glm::ivec3(0, 1, 2),
		glm::ivec3(2, 3, 0),
		// Back face.
		glm::ivec3(7, 6, 5),
		glm::ivec3(5, 4, 7),
		// Right face.
		glm::ivec3(3, 2, 6),
		glm::ivec3(6, 7, 3),
		// Left face.
		glm::ivec3(4, 5, 1),
		glm::ivec3(1, 0, 4),
		// Top face.
		glm::ivec3(4, 0, 3),
		glm::ivec3(3, 7, 4),
		// Bottom face.
		glm::ivec3(1, 5, 6),
		glm::ivec3(6, 2, 1),
	};

	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	/*glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbos);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the vertices.
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
		vertices.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind to the second VBO. We will use it to store the indices.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
	// Pass in the data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(),
		indices.data(), GL_STATIC_DRAW);

	// Unbind from the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);*/
    glGenVertexArrays(1, &skyboxVAO);
    glGenTextures(1, &textureID);
    glGenBuffers(1, &vbo);
    int width, height, nrChannels;
    for(int i = 0; i<texture_list.size();i++){
        unsigned char *data = stbi_load(texture_list[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << texture_list[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo2);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices_position.size(), vertices_position.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
}

Cube::~Cube()
{
	// Delete the VBOs and the VAO.
	glDeleteBuffers(2, vbos);
	glDeleteVertexArrays(1, &vao);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &vbo2);
}

void Cube::draw()
{
	// Bind to the VAO.
	//glBindVertexArray(skyboxVAO);
	// Draw triangles using the indices in the second VBO, which is an 
	// elemnt array buffer.
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	// Unbind from the VAO.
	//glBindVertexArray(0);
    glDepthMask(GL_FALSE);
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
}

void Cube::drawline(){
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, vertices_position.size()/3);
    glBindVertexArray(0);
}

void Cube::update()
{
	// Spin the cube by 1 degree.
	//spin(0.1f);
}

void Cube::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	model = glm::rotate(glm::mat4(1), glm::radians(deg), 
		glm::vec3(0.0f, 1.0f, 0.0f)) * model;
}




