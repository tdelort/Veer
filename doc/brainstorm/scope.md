# Veer's Scope  

This is intended as a Hardward Abstraction Library + Common Toolbox for making game engine. Not an actual game engine  

Thus, it will only contain basic low level functions and types that make life easier without restraining too much what can be done  

## Rendering  

No Render graph implemented here BUT all the basic blocks needed.  

A render device class + allocator class (today, they are the same, might be wise to split them ??)  

Render device resources abstraction (read textures, buffers, etc) with automated resource state tracking (this could be opt. out if the need arises (say you know your full render graph early on OR you are fully monothreaded) )  

Bindless for easier implementation and customisation  

### Shaders  

Veer will take as input shader code with special syntax (mostly custom macros) to make it hardward independant + a metadata file containing what's needed to build the PSO (blend states, render target formats, input layout, etc)  
This metadata file will be in a syntax making it easy to express all combinaisons of all of these metadata to avoid having PSO recompiling during runtime AND to minimise PSO count  

Since I'm not yet sure about putting any serialization code in veer. 

## Physics  

The client should be able to build 2D or 3D physics, and a wide variety of things from simple collision detection to simulating a basic airfoil  

Note 100% settled on where veer should stop, but I think it should at least contain the math for simple primitive to primitive collision detection and solving  

## Std

Most basic things in veer (veer itself is built on them)  

### Containers

For starters, I want at least :  

statically allocated array of static size  
dynamically allocated array of static size  
dynamically allocated array of dynamic size  
OR ?
static_array,     <T, N> (std::array)
resizable_array,  <T>    (std::vector)
// buffered_resizable_array,  <T, N>

ordered map and set ( read binary tree here )  
unordered map and set ( read hash table here )  

And maybe a (doubly) linked list type. (the reduced insert cost at the expanse of more cache trashing might never be needed, so don't implement yet)

There might be some need for other types like queues, but these should probably built on top of existing types  
(from my experience, we don't need a lot more than the above containers if there is a way to scope their API (for example a stack view of a vector))  


### Math

We have vector, matrices, quaternions, and things like that

### OS

Expose writing / reading from disk, getting time, 
Pooling inputs events  
For now the window is handled by GLFW, but it might be interesting to handle it ouserlves

### Multi Threading

Expose 2 multi threading idioms. The hability to create threads AND a thread pool system with tasks  
(most of the time, we will only want to use the tasks system, but some API might require us to pool their status in a loop in a thread)  
For multi threading synchronisation primitives, I want to have something that makes the client afraid of using them. Locks should be forbidden UNLESS ALL OTHER WAYS HAVE BEEN EXPLORED. Because even one lock will have a slowdown scaled by the number of running threads (and my guess is that CPU will only increase in cores (and maybe bandwidth) and not clock speed from now on)  
BUT one exception might be simple atomic types. So i'll probably expose the atomic operations such as compareAndSwap  

With high multithreading in mind, a strong focus should be put on how a task graph can be made and debugged.  
