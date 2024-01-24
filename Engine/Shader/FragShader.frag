#version 330
out vec4 FragColor;

in vec3 outColor;
in vec2 texCoord;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

void main()
{
    FragColor = mix(texture(ourTexture, texCoord), texture(ourTexture2, texCoord), 0.2);
}