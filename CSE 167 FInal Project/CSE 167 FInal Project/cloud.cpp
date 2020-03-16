#include "cloud.h"
#include <stdio.h>

Cloud::Cloud()
{
    // set up the noise for one tile
    setupNoise(map32);
}

Cloud::~Cloud()
{
    
}

// random generate the cloud by implementing perlin noise
float Cloud::noise(int x, int y, int random)
{
    int n = x + y * 57 + random * 131;
    n = (n<<13) ^ n;
    return (1.0f - ( (n * (n * n * 15731 + 789221) +
            1376312589)&0x7fffffff)* 0.000000000931322574615478515625f);
}

// go through the map and smooth every single point in 2D
void Cloud::setupNoise(float *map)
{
    float temp[34][34];
    int random=rand() % 5000;

    // set the color to each position
    for (int y = 1; y < 33; y++)
    {
        for (int x = 1; x < 33; x++)
        {
            temp[x][y] = 128.0f + noise(x, y, random) * 128.0f;
        }
    }
    
    // eliminate the border
    for (int i = 1; i < 33; i++)
     {
           temp[0][i] = temp[32][i];
           temp[33][i] = temp[1][i];
           temp[i][0] = temp[i][32];
           temp[i][33] = temp[i][1];
     }
     temp[0][0] = temp[32][32];
     temp[33][33] = temp[1][1];
     temp[0][33] = temp[32][1];
     temp[33][0] = temp[1][32];

    // smooth the tile to make the cloud more realistic by doing covolution
    for (int y = 1; y < 33; y++)
        for (int x = 1; x < 33; x++)
        {
            float center = temp[x][y] / 4.0f;
            float sides = (temp[x + 1][y] + temp[x - 1][y] + temp[x][y + 1] + temp[x][y - 1]) / 8.0f;
            float corners = (temp[x + 1][y + 1] + temp[x + 1][y - 1] + temp[x - 1][y + 1] + temp[x - 1][y - 1]) / 16.0f;

            map[((x - 1) * 32) + (y - 1)] = center + sides + corners;
        }
}

// further smooth the curve to make the cloud realistic
float Cloud::interpolate(float x, float y, float *map)
{
    // do a linear interpolate on edge
    int Xint = (int)x;
    int Yint = (int)y;

    float Xfrac = x - Xint;
    float Yfrac = y - Yint;
    
    int X0 = Xint % 32;
    int Y0 = Yint % 32;
    int X1 = (Xint + 1) % 32;
    int Y1 = (Yint + 1) % 32;
    
    float bot = map[X0*32 + Y0] + Xfrac * (map[X1*32 + Y0] - map[X0*32 + Y0]);
    float top = map[X0*32 + Y1] + Xfrac * (map[X1*32 +  Y1] - map[X0*32 + Y1]);

    return (bot + Yfrac * (top - bot));
}

// control the number of plasma
void Cloud::octave(float *map32, float *map256)
{
    // clear the final texture mapping previously we have
    for (int x=0; x<256*256; x++)
    {
        map256[x] = 0;
    }
    
    for (int x=0; x<256; x++)
        for (int y=0; y<256; y++)
        {
//            float scale = 1 / pow(2, 3-octave);
//            float noise = interpolate(x*scale, y*scale , map32);
            float noise = interpolate(x, y, map32);
//            map256[(y*256) + x] += noise / pow(2, octave);
            map256[(y*256) + x] += noise;
        }
    // overlap several octave to make the cloud more various
//    for (int octave=0; octave<4; octave++)
//        for (int x=0; x<256; x++)
//            for (int y=0; y<256; y++)
//            {
//                float scale = 1 / pow(2, 3-octave);
//                float noise = interpolate(x*scale, y*scale , map32);
//                map256[(y*256) + x] += noise / pow(2, octave);
//            }
}

// control the sharpness of the cloud
void Cloud::filter(float *map)
{
    // define the default sharpness
    // more fuzzy when value closer to 1.0
    float sharpness = 0.9f;
    
    // define what portion of sky should be covered
    float cloudCover = 30;
    
    // loop through the map
    for (int x=0; x<256*256; x++)
    {
        float c = map[x] - (255.0f - cloudCover);
        if (c<0)     c = 0;
        map[x] = 255.0f - ((float)(pow(sharpness, c))*255.0f);
    }
}

// draw the cloud by setting up the texture
void Cloud::draw(GLuint shaderProgram)
{
    // set up texture
    octave(map32, map256);
    //filter(map256);
    
     // set up RGBA value
    std::vector<unsigned char> texture(256 * 256 * 4);
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            float color = map256[i * 256 + j];
            texture.at((i + (j * 256)) * 4) = color;
            texture.at((i + (j * 256)) * 4 + 1) = color;
            texture.at((i + (j * 256)) * 4 + 2) = color;
            texture.at((i + (j * 256)) * 4 + 3) = 0;
            std::cout << color << std::endl;
        }
    }
    

    
    // generate texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // using the value in temp texture, generate bind cloud to upper side of cube
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, &texture[0]);
    
    // set up wraping and filter specifics
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
