#version 330 core
 
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords; // texture test

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

//out vec3 fragNormal;
//out vec3 fragPosition; // Output to fragment shader
out vec2 TexCoords; // Pass texture coordinates to the fragment shader

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
    //fragNormal = mat3(transpose(inverse(model))) * normal; // Properly transform normals if non-uniform scaling is present
    //fragPosition = vec3(model * vec4(position, 1.0)); // Transform vertex position to world space
    TexCoords = texCoords;
}

