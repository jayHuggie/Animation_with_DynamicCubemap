#version 330 core
out vec4 FragColor;

uniform samplerCube cubeMap;
uniform int faceIndex;

void main() {
    vec3 dir;
    switch(faceIndex) {
        case 0: dir = vec3(1, 0, 0); break; // +X
        case 1: dir = vec3(-1, 0, 0); break; // -X
        case 2: dir = vec3(0, 1, 0); break; // +Y
        case 3: dir = vec3(0, -1, 0); break; // -Y
        case 4: dir = vec3(0, 0, 1); break; // +Z
        case 5: dir = vec3(0, 0, -1); break; // -Z
        default: dir = vec3(0, 0, 0); break; // Fallback, should not happen
    }
    FragColor = texture(cubeMap, dir);
}
