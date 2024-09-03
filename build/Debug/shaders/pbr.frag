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


//vec3 F0 = vec3(0.04);



/* 
// Main function
void main() {
    vec3 N = normalize(FragNormal);
    vec3 V = normalize(viewPos - FragPosition);

    // Hardcoded light for this example
    vec3 L = normalize(lightPosition - FragPosition);
    vec3 lightContribution = calculateDirectLighting(N, V, L, lightColor, roughness, metallic);
    
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + lightContribution;

    // Gamma correction
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color, 1.0);
}
*/
 /*
// Main function simplified for debugging
void main() {
    // Directly visualize albedo
    vec3 color = albedo;

    // Uncomment one block at a time to visualize its effect

    // Visualize metallic effect
    color *= metallic;

    // Visualize roughness effect
    color *= vec3(roughness);

    // Visualize ambient occlusion effect
    color *= ao;

    // Hardcode a simple lighting effect to visualize light influence
    vec3 N = normalize(FragNormal);
    vec3 L = normalize(lightPosition - FragPosition);
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * lightColor;
    color *= diffuse;

    FragColor = vec4(color, 1.0);
}

*/
/* 
void main() {
    vec3 N = normalize(FragNormal);
    vec3 V = normalize(viewPos - FragPosition);
    vec3 L = normalize(lightPosition - FragPosition);
    vec3 H = normalize(V + L);

    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);
    float NdotH = max(dot(N, H), 0.0);
    float VdotH = max(dot(V, H), 0.0);

    // Fresnel term
    vec3 F0_mixed = mix(F0, albedo, metallic);
    vec3 F = fresnel(VdotH, F0_mixed);

    // Simple diffuse and specular model without considering roughness
    vec3 diffuse = (1.0 - F) * albedo / PI;
    vec3 specular = F;

    // Combine effects
    vec3 color = (diffuse * (1.0 - metallic) + specular) * NdotL * lightColor;

    FragColor = vec4(color, 1.0);
}
*/
/* 
void main() {
    vec3 N = normalize(FragNormal);
    vec3 V = normalize(viewPos - FragPosition);
    vec3 R = reflect(-V, N);

    // Sample the environment map using the reflection vector
    vec3 envColor = texture(skybox, R).rgb;
    
    // Apply roughness to the reflection
    vec3 reflection = mix(envColor, vec3(0.5), roughness); // Use 0.5 (mid-gray) to simulate diffuse reflection for rough surfaces

    // Fresnel-Schlick approximation
    float cosTheta = dot(N, V);
    vec3 F = fresnel(cosTheta, F0);

    // Adjust base reflectivity based on metallic property
    vec3 baseReflectivity = mix(F0, albedo, metallic);

    // Combine direct reflection with the albedo base color
    vec3 color = mix(albedo * (1.0 - metallic), reflection, F * metallic);

    // Adjust the color by the reflection strength
    color = mix(color, reflection, reflectionStrength);

    // Apply ambient occlusion
    color *= ao;

    FragColor = vec4(color, 1.0);
}
*/

/* 
void main() {
    // Normal, View, and Reflection vectors
    vec3 N = normalize(FragNormal);
    vec3 V = normalize(viewPos - FragPosition);
    vec3 L = normalize(lightPosition - FragPosition); // Assuming a single light source for simplicity

// Diffuse term using Lambertian reflectance
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * lightColor * albedo;

    // Ambient Occlusion applied to the diffuse term
    diffuse *= ao;

    // Simple environmental reflection (omitting Fresnel and roughness for now)
    vec3 R = reflect(-V, N);
    vec3 envReflection = texture(skybox, R).rgb;

    // Combining the diffuse term with environmental reflection
    // Note: This simplistic model doesn't fully account for roughness affecting the diffuse term.
    vec3 color = ambientLight + diffuse + envReflection * metallic; // Blend some environmental reflection

    FragColor = vec4(color, 1.0);
}
*/

/* 
void main() {
    vec3 N = normalize(FragNormal);
    vec3 V = normalize(viewPos - FragPosition);
    vec3 L = normalize(lightPosition - FragPosition);
    vec3 R = reflect(-V, N);

    // Diffuse component
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * lightColor * albedo * (1.0 - metallic);

    // Environment reflection for metallic surfaces
    vec3 envReflection = texture(skybox, R).rgb;

    // Calculate the Fresnel term
    float cosTheta = dot(N, V);
    vec3 F = fresnel(cosTheta, F0);

    // Simple specular reflection model (for the sake of example, using a constant value for specular highlight)
    vec3 specReflection = vec3(0.04) * F; // Specular highlight color, could be more complex

    // Combine diffuse, specular, and environmental reflections
    // Metallic materials use more of the environment map; non-metallic materials use the Fresnel-modulated specular reflection
    vec3 color = ambientLight * ao + diffuse + mix(specReflection, envReflection, metallic);

    // Apply ambient occlusion
    color *= ao;

    FragColor = vec4(color, 1.0);
}*/

/* lookin better
void main() {
    vec3 N = normalize(FragNormal);
    vec3 V = normalize(viewPos - FragPosition);
    vec3 L = normalize(lightPosition - FragPosition);
    float NdotL = max(dot(N, L), 0.0);

    // Diffuse term using Lambertian reflectance
    vec3 diffuse = albedo * lightColor * NdotL / PI;

    // Environmental reflection
    vec3 R = reflect(-V, N);
    vec3 envReflection = texture(skybox, R).rgb;

    // Incorporate ambient occlusion and diffuse lighting into reflection modulation
    // This step dynamically adjusts the reflection intensity based on lighting conditions
    vec3 reflectionModulator = mix(vec3(1.0), vec3(ao * NdotL), metallic);

    // Modulated environmental reflection based on diffuse lighting and AO
    vec3 modulatedEnvReflection = envReflection * reflectionModulator;

    // Combine diffuse lighting with modulated environmental reflection
    vec3 color = (ambientLight * ao + diffuse) + modulatedEnvReflection * metallic; // The reflection contribution can be further controlled by the metallic property

    FragColor = vec4(color, 1.0);
} */