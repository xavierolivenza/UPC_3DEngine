# UPC_3DEngine

UPC_3DEngine is a project for the subject 3DEngine of the VideoGame Developing career in CITM, UPC fundation in Terrassa.
The main objective is to create a simple 3D game editor.

## TEAM MEMBERS

### Xavier Olivenza Busquets

![](https://github.com/xavierolivenza/UPC_3DEngine/blob/master/docs/Xavier_Olivenza.png)

- Github: https://github.com/xavierolivenza

### Sergio Alvarez Llorca

PHOTO TEMPLATE

I have worked in the Octree implementation and usage with frustum culling, I have worked in the UI of the Engine, I have worked in camera serialization, I have worked in actualizing some of the libraries used, I have worked in some primitives and I have created some smokes and fireworks of the particle system.

 - Github: https://github.com/Sergio111

### CORE SUB-SYSTEMS

-Profiler: This window shows the ms wasted in every module

-Hierarchy: This window shows game object hierarchy, if you clic one node, the inspector window updates with the new data to show

-Inspector: This window shows game object and associated components configuration

-Console: Can show a console that LOGS all the engine process, the files in assets foilder and the files inside library folder

-Configuration:

	-Application: This window shows useful information of the app, FPS, memory usage, etc... It also allows the user to cap FPS, change app name, etc...
	
	-Time: Allow you to see time variables and edit time distortion
	
	-Window: This window shows different window information and allows the user to modify some window variables
	
	-Hardware: This window shows useful hardware information
	
-Module Variables: This window shows useful information about every module. And allows the user to modifiy some variables in some of them

-Resource: First frame shows in memory resources, if you click one, the second frame show information about that resource and if texture, its importing options, change them and click import button to update the resource.

-File: Option load to load fbx and scenes from assets, you have a shorcut directory to scenes folder. Option save to save the actual scene to scene folder. Option Clear Scene to clean all scene game objects.

-View: This window let the user choose which windows visualize. It can also be done with LAlt + number (1 to 8 by now)

-Create: One option Create Camera to create one gameobject with camera.

-Help: This window has the options to download latest release, report bug, and show the documentation. It also has about, which is a window with useful information like license, libraries used, authors, etc...

-Close: Closes the engine

## PARTICLE SYSTEM

Our particle system is an aproach to Unity particle system. You can create a particle system component. When creating a particle component, a part of all the transformation options that you have in all components there's an option to visualize a particle editor. The particle editor has multiple options.

If you have loaded a texture in the preview particle texture window, you can set the number of columns and rows you want in the preview to choose a posible limitation in animation frames, you can also choose in which direction the animation of the texture has to be readed.

You have initial and final state editor window. In these windows the options are the same, the difference is in which moment of the life of the particle the modifications are going to be applied. The options that you have in these windows are changing the color and alpha of the particle, setting a color variation, setting an acceleration (gravity is the one predefined) and set an acceleration variation.

Finally you have the emitter options editor. In this window you can choose the shape of the emitter and some variables depending on the shape, for example aperture and base radius and height for the cone. You can also choose the emitter life, how many particles is the emitter emitting per second, the lifetime of the particles, lifetime variation, if you want loop in the emision or not, you can see the emission duration, the number if particles in scene, the speed of the particles and a variation if speed, the emyssion type (Local or World), the particle facing (Null, Billboard, Horizontal Billboard, Vertical Billboard), if you want the emitter to be shown, if you want the emitter AABB to be shown and finally AABB min and max.

Once you have all the options in a way you like, just press play button and see the particle component running in scene. While running you can continue editing each particle component.

Finally you can save and load all the particle resources and emitters each with its texture, pressing the buttons in each particle system component.

![](https://github.com/xavierolivenza/UPC_3DEngine/blob/master/docs/GIF01.gif)
	
## CREDITS

Github project page: https://github.com/xavierolivenza/UPC_3DEngine

## LICENSE

 GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

CITM Terrassa 2016-2017
