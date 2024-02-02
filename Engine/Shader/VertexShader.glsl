#version 330
layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec3 aColor;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

//out vec3 outColor;
out vec2 texCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    //outColor = aColor;
    texCoord = aTexCoord;
    FragPos = vec3(model * vec4(aPos.x, aPos.y, aPos.z, 1.0));
    Normal = aNormal;
}