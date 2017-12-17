# UPC_3DEngine

UPC_3DEngine is a project for the subject 3DEngine of the VideoGame Developing career in CITM, UPC fundation in Terrassa.
The main objective is to create a simple 3D game editor.

## WEBSITE

Web Page: https://xavierolivenza.github.io/UPC_3DEngine/

## Assignment 3

To see particle systems with the street scene, you can click the file menu of the upper left corner, select load file, go to the scenes directory (../Game/Assets/Scenes/), and select one of the three scenes named as:

- Assignment3_street_with_smoke

- Assignment3_street_with_smoke_and_snow

- Assignment3_street_with_smoke_and_fire_and_land_smoke

Warning: The smoke comming out of the chimneys have a little position problem because the texture that the particles are using is not perfectly centred, so the billboards are well placed, but the texture is not centred and depending on which angle you see the chimneys, it seems the smoke do not come from the chimneys holes exactly.

If you want to open an other scene, go to the file menu and click Clear Scene option, now you can load an other scene without having multiple scenes loaded at once, we don't do this automatically because you can use scenes as prefabs, so we let you load multiple times the same scene, and if you want to reset, you must do it manually.

Tip: To see particles working, you must be in play mode, so press play button of time manager.
Tip: You can open View->Configuration->Time, to make time distortion while play mode.
Tip: For more info about controls, go to "Controls" and check "Particle System controls".
Tip: Also if you want to learn how to use the particle system, there is a "PARTICLE SYSTEM TUTORIAL" below "Controls".

## INSTALLATION

Unzip the realese folder and execute the .exe file.

In assets folder, you can find some models to test the engine.

## USAGE

FRUSTUM CULLING:

Load any geometry, open culling window with LAlt + 8 or manually. You can draw octree by pressing F1 or in culling window. Press calc octree button. Then choose one camera, set it to main camera and enable frustum culling in the component camera window. If you add new geometry calculate octree again.

RESOURCE MANAGER:

We have reference counting.

If you load a fbx and then you edit one of its textures or mesh, this will be reloaded.

Textures have importing/loading options.

WARNING: 

-This software is developed and tested in Windows OS, if you use this software in any other OS, we don't ensure this will work (we don't have any other computer with other OS to test it). Sorry for the inconvinience

-You can only load geometry files (.fbx, .FBX, .obj, .OBJ, .dae, .DAE) with textures in format (.jpg, .JPG, .png, .PNG, .tga, .TGA, .dds, .DDS).

-When you charge a new module without texture it will automatically charge checkers texture, the objective of that is to see if the UV are correct if you want the model without texture just go to render options and disable GL_TEXTURE_2D.

-street.fbx has a strange error, there are three houses that do not load at first attempt, but if you press play and then stop, or save and load a scene, they load well (you may have to repeat this loop to get it loaded)

TIPS:

-In the Option Window inside Configuration window, there is one option to toggle Library folder visibility, change it and restart the engine to update this.

-If you set one camera to MainCamera an press Play button, the main view of the editor is the view of the MainCamera, when you press stop, the view return to the editor camera.

-Octree only works with static objects, if an object is not static, this will not be inside octree. If you activate frustrum culling and everything disappears that mean that you might have static objects and an invalid octree, just recalculate it.

-Inside Assets folder will be created one Scenes Folder, here all scenes will be stored, the scene called "EditorScene_Backup" is the scene useed by the editor to store the play/stop status of the scene.

### CONTROLS

-Camera controls

	-Right click + "WASD" fps-like movement
	
	-Mouse wheel zooms in and out. If the mouse cursor is over engine UI or you have focus on any input text, wheel is disabled, press enter or move your mouse to world view to solve this.
	
	-Alt+Left click to orbit the object or right click
	
	-F focus the camera around the geometry you have selected in inspector, can pick it with mouse picking
	
	-Right click + "QE" lift movement. Q up, E down
	
-Gizmos controls

	-If the Game Object selected is static, the gizmo is not shown
	
	-Press W to enable Translation gizmo
	
	-Press E to enable Rotation gizmo
	
	-Press R to enable Scalation gizmo
	
-Particle System controls

Only works in play mode, so fist of all, press play button of time manager.
	
	-Spawn fireworks that execute a child emitter with 1
	
	-Spawn fireworks that are individual GameObjects executed one after the other with 2
	
-Drag any FBX and drop it into the executable to load it, if you load its textures before the fbx file, this will be automatically loaded with textures

### WINDOWS

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

-Create: Two options, Create Camera to create one gameobject with camera and create Particle System to create a gameobject with particle system component

-Help: This window has the options to download latest release, report bug, and show the documentation. It also has about, which is a window with useful information like license, libraries used, authors, etc...

-Close: Closes the engine

## PARTICLE SYSTEM TUTORIAL

First of all, to see particle emitter working, you must be in play mode, so fist of all, press play button of time manager.

In the particle system component you can see a checkbox to open the particle editor.

The particle editor consists of three boxes and a larger box at the bottom area. From the three boxes at the top starting from the left, there is the texture and animation editor, the editor of the initial state of the particles and the editor of the final state of the particles.

- Texture and animation editor: In this area, the loaded texture that the particles will use appears in the box. Below the canvas where the texture is shown, there is the selector of how many rows and columns the spritesheet has, there is also another option to select how many frames of the animation you are going to use, and at the end, the option to set if the animation advances starting from the upper left corner and advancing to the right row to row, or advancing down and column to column.

- The editor of the initial/final state of the particles: In these editors you can set initial and final states options with values and variables. These states are linearly interpolated during the lifetime of the particle. You can select a color filter and alpha, size and variation, color variation of the filter (from 0.0f to 1.0f) and you can finally establish an external force in three axes and its variation.

- The last and largest box is for the emitter options, with three columns.

- The first column serves to set the shape of the emitter, sphere, semisphere, cone, box and circle, each with its variables.

- The second column contains the life of the emitter, which if negative, the emitter never dies, the number of particles emitted per second, the life and variation of the life of each particle, duration of the emission in seconds and the number of alive particles, there is also a checkbox to make the emission loop or not, so if the life of the emitter is negative or this checkbox is active, the emitter will not die, finally there is the option to set the speed and variation of the particles emitted.

- In the last column, there is the option of what type of billbioard is wanted for the particles, null, billboard and vertical or horitzontal billboard, there are also two checkboxes to draw or not the emitter and to draw or not the AABB of the particle system, finally there are the options to set the maximum and minimum point of the AABB  of the particle system.

Going back to the options that the component has, there is the option to save and load the particles (the three upper boxes of the particle editor) and the option to save and load the emitter (the bottom box of the particle editor), we save two different resources because with that you can swap particles and emitters with different particle systems. There are also the options to load a particle and emitter as a child of the particle system that is being edited, so if the system that is being edited has the option to die (the emitter does not loop and it has positive life), when this die, the child emitter will be executed, there is also the option to unlink the children, with the "Unload children" button, then there are two texts that will show the paths of the particle and emitter loaded as a children, if this do not show paths, it means that this does not have children assigned.
Finally there is a button to select the texture that you want to load in the particle system.

## CHANGELOG

v1.0(Third Assignment):

	-Added particle system component
	-Implemented particle editor
	-Particles can have a texture and animation
	-Particles resource and emitter resource can be saved and loaded
	-Scenes are saved and loaded with particle systems
	-Added fireworks with 1 and 2
	-Added smoke to street_scene
	-Added snow to street_scene
	-Added fire to street_scene
	-Added ground smoke to street_scene

v0.7.1(Second Assignment):

	-Added debug info and variables for octree
	-Fixed octree
	-Fixed import manager
	-New console/assets/library window
	-Texture importing options added
	-Gizmos improved
	-Menu create camera
	-Changed UI colors
	-Radio Buttons added

v0.7.0:

	-Resource manager implemented
	-Frustum culling working with adaptative octree
	-Added gizmos to dynamic geometry (WIP)
	-Added time window with play, stop and pause
	-Added mouse picking
	-Save and Load process enhanced

v0.6:

	-New structure with game objects and components.
	-Components mesh, matrial, transformation working, camera components only shows frustum, WIP.
	-Now fbx are imported to library/Mesh as own format
	-MeshAlvOli, contains only one mesh. GameObjectMeshAlvOli contains multiples meshes
	-Textures are imported as dds to library/materials
	-By now this importing system takes everything inside assets folder and import them.
	-By now the load process only acceps own format files (open library/mesh to get the files to drop)

v0.5(First Assignment):

	-Framerate cap implemented
	-Implemented properties window which shows useful data about loaded geometry
	-Now normal length can be modified
	-If there's charged geometry now multiples textures can be loaded
	-Added new variables in some modules in module variables window
	-The camera now is centred in the centre of the scene when charging geometry, not in the mesh centre. The user can force it manually by pressing f
	-Added more debug data about CPU and VRAM

v0.4:

	-Camera autocentre + autoframe to loaded geometry
	-DevIL library added to load textures
	-FBX load their own textures from Assets folder
	-New style for the GUI(WIP, may will change)
	-Camera log "hit" if looking to the loaded geometry
	-Cube primitive texure coords added
	-Camera controls changed to be Unity-like

v0.3:

	-Profiler window
	-Module variables window
	-Primitive cube and sphere
	-Drag & drop
	-Assimp implemented
	-Load fbx files
	-Show vertices normals
	-New more clean layout

v0.2:

	-Added Console
	-OpenGl totally operative
	-Added geometry options: Fog, Wireframe, Lightning, Culling, etc...
	-All modules frame, ms, memory usage plot
	-First triangles printed on screen
	-Random generator feature added
	-Help and close menu added
	-Hardware data
	-Now you can modify variables from the editor
	-Parson save & load

v0.1:

	-Added base code.
	-Integrated ImGui library.
	-Added menu on top of screen.
	-Added menu that shows example window.
	
## CREDITS

Github project page: https://github.com/xavierolivenza/UPC_3DEngine

Sergio Alvarez's Github account

 - Github: https://github.com/Sergio111


Xavier Olivenza's Github account

 - Github: https://github.com/xavierolivenza

## LICENSE

 GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

CITM Terrassa 2016-2017
