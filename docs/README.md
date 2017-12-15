
#Web README Test

# UPC_3DEngine

UPC_3DEngine is a project for the subject 3DEngine of the VideoGame Developing career in CITM, UPC fundation in Terrassa.
The main objective is to create a simple 3D game editor.

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

-If you charge a huge model or a model that has errors, the camera is moved to a far distance without sight of the plane and the axis. Charging a new smaller/correct one will make the camera return to a normal position. You can also open camera options and change position and reference variables to force the camera move where you want.

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

-Create: One option Create Camera to create one gameobject with camera.

-Help: This window has the options to download latest release, report bug, and show the documentation. It also has about, which is a window with useful information like license, libraries used, authors, etc...

-Close: Closes the engine

## Innovation grade candidates

-Adaptative Octree

-Gizmos

## CHANGELOG

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

Web Page: https://xavierolivenza.github.io/UPC_3DEngine/

Sergio Alvarez's Github account

 - Github: https://github.com/Sergio111


Xavier Olivenza's Github account

 - Github: https://github.com/xavierolivenza

## LICENSE

 GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

CITM Terrassa 2016-2017
