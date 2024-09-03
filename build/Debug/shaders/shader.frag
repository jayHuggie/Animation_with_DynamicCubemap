#version 330 core

//uniform samplerCube skybox; // The cubemap texture for static reflections
uniform sampler2D sphereTexture;

/* 
struct Material {
    vec3 ambient;   
    vec3 diffuse;   
    vec3 specular;  
    float shininess;
};

// Pass the material properties for the object as a uniform
uniform Material material;

in vec3 fragNormal;
in vec3 fragPosition;

// Light properties
uniform vec3 LightPositions[2];
uniform vec3 LightColors[2];
uniform vec3 CameraPosition;
*/

in vec2 TexCoords; // Make sure this matches the name used in vertex shader

out vec4 FragColor;



void main() {
    /* 
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(CameraPosition - fragPosition);
    vec3 result = material.ambient; // Use the material's ambient color

    // Reflect the view direction around the normal
    vec3 reflectDir = reflect(-viewDir, norm);
    // Sample the environment map
    vec3 environmentColor = texture(skybox, reflectDir).rgb;

    for(int i = 0; i < 2; i++) {
        vec3 lightDir = normalize(LightPositions[i] - fragPosition);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = material.diffuse * diff * LightColors[i];
        
        float spec = pow(max(dot(viewDir, reflect(-lightDir, norm)), 0.0), material.shininess);
        vec3 specular = material.specular * spec * LightColors[i];

        result += diffuse + specular;
    }
    
    // Mix the material color with the environment color based on the material's reflectivity
    vec3 finalColor = mix(result, environmentColor, 0.3); // Adjust reflectivity ratio as needed
    fragColor = vec4(finalColor, 1.0);
    */

    FragColor = texture(sphereTexture, TexCoords);
}
