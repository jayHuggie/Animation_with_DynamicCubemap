#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal; // Ensure normals are provided in the VAO
layout (location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragNormal;   // Pass normals to fragment shader
out vec3 FragPosition;  // Pass fragment position to fragment shader
out vec2 TexCoords;

void main()
{
    FragPosition = vec3(model * vec4(position, 1.0));  // Convert position to world coordinates
    FragNormal = mat3(transpose(inverse(model))) * normal;  // Transform normals
    TexCoords = texCoords;
    gl_Position = projection * view * vec4(FragPosition, 1.0);
}

/* 
#version 330 core

layout (location = 0) in vec2 aPos; // Fullscreen quad vertices

void main() {
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); // Directly use provided quad vertices
}
*/