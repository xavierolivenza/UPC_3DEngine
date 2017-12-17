# UPC_3DEngine

UPC_3DEngine is a universitary project for the subject 3DEngine of the VideoGame Developing career in CITM, UPC fundation in Terrassa.
Made by Xavier Olivenza and Sergio Alvarez. The main objective is to create a simple 3D game editor. By doing that, students can learn about different techniques used in actual videogame industry. It's also a great tool to itroduce programmers that have worked and programmed only in 2D environment to 3D programming in videogames.

## TEAM MEMBERS

### Xavier Olivenza Busquets

<img src="https://github.com/xavierolivenza/UPC_3DEngine/blob/master/docs/Xavier_Olivenza.png?raw=true" width="300">

- Github: <a href="https://github.com/xavierolivenza">Link</a>

What I have done:

-The implementation of Assimp in the code to be able to read fbx files, collect the data of vertices, indices, normals and UVs, save them and show the model on screen with OpenGL.

-The implementation of DevIL in the code to be able to read image files, load them in memory and change their format.

-Hierarchy of GameObjects and components. Every object in the world is a GameObject, and this acquires meaning depending on which components it has. I created the transformation, mesh and material components.

-Since reading files with assimp or devil is too slow (not because the libraries, but the formats and all the data they contain), I've implemented own file formats for greater loading speed, for the fbx files, format .meshAlvOli/.GameObjectMeshAlvoli and for the textures .dds format.

-For the frustrum culling I created the camera component, which contains the frustum. Thanks to this component and the collision test with the AABB of the other GameObjects, frustrum culling is achieved.

-I have also done the whole system of saving and loading the scene, which entails saving all the GameObjects with their hierarchy, and all the components of each GameObjects with their characteristics.

-Mouse picking, using the frustum of the editor's camera and the click of the mouse, I made it possible to select the GameObjects with AABB in the scene, which is useful to click a GameObject and visualize its components in the inspector window.

-I enabled the time management, with the play/stop, pause/continue and frame buttons, there is also a window in which you can see how the time advances for the real time and the time of the game, there is also the possibility to modify the time, accelerating it or slowing it down.

-Resources, I have implemented a resource system to load meshes and textures only once in memory, so if several GameObjects/components have the same mesh or texture loaded, they all use the same resource.

-With ImGuizmo I created the gizmos to move, rotate and scale the game objects.

-I also made a small modification in the octree to make it adaptive to the scene that is loaded.

-Finally, I made the particle system, first making an UML to organize the code and functionality. I have programmed the emission, movement, characteristics and behavior of the particles. The files ParticleSystem.h/.cpp have some repeated functionality from the whole engine, but I wanted to keep it as independent as possible, so for example, it has an internal storage system for meshes and textures. So I have made sure that these two files are 100% independent of all the code (can be compiled in a dll and export), who is responsible for linking these files with the whole engine is the particle system component, it is responsible for receiving information from engine and user input, and react by calculating different things and filling the ParticleSystem with information only calling its methods.

### Sergio Alvarez Llorca

<img src="https://github.com/xavierolivenza/UPC_3DEngine/blob/master/docs/Sergio_Alvarez.png?raw=true" width="300">

What I have done:

-I have worked in the Octree implementation and usage with frustum culling.

-I have worked in the UI of the Engine.

-I have worked in camera serialization.

-I have worked in actualizing some of the libraries used.

-I have worked in some primitives.

-I have worked in particles editor UI.

-I have created some smokes and fireworks of the particle system.

 - Github: <a href="https://github.com/Sergio111">Link</a>

### CORE SUB-SYSTEMS

### Structure of GameObjects and Components

The engine uses a structure of GameObjects. All the GameObject have a tranformation component and several components which determine their GameObject type, for example, component mesh, component material, component camera, etc... All the GameObjects have an active and static button to choose if the user can or can't modifiy the transformation. The component transformation has three dragfloat3 to modify rotation, scale and translation.

### Assimp, DevIL and Own File Format

At first we draw geometry with direct draw mode of openGL. Then we started using Assimp in the code to be able to read fbx files, collect the data of vertices, indices, normals and UVs, save them and show the model on screen with OpenGL. And we used DevIL to load textures. DevIL is an open source library to develop applications with very powerful image loading capabilities. This was not very good in terms of performance because reading files with assimp or devil is too slow (not because the libraries, but the formats and all the data they contain) and has a lot of cost. To solve this, we implemented our own file format to enhance the performance in loading by reading all the data stored in our format files, that is much faster than Assimp importation.

### Scene serialization

You can save any scene you have worked in by clicking File->Save File and introducing a name to the scene, if the name is already in use, it will overwritte the first one. We store the hierarchy of the GameObjects, and all it's components. All this information is stored in a JSON.

### Frustrum culling and octree

The frustum culling is an optimization that consists in culling (not drawing) the GamObjects which AABB is enterily outside the main camera. This is a great enhancement because you win a lot in performance due to not having to waste in drawing all the GameObjects that you don't really need to. Our frustum culling works with an adaptative octree, that iterates all the GameObjects and distribute them in children that it creates.

### Mouse Picking

When you click a geometry that is inside the editor camera, we use the raycast to find which object's AABB you have clicked, so the user can select any GameObject in the scene to visualize and modify it's components.

### Time manager

We implemented a time manager similar to Unity time manager. We have a start button, which makes all GameObjects start updating it's logic, a pause button which stops updates, a stop button which makes all GameObjects return to it's original state and when you click pause button, you have a continue button which makes GameObjects update again and frame button, which makes update only one frame. There's also a window where you can se how real time and game time advances and the possibility to modify time.

### Resource Manager

Thanks to this implementation now the mesh and texture are loaded only once in memory. So, if a GameObject or a component have the same mesh or texture loaded, they will use the same resource and not restore it again. This is an important optimization, it's good to not store in memory things that are already stored.

## PARTICLE SYSTEM

As a high-level system we have made a prticle system. In our engine wou can create a particle system with Create->Create Particle System to create a GameObject with a particle system component. When creating a particle component, a part of all the transformation options that you have in all components there's an option to visualize a particle editor. The particle editor has multiple options.

If you have loaded a texture in the preview particle texture window, you can set the number of columns and rows you want in the preview to choose a posible limitation in animation frames, you can also choose in which direction the animation of the texture has to be readed.

You have initial and final state editor window. In these windows the options are the same, the difference is in which moment of the life of the particle the modifications are going to be applied. The options that you have in these windows are changing the color and alpha of the particle, setting a color variation, setting an acceleration (gravity is the one predefined) and set an acceleration variation.

Finally you have the emitter options editor. In this window you can choose the shape of the emitter and some variables depending on the shape, for example aperture and base radius and height for the cone. You can also choose the emitter life, how many particles is the emitter emitting per second, the lifetime of the particles, lifetime variation, if you want loop in the emision or not, you can see the emission duration, the number if particles in scene, the speed of the particles and a variation if speed, the emyssion type (Local or World), the particle facing (Null, Billboard, Horizontal Billboard, Vertical Billboard), if you want the emitter to be shown, if you want the emitter AABB to be shown and finally AABB min and max values to set an AABB around the Particle System to detect it from culling techniques such as octree.

Once you have all the options in a way you like, just press play button and see the particle component running in scene. While running you can continue editing each particle component, but when you press stop, the scene is reseted as it was at the time when you pressed the play button.

Finally you can save and load all the particle resources and emitters each with its texture, pressing the buttons in each particle system component.

![](https://github.com/xavierolivenza/UPC_3DEngine/blob/master/docs/GIF01.gif?raw=true)
![](https://github.com/xavierolivenza/UPC_3DEngine/blob/master/docs/GIF02.gif?raw=true)
![](https://github.com/xavierolivenza/UPC_3DEngine/blob/master/docs/GIF03.gif?raw=true)
![](https://github.com/xavierolivenza/UPC_3DEngine/blob/master/docs/GIF04.gif?raw=true)

### PARTICLE SYSTEM TUTORIAL

First of all, to see particle emitter working, you must be in play mode, so fist of all, press play button of time manager.

In the particle system component you can see a checkbox to open the particle editor.

The particle editor consists of three boxes and a larger box at the bottom area. From the three boxes at the top starting from the left, there is the texture and animation editor, the editor of the initial state of the particles and the editor of the final state of the particles.

- Texture and animation editor: In this area, the loaded texture that the particles will use appears in the box. Below the canvas where the texture is shown, there is the selector of how many rows and columns the spritesheet has, there is also another option to select how many frames of the animation you are going to use, and at the end, the option to set if the animation advances starting from the upper left corner and advancing to the right row to row, or advancing down and column to column.

- The editor of the initial/final state of the particles: In these editors you can set initial and final states options with values and variables. These states are linearly interpolated during the lifetime of the particle. You can select a color filter and alpha, size and variation, color variation of the filter (from 0.0f to 1.0f) and you can finally establish an external force in three axes and its variation.

The last and largest box is for the emitter options, with three columns.

- The first column serves to set the shape of the emitter, sphere, semisphere, cone, box and circle, each with its variables.

- The second column contains the life of the emitter, which if negative, the emitter never dies, the number of particles emitted per second, the life and variation of the life of each particle, duration of the emission in seconds and the number of alive particles, there is also a checkbox to make the emission loop or not, so if the life of the emitter is negative or this checkbox is active, the emitter will not die, finally there is the option to set the speed and variation of the particles emitted.

- In the last column, there is the option of what type of billbioard is wanted for the particles, null, billboard and vertical or horitzontal billboard, there are also two checkboxes to draw or not the emitter and to draw or not the AABB of the particle system, finally there are the options to set the maximum and minimum point of the AABB  of the particle system.

Going back to the options that the component has, there is the option to save and load the particles (the three upper boxes of the particle editor) and the option to save and load the emitter (the bottom box of the particle editor), we save two different resources because with that you can swap particles and emitters with different particle systems. There are also the options to load a particle and emitter as a child of the particle system that is being edited, so if the system that is being edited has the option to die (the emitter does not loop and it has positive life), when this die, the child emitter will be executed, there is also the option to unlink the children, with the "Unload children" button, then there are two texts that will show the paths of the particle and emitter loaded as a children, if this do not show paths, it means that this does not have children assigned.
Finally there is a button to select the texture that you want to load in the particle system.

### Speed-Up video of the Engine

<iframe width="560" height="315" src="https://www.youtube.com/embed/IHkEHwYG4kM" frameborder="0" gesture="media" allow="encrypted-media" allowfullscreen></iframe>

## CREDITS

Github project page: <a href="https://github.com/xavierolivenza/UPC_3DEngine">Link</a>

Sergio Alvarez's Github account

 - Github: <a href="https://github.com/Sergio111">Link</a>

Xavier Olivenza's Github account

 - Github: <a href="https://github.com/xavierolivenza">Link</a>

## LICENSE

 GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

CITM Terrassa 2016-2017
