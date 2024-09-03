# Animation with Dynamic Cubemap

<img src="/scenes/overview.gif" alt="scene_overview" title="scene_overview" width="1080"/>
This is my final project for the CSE 169 (Computer Animation) course at UCSD, showcasing a character animation system with skeleton loading, skinning, and keyframe animation. It also includes static and dynamic cubemaps for environment mapping, real-time reflections, and basic Physically Based Rendering (PBR) materials, all developed with OpenGL and GLSL.


**Note**: The reflection using the dynamic cubemap is slightly off, but I will work on fixing it as soon as possible. I hope you enjoy!

## Program Controls:

* **Rotate Camera**: Mouse Click + Drag
* **Pan Camera**: W / A / S / D
* **Zooming**: 'Q' (closer) and 'E' (farther).
* **Reset Camera**: 'R'


# Features:


* **Skeleton Loading and Visualization**
    * Loads and visualizes a character skeleton from a `.skel` file, supporting 3-DOF rotational joints (ball-and-socket joints).
    * Performs forward kinematics to compute world space matrices for each joint.
    * Allows flexible command line input for loading different `.skel` files.

* **Skinning and Shading**
    * Attaches a character skin to the skeleton by loading from a `.skin` file.
    * Implements basic shading with at least two colored lights to enhance visual realism.
    * Provides interactive controls for adjusting the Degrees of Freedom (DOF) of the skeleton joints, either through keyboard input or a GUI-based approach.

* **Keyframe Animation**
    * Integrates keyframe animations by loading an `.anim` file and playing it back on the skinned character.
    * Supports seamless integration of `.skel`, `.skin`, and `.anim` files via command line arguments, enabling various character and animation combinations.

* **Static Cubemap - Skybox**
    * Implements a static cubemap for environment mapping, creating a realistic skybox that enhances the sense of immersion.
    * Utilizes the `GL_TEXTURE_CUBE_MAP` enum in OpenGL to load six 2D textures corresponding to each face of the cube, simulating a vast 3D environment.

* **Dynamic Cubemap - Real-Time Reflections**
    * Generates dynamic cubemaps at runtime, allowing for real-time reflections on reflective or refractive surfaces like spheres.
    * Renders the scene from six different perspectives (one for each face of the cube) by setting the camera along each positive and negative axis with a 90-degree field of view (FOV).
    * Uses a framebuffer object to capture and store the rendered images for each face, which are then combined into a cubemap texture.
    * Applies the cubemap texture during the final rendering pass to create realistic and responsive reflections, adapting to real-time environmental changes.

* **Basic Physically Based Rendering (PBR) Material**
    * Implements a basic PBR material shader that calculates realistic lighting effects using parameters like albedo, metallic, roughness, and ambient occlusion.
    * Integrates environmental reflections using a cubemap texture, allowing dynamic reflections on the material's surface.
    * Uses the Fresnel-Schlick approximation to simulate realistic specular reflections, adjusting for different material properties.
    * Supports multiple light sources with diffuse and specular components, enhancing the realism of rendered objects.


# Launching the Program

* Clone the repository.
```
git clone https://github.com/jayHuggie/Animation_with_DynamicCubemap.git
```
* Navigate to the following directory:
```
  DynamicCubeMap/build/Debug
```

* Open “Animation_with_DynamicCubeMap.exe” to run the program!
