#version 330 core

out vec4 FragColor;

in vec3 FragNormal;
in vec3 FragPosition;

uniform vec3 albedo;  // Base color of the material
uniform float metallic;
uniform float roughness;
uniform float ao; // Ambient occlusion
uniform vec3 lightPosition; // World space position of the light
uniform vec3 lightColor; // Color of the light
uniform vec3 viewPos; // Camera position for specular reflection

uniform samplerCube skybox;  // Cubemap texture for environment mapping
uniform float reflectionStrength; // Add this uniform to control reflection strength

const float PI = 3.14159265359;

const vec3 ambientLight = vec3(0.03, 0.03, 0.03); // Ambient light color

// Define a structure for lights
struct Light {
    vec3 position;
    vec3 color;
};

uniform Light lights[2]; // Array for two lights


/// Calculate Fresnel-Schlick approximation
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (max(vec3(1.0 - metallic), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}
const float maxMetallicFresnel = 0.1; // Capped metallic value for Fresnel calculation

void main() {
    vec3 N = normalize(FragNormal);
    vec3 V = normalize(viewPos - FragPosition);
    //V.y = -V.y;

    vec3 F0 = mix(vec3(0.04), albedo, min(metallic, maxMetallicFresnel));
    
    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);
    
    for (int i = 0; i < 2; i++) {
        vec3 L = normalize(lights[i].position - FragPosition);
        vec3 H = normalize(V + L);
        float NdotL = max(dot(N, L), 0.0);
        float NdotV = max(dot(N, V), 0.0);
        float NdotH = max(dot(N, H), 0.0);
        float VdotH = max(dot(V, H), 0.0);
        
        // Diffuse term using Lambertian reflectance
        vec3 diffuse = albedo * lights[i].color * NdotL / PI;
        
        // Phong specular reflection for the fresnel effect
        vec3 fresnel = F0 + (1.0 - F0) * pow(1.0 - NdotV, 5.0);
        vec3 specular = pow(VdotH, roughness * 128.0) * fresnel;
        
        totalDiffuse += diffuse;
        totalSpecular += specular * (1.0 - metallic); // Specular only for non-metallic parts
    }
    
    // Environmental reflection
    vec3 envReflection = texture(skybox, reflect(V, N)).rgb * reflectionStrength;
    
    // Combine diffuse, specular, and environmental reflection
    vec3 color = ambientLight * ao + totalDiffuse + totalSpecular + envReflection * metallic;
    
    FragColor = vec4(color, 1.0);
}
