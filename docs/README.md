# UPC_3DEngine

UPC_3DEngine is a project for the subject 3DEngine of the VideoGame Developing career in CITM, UPC fundation in Terrassa.
The main objective is to create a simple 3D game editor.

## TEAM MEMBERS

### Xavier Olivenza Busquets

![](https://github.com/xavierolivenza/UPC_3DEngine/blob/master/docs/Xavier_Olivenza.png?raw=true)

- Github: https://github.com/xavierolivenza

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

![](https://github.com/xavierolivenza/UPC_3DEngine/blob/master/docs/Sergio_Alvarez.png?raw=true)

What I have done:

-I have worked in the Octree implementation and usage with frustum culling.

-I have worked in the UI of the Engine.

-I have worked in camera serialization.

-I have worked in actualizing some of the libraries used.

-I have worked in some primitives.

-I have worked in particles editor UI.

-I have created some smokes and fireworks of the particle system.

 - Github: https://github.com/Sergio111

### CORE SUB-SYSTEMS

### Structure of GameObjects and Components

The engine uses a structure of GameObjects. All the GameObject have a tranformation component and several components which determine their GameObject type, for example, component mesh, component material, component camera, etc... All the GameObjects have an active and static button to choose if the user can or can't modifiy the transformation. The component transformation has three dragfloat3 to modify rotation, scale and translation.

### Resource Manager

Thanks to this implementation now the mesh and texture are loaded only once in memory. So, if a GameObject or a component have the same mesh or texture loaded, they will use the same resource and not restore it again. This is an important optimization, it's good to not store in memory things that are already stored.

### Assimp and Own File Format

At first we draw geometry with direct draw mode of openGL. Then we started using Assimp in the code to be able to read fbx files, collect the data of vertices, indices, normals and UVs, save them and show the model on screen with OpenGL. This was not very good in terms of performance because reading files with assimp or devil is too slow (not because the libraries, but the formats and all the data they contain) and has a lot of cost. To solve this, we implemented our own file format to enhance the performance in loading by reading all the data stored in our format files, that is much faster than Assimp importation.

### Scene serialization

You can save any scene you have worked in by clicking File->Save File and introducing a name to the scene, if the name is already in use, it will overwritte the first one. We store the hierarchy of the GameObjects, and all it's components. All this information is stored in a JSON.

### Mouse Picking

When you click a geometry that is inside the editor camera, we use the raycast to find which object's AABB you have clicked, so the user can select any GameObject in the scene to visualize and modify it's components.

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

## CREDITS

Github project page: https://github.com/xavierolivenza/UPC_3DEngine

## LICENSE

 GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

CITM Terrassa 2016-2017
