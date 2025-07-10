# Veer

> If you wish to make an apple pie from scratch, you must first invent the universe  
--- Carl Sagan

Veer is the low level framework from which I'll build my game engine, using as little dependencies as possible (the goal being to learn things)   
The first apple pie will probably be a flight simulator game  

## Tasks

Todo before v0 :

[ ] Remove platform specific code from application.cpp

Todo in v0.1

[ ] Add GPU resources types
[ ] Draw triangle in GBuffer
[ ] Add parser to load GPU resources from files
[ ] Draw textured mesh in GBuffer

Todo in v0.2

[ ] Put gameplay code into app project
[ ] Add input system

Unsorted :  

[ ] Switch building pipeline to CMake (to be able to ditch VisualStudio)
[ ] Add Vulkan support
[ ] Think about where to put the boundary between app and engine (should the job system, service init, and main render loop be in the engine or left as tools for the app to compose with ? Freedom / boilerplate)

[ ] Start thinking about allocators
