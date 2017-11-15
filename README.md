# UPC_3DEngine

UPC_3DEngine is a project for the subject 3DEngine of the VideoGame Developing career in CITM, UPC fundation in Terrassa.
The main objective is to create a simple 3D game editor.

## INSTALLATION

Unzip the realese folder and execute the .exe file.

In assets folder, you can find some models to test the engine.

## USAGE

FRUSTUM CULLING:

Load any geometry, open culling window with LAlt + 8 or manually. You can draw octree by pressing F1 or in culling window. Press calc octree button. Then choose one camera, set it to main camera and enable frustum culling in the component camera window. If you add new geometry calculate octree again.

WARNING: 

-If you charge a huge model or a model that has errors, the camera is moved to a far distance without sight of the plane and the axis. Charging a new smaller/correct one will make the camera return to a normal position. You can also open camera options and change position and reference variables to force the camera move where you want.

-When you charge a new module without texture it will automatically charge checkers texture, the objective of that is to see if the UV are correct if you want the model without texture just go to render options and disable GL_TEXTURE_2D.

### CONTROLS

-Camera controls

	-Right click+�WASD� fps-like movement
	
	-Mouse wheel zooms in and out
	
	-Alt+Left click to orbit the object or right click
	
	-F focus the camera around the geometry
	
	-Right click + "QE" lift movement. Q up, E down
	
-Drag any FBX and drop it into the executable to load it

-Drag any texture and drop it into the executable to apply it to loaded geometry

### WINDOWS

-Profiler: This window shows the ms wasted in every module

-Hierarchy: This window shows game object hierarchy, if you clic one node, the inspector window updates with the new data to show

-Inspector: This window shows game object and associated components configuration

-Console: A console that LOGS all the engine process

-Configuration:

	-Application: This window shows useful information of the app, FPS, memory usage, etc... It also allows the user to cap FPS, change app name, etc...
	
	-Window: This window shows different window information and allows the user to modify some window variables
	
	-Hardware: This window shows useful hardware information
	
-Module Variables: This window shows useful information about every module. And allows the user to modifiy some variables in some of them

-File: At the moment does nothing

-View: This window let the user choose which windows visualize. It can also be done with LAlt + number (1 to 5 by now)

-Help: This window has the options to download latest release, report bug, and show the documentation. It also has about, which is a window with useful information like license, libraries used, authors, etc...

-Close: Closes the engine

## CHANGELOG

v0.7(Second Assignment):

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
