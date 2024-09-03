#version 330 core
layout (location = 0) in vec3 aPos;    // Vertex position
layout (location = 1) in vec3 aNormal; // Vertex normal (not used in this shader but kept for consistency)
layout (location = 2) in vec2 aTexCoord; // Texture coordinate

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords; // Pass texture coordinate to fragment shader

void main()
{
    TexCoords = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
