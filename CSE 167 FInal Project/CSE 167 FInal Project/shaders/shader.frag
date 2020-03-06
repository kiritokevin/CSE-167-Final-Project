#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
// struct for material
//struct Material {
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//    float shininess;
//};
//
//struct directionLight
//{
//    vec3 lightColor;
//    vec3 direction;
//};
//
//struct pointLight
//{
//    vec3 pointColor;
//    vec3 pointPosition;
//};
//
//// uniform variable connect to cpp file
//uniform Material material;
//uniform directionLight direction;
//uniform pointLight point;
//uniform int renderingMode;
//uniform int objectCode;
//uniform int lightScene;
//uniform vec3 viewPos;
//
////uniform vec3 color;
//in float sampleExtraOutput;
//in vec3 FragPos;
//in vec3 aNormal;
////in vec3 surface_normal;
//in vec3 new_normal;
//
//// You can output many things. The first vec4 type output determines the color of the fragment
//out vec4 fragColor;
//
//void main()
//{
//    // normal color
//    if(renderingMode == 1)
//    {
//         //Use the color passed in. An alpha of 1.0f means it is not transparent.
//        fragColor = vec4(new_normal, sampleExtraOutput);
//    }
//    // phong color
//    else
//    {
//        vec3 result;
//        vec3 ambient;
//        vec3 diffuse;
//        vec3 specular;
//        vec3 pointLightDir;
//        vec3 pointSpecular;
//        vec3 pointReflectDir;
//        float pointSpec;
//        float attenuation;
//        float attenuationDiff;
//        // direction ambient
//        float ambientStrength = 0.1;
//        vec3 directionAmbient = ambientStrength * material.ambient;
//
//        // direction diffuse
//        vec3 surfaceNormal = normalize(aNormal);
//        vec3 lightDir = normalize(direction.direction);
//        float diff = max(dot(surfaceNormal, lightDir), 0.0);
//        vec3 directionDiffuse = direction.lightColor * (material.diffuse * diff);
//
//        // direction specular
//        float specularStrength = 1.0;
//        vec3 viewDir = normalize(viewPos - FragPos);
//        vec3 reflectDir = reflect(-lightDir, surfaceNormal);
//        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//        vec3 directionSpecular = specularStrength * spec * material.specular * direction.lightColor;
//
//        // point light
//        // point ambient
//        vec3 pointAmbient = ambientStrength * material.ambient;
//
//        // point diffuse
//        pointLightDir = normalize(point.pointPosition - FragPos);
//        float pointDiff = max(dot(surfaceNormal, pointLightDir), 0.0);
//        vec3 pointDiffuse = pointDiff * material.diffuse * point.pointColor;
//
//        // point specular
//        pointReflectDir = reflect(-pointLightDir, surfaceNormal);
//        pointSpec = pow(max(dot(viewDir, pointReflectDir), 0.0), material.shininess);
//        pointSpecular = specularStrength * pointSpec * material.specular * point.pointColor;
//
//        // attenuation
//        attenuationDiff = length(point.pointPosition - FragPos);
//        attenuation = 1/attenuationDiff;
//
//        // light scenes
//        if(lightScene == 1)
//        {
//            ambient = directionAmbient;
//            diffuse = directionDiffuse;
//            specular = directionSpecular;
//        }
//        else if(lightScene == 2)
//        {
//            ambient = directionAmbient + attenuation * pointAmbient;
//            diffuse = directionDiffuse + attenuation * pointDiffuse;
//            specular = directionSpecular + attenuation * pointSpecular;
//        }
//        else if(lightScene == 3)
//        {
//            ambient = attenuation * pointAmbient;
//            diffuse = attenuation * pointDiffuse;
//            specular = attenuation * pointSpecular;
//        }
//        else
//        {
//            ambient = material.ambient;
//            diffuse = vec3(0.0f);
//            specular = vec3(0.0f);
//        }
//
//        // object material differences
//        if(objectCode == 1)
//        {
//            result = (ambient + specular);
//        }
//        else if(objectCode == 2)
//        {
//            result = (ambient + diffuse);
//        }
//        else
//        {
//            result = (ambient + specular + diffuse);
//        }
//        // paint the sphere
//        if (objectCode == 4)
//        {
//            fragColor = vec4(vec3(point.pointColor), sampleExtraOutput);
//        }
//        else
//        {
//            fragColor = vec4(result, sampleExtraOutput);
//        }
//
//    }
//}
in float sampleExtraOutput;

uniform vec3 color;
in vec3 new_normal;
// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    fragColor = vec4(new_normal, sampleExtraOutput);
}
