// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
// struct for material
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube Skybox;
//uniform sampler2D Cloud;

void main()
{
    FragColor = texture(Skybox, TexCoords);
}

//in vec3  TexCoords;
//
//// Following for paiting cloud
//in vec3 pos;
////in vec3 fsun;
//uniform float time = 0.0;
//uniform float cirrus = 0.2;
//uniform float cumulus = 0.9;
//uniform samplerCube skybox;
//// uniform int showCloud;
//
//const float Br = 0.0005;
//const float Bm = 0.0003;
//const float g =  0.9200;
//const vec3 nitrogen = vec3(0.650, 0.570, 0.475);
//const vec3 Kr = Br / pow(nitrogen, vec3(4.0));
//const vec3 Km = Bm / pow(nitrogen, vec3(0.84));
//
//float hash(float n);
//float noise(vec3 x);
//float fbm(vec3 p);
//const mat3 m = mat3(0.0, 1.60,  1.20, -1.6, 0.72, -0.96, -1.2, -0.96, 1.28);
//
//out vec4 FragColor;
//
//uniform samplerCube Skybox;
////uniform sampler2D Cloud;
//
//float hash(float n)
//{
//  return fract(sin(n) * 43758.5453123);
//}
//
//float noise(vec3 x)
//{
//  vec3 f = fract(x);
//  float n = dot(floor(x), vec3(1.0, 157.0, 113.0));
//  return mix(mix(mix(hash(n +   0.0), hash(n +   1.0), f.x),
//                 mix(hash(n + 157.0), hash(n + 158.0), f.x), f.y),
//             mix(mix(hash(n + 113.0), hash(n + 114.0), f.x),
//                 mix(hash(n + 270.0), hash(n + 271.0), f.x), f.y), f.z);
//}
//
//float fbm(vec3 p)
//{
//  float f = 0.0;
//  f += noise(p) / 2; p = m * p * 1.1;
//  f += noise(p) / 4; p = m * p * 1.2;
//  f += noise(p) / 6; p = m * p * 1.3;
//  f += noise(p) / 12; p = m * p * 1.4;
//  f += noise(p) / 24;
//  return f;
//}
//
//void main()
//{
//    // map skybox
//    FragColor = texture(Skybox, TexCoords);
//
//    // get the cloud texture color
//    vec4 color;
//
//    // Atmosphere Scattering
//    vec3 fsun = vec3(0.0, 0.5, 0.5);
//    float mu = 1.0;
//    vec3 normPos = normalize(pos);
//    float posy = min(max (1.5 * normPos.y, 0), 1);
//    float fsuny = 0.0;
//    //vec3 extinction = 0.9 * mix(exp(-exp(-((posy + fsuny * 4.0) * (exp(-posy * 16.0) + 0.1) / 80.0) / Br) * (exp(-posy * 16.0) + 0.1) * Kr / Br) * exp(-posy * exp(-posy * 8.0 ) * 4.0) * exp(-posy * 2.0) * 4.0, vec3(1.0 - exp(fsuny)) * 0.2, -fsuny * 0.2 + 0.5);
//    //pos = normalize(pos);
//    // exponential filter
//    vec3 extinction = mix(exp(-exp(-((pos.y + fsun.y * 4.0) * (exp(-pos.y * 16.0) + 0.1) / 80.0) / Br) * (exp(-pos.y * 16.0) + 0.1) * Kr / Br) * exp(-pos.y * exp(-pos.y * 8.0 ) * 4.0) * exp(-pos.y * 2.0) * 4.0, vec3(1.0 - exp(fsun.y)) * 0.2, -fsun.y * 0.2 + 0.5);
//    color.rgb = 3.0 / (8.0 * 3.14) * (1.0 + mu * mu) * (Kr + Km * (1.0 - g * g) / (2.0 + g * g) / pow(1.0 + g * g - 2.0 * g * mu, 1.5)) / (Br + Bm) * extinction;
//
//    // Cirrus Clouds
//    float density = smoothstep(1.0 - cirrus, 1.0, fbm(pos.xyz / pos.y * 2.0 + time * 0.05)) * 0.3;
//    color.rgb = mix(color.rgb, extinction * 4.0, density * max(pos.y, 0.0));
//
//    // Cumulus Clouds
//    for (int i = 0; i < 3; i++)
//    {
//      float density = smoothstep(1.0 - cumulus, 1.0, fbm((0.7 + float(i) * 0.01) * pos.xyz / pos.y + time * 0.3));
//      color.rgb = mix(color.rgb, extinction * density * 5.0, min(density, 1.0) * max(pos.y, 0.0));
//    }
//
//    // Dithering Noise
//    color.rgb += noise(pos * 1000) * 0.01;
//    color.w = FragColor.w;
//
//    //color = vec4(-255, -255, -255, FragColor.w);
//    // mix two color together
//    // Caculate ratio for cloud and skybox
//    //vec3 normPos = normalize(pos);
//    float cloudRatio = 0.0;
//    // Use sigmoid to calculate ratio
//    cloudRatio = exp(10 * normPos.y) / (1 + exp(10 * normPos.y));
//    // cloudRatio = (3 * length (cloudColor)) / (3 * length(cloudColor) + length(fragColor));
//    FragColor = color * cloudRatio + (1-cloudRatio) * FragColor;
//}
