#version 330 core

out vec4 FragColor;

in vec3 FragNormal;
in vec3 FragPosition;

uniform samplerCube skybox;  // Cubemap texture
uniform vec3 viewPos;

void main() {
    // Environment reflection vector
    vec3 I = normalize(viewPos - FragPosition); 
    vec3 R = reflect(I, normalize(FragNormal));

    //vec3 I = normalize(FragPosition - viewPos);  // This represents the incident vector
    //vec3 R = reflect(I, normalize(FragNormal));  // Reflect the incident vector around the normal


    // Adjust the y-component of the reflection vector to correct vertical flip
    //R.y = -R.y; // Negate the y-component
    //R.x = -R.x;
    //R.z = -R.z;
    vec3 envColor = texture(skybox, R).rgb;

    // Blend the environment color with the object color for reflectivity
    float reflectivity = 1.0; // Adjust for desired reflectivity
    vec3 objectColor = vec3(0.0, 0.5, 0.31); // Example object color
    vec3 result = mix(objectColor, envColor, reflectivity);

    FragColor = vec4(result, 1.0);
}

