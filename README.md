# C++ RayTracing Framework
A raytracing framework written in C++, implemented reflection; refraction; texture mapping; mesh model; 360 background; motion blur, defocus blur (depth of field).

The initial state of the scene including the positions and transformations for objects, lights and camera; the colors, textures, and coefficients of materials are read from "raytracing-framework/Asset/video.lua".

A camera path system is impletemted: In A4.cpp camera states can be created by setting the duration and camera trasformation at the state.

A basic physics engine that can calculate the kinetics of billiard balls, and simulate perfect elastic collisions.

## Video Demo:

Please watch the full video demo [here](https://github.com/DaveHJT/RayTracing-Project/blob/main/demo%20video.mp4).

## Document:
Please see the [document](https://github.com/DaveHJT/RayTracing-Project/blob/main/document.pdf) for details.

## Compilation:
1. This assignment is completed on the CS484 Virtual Machine.

2. Go to the folder "raytracing-framework", run the program by : 
premake4 gmake
make
/A4 Asset/video.lua

## Manual:
1. This program is based on the A4 framework so it runs the same as A4. After activation, the frames are generated in ./render folder.

## Image Demo:

### Reflection and Refraction

<p>
<img src="https://github.com/DaveHJT/RayTracing-Project/blob/main/render/records/refraction%20test%201.6.png?raw=true" width="600">
</p>

### Depth of Field

<p>
<img src="https://github.com/DaveHJT/RayTracing-Project/blob/main/render/records/dof.png?raw=true" width="600">
</p>
