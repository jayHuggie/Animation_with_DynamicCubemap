# Cloth Simulation

<img src="/scenes/cloth_video.gif" alt="scene_overview" title="scene_overview" width="1080"/>

This is a simulation of a cloth using particles, spring-dampers, and triangular surfaces, developed with OpenGL for my CSE 169 (Computer Animation) course at UCSD.

# Features:


* **Particle-Based Cloth Simulation**

    Implements a cloth model using a system of particles, spring-dampers, and triangular surfaces.
    Simulates realistic cloth behavior with forces such as uniform gravity, spring elasticity, damping, and aerodynamic drag.

* **Spring-Damper System**

    Uses a spring-damper model to maintain the structural integrity of the cloth.
    Adjustable spring constants to control the stiffness of the cloth.
    Damping factor to regulate the oscillations and achieve a stable rest state.

* **Aerodynamic Interactions**

    Includes aerodynamic drag that affects the cloth’s movement in response to wind and other environmental factors.
    Provides controls to adjust ambient wind speed, allowing for dynamic simulations of cloth under different wind conditions.

* **Ground Plane Collision Handling**

    Simulates collisions between the cloth and a simple ground plane, ensuring the cloth reacts realistically upon impact.
    Implements restitution and friction to model realistic ground interactions.

* **Fixed Particles for Cloth Manipulation**

    Allows certain particles in the cloth to be fixed in place to simulate various cloth-hanging scenarios (e.g., corners fixed to simulate a hanging flag).
    User controls to manipulate the position of fixed particles, enabling real-time interaction and adjustment of the cloth’s configuration.

* **Dynamic Normal Calculation and Shading**

    Calculates triangle normals dynamically during each update for accurate physics simulation.
    Implements dynamic smooth shading by averaging normals at vertices, enhancing the visual realism of the cloth.

* **Real-Time User Interaction**

    Provides controls to adjust the position of fixed particles and influence the cloth's behavior in real time using **ImGui**.


# Launching the Program

* Clone the repository.
```
git clone https://github.com/jayHuggie/Cloth_Simulation.git
```
* Navigate to the following directory:
```
  Cloth_Simulation/build/Debug
```

* Open “Cloth_Simulation.exe” to run the program!
