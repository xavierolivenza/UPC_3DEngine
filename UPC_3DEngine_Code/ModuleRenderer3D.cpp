#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"

#include "DevIL\include\il.h"
#include "DevIL\include\ilu.h"
#include "DevIL\include\ilut.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "renderer3d";
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOGP("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOGP("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOGP("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOGP("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOGP("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOGP("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		//GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		lights[0].Active(true);

		if (GL_DepthTest)
			glEnable(GL_DEPTH_TEST);
		if (GL_CullFace)
			glEnable(GL_CULL_FACE);
		if (GL_Lighting)
			glEnable(GL_LIGHTING);
		if (GL_ColorMaterial)
			glEnable(GL_COLOR_MATERIAL);
		if (GL_Texture2D)
			glEnable(GL_TEXTURE_2D);
		if (GL_Fog)
			glEnable(GL_FOG);
		if (GL_Diffuse)
			glEnable(GL_DIFFUSE);
		if (GL_Specular)
			glEnable(GL_SPECULAR);
	}

	// Projection matrix for
	//OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

bool ModuleRenderer3D::Start()
{
	checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &id_checkImage);
	glBindTexture(GL_TEXTURE_2D, id_checkImage);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	glBindTexture(GL_TEXTURE_2D, 0);

	//uint w = 0;
	//uint h = 0;
	//uint d = 0;
	//Lenna_tex = App->loadmesh->LoadImageFromFile("Assets/Lenna.png", w, h, d);

	return true;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	/*
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	const ComponentCamera* camera = App->scene->GetActiveCamera();
	if (camera != nullptr)
		glLoadMatrixf(App->scene->GetActiveCamera()->GetViewProjMatrix());
	else
		glLoadMatrixf(App->camera->GetViewMatrix());
	*/
	/**/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//If the engine is in play mode unpaused, try to get main camera matrix
	if (App->GetEngineTimeStatus() == EngineTimeStatus::play_unpause)
	{
		const ComponentCamera* camera = App->scene->GetActiveCamera();
		//If we have main camera, get matrix and load
		if (camera != nullptr)
			glLoadMatrixf(App->scene->GetActiveCamera()->GetViewProjMatrix());
		//If not, continue loading editor matrix
		else
			glLoadMatrixf(App->camera->GetViewMatrix());
	}
	//Load editor camera matrix
	else
		glLoadMatrixf(App->camera->GetViewMatrix());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/**/

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

update_status ModuleRenderer3D::Update(float dt)
{
	if (GL_Wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (GL_Point)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	/*
	//This is not fov, but shows the angle from you are looking the reference point, it's something...
	float* viewmatrix = App->camera->GetViewMatrix();
	float fov = 2.0f * atan(1.0f / viewmatrix[5]) * 180.0f / 3.1416f;
	LOGP("FOV = %f", fov);
	*/

	//glColor3f(1.0f,1.0f,1.0f);

	//draw line
	/*
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 10.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);
	*/

	//draw cube
	/*
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE, ImageName);

	glLineWidth(2.0f);
	glBegin(GL_TRIANGLES);
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f); //4
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f); //8
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 1.0f); //5
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f); //4
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 1.0f); //3
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f); //8
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 1.0f); //3
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f); //7
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 1.0f); //8
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 1.0f); //3
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f); //2
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f); //7
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f); //2
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f); //6
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f); //7
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f); //2
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); //1
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f); //6
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); //1
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 1.0f); //5
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f); //6

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); //1
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1.0f); //4
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 1.0f); //5
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 1.0f); //5
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f); //7
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f); //6
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 1.0f); //5
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f); //8
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f); //7
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 1.0f); //3
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); //1
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f); //2
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 1.0f); //3
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1.0f); //4
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); //1
	
	glEnd();
	glLineWidth(1.0f);

	glBindTexture(GL_TEXTURE, 0);
	glDisable(GL_TEXTURE_2D);
	
	*/

	/*
	static const GLfloat vertex_buffer[] =
	{
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,

		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer), vertex_buffer, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, 12*3*3);
	glDisableClientState(GL_VERTEX_ARRAY);
	*/

	/*
	static std::vector<GLfloat>  vertices =
	{
		0.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,1.0f,1.0f,
		0.0f,1.0f,0.0f,
		1.0f,1.0f,0.0f,
		1.0f,1.0f,1.0f
	};

	static std::vector<uint> indices =
	{
		3,7,4,
		3,2,7,
		2,6,7,
		2,1,6,
		1,5,6,
		1,0,5,
		0,4,5,
		0,3,4,
		4,6,5,
		4,7,6,
		1,3,0,
		2,3,1
	};

	glTexCoord2f(0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	
	glTexCoord2f(0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	
	glTexCoord2f(0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);
	
	glTexCoord2f(0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	
	glTexCoord2f(0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);
	
	glTexCoord2f(0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	
	glTexCoord2f(0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);
	
	glTexCoord2f(0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);
	
	glTexCoord2f(0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);
	
	glTexCoord2f(0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);
	
	glTexCoord2f(0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);

	static GLuint indices_buf = 0;
	static GLuint vertices_buf;

	// Buffer for vertices
	glGenBuffers(1, &vertices_buf);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_buf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) *vertices.size() * 3, &vertices[0], GL_STATIC_DRAW);

	// Buffer for indices
	glGenBuffers(1, &indices_buf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_buf);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	*/

	/*
	//Test to draw a triangle with openGL http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
	//3 vertices
	static const GLfloat g_vertex_buffer_data[] =
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		3.0f, -1.0f, 0.0f,
		2.0f,  1.0f, 0.0f,
	};
	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	glDeleteBuffers(1, &vertexbuffer);


	//test to draw a sphere
	vec sphere_center = { -3,0,0 };
	Sphere *triangle_sphere = new Sphere(sphere_center, 1);
	math::vec outPos[1536];
	math::vec outNormal[1536];
	triangle_sphere->Triangulate(outPos, outNormal, NULL, 1536, false);
	// This will identify our vertex buffer
	GLuint vertexbuffer2;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer2);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(outPos), outPos, GL_STATIC_DRAW);
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 1536); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	glDeleteBuffers(1, &vertexbuffer2);
	*/

	if (GL_Wireframe | GL_Point)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOGP("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}

bool ModuleRenderer3D::Draw(const GeometryData* meshData) const
{
	bool ret = true;

	if ((meshData == nullptr) || (meshData->vertices == nullptr) || (meshData->indices == nullptr))
		return false;

	if (GL_Wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (GL_Point)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	if(DebugVNormals && (meshData->normals != nullptr))
		for (uint i = 0; i < meshData->num_vertices * 3; i += 3)
		{
			glLineWidth(2.0f);
			glColor3f(1.0f, 0.0f, 0.0f);

			glBegin(GL_LINES);
			glVertex3f(meshData->vertices[i], meshData->vertices[i + 1], meshData->vertices[i + 2]);
			glVertex3f(meshData->vertices[i] + meshData->normals[i] * (float)NormalLength, meshData->vertices[i + 1] + meshData->normals[i + 1] * (float)NormalLength, meshData->vertices[i + 2] + meshData->normals[i + 2] * (float)NormalLength);
			glEnd();

			glLineWidth(1.0f);
			glColor3f(1.0f, 1.0f, 1.0f);
		}

	//glEnable(GL_TEXTURE_2D);
	if (glIsEnabled(GL_TEXTURE_2D))
	{
		if (meshData->texture_name != "")
			glBindTexture(GL_TEXTURE_2D, meshData->id_texture);
		else
		{
			glBindTexture(GL_TEXTURE_2D, id_checkImage);
			//glBindTexture(GL_TEXTURE_2D, Lenna_tex);
		}
	}
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, meshData->id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (meshData->normals != nullptr)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, meshData->id_normals);
		glNormalPointer(GL_FLOAT, 0, NULL);
	}
	if (meshData->colors != nullptr) {
		glEnableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, meshData->id_colors);
		glColorPointer(3, GL_FLOAT, 0, NULL);
	}
	if (meshData->texture_coords != nullptr) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, meshData->id_texture_coords);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->id_indices);
	glDrawElements(GL_TRIANGLES, meshData->num_indices, GL_UNSIGNED_INT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisable(GL_TEXTURE_2D);

	if (GL_Wireframe | GL_Point)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	return ret;
}

bool ModuleRenderer3D::Draw(const std::vector<GeometryData>* meshData) const
{
	bool ret = true;

	for (std::vector<GeometryData>::const_iterator item = meshData->begin(); item != meshData->cend() && ret == true; ++item)
		ret = Draw(item._Ptr);

	return ret;
}

bool ModuleRenderer3D::DrawComponentMeshMaterial(const ComponentTransform* transform, const ComponentMesh* mesh, const ComponentMaterial* material) const
{
	if ((mesh == nullptr) || ((mesh->MeshDataStruct.vertices == nullptr) || (mesh->MeshDataStruct.indices == nullptr)))
		return false;

	const MeshData* meshData = &mesh->MeshDataStruct;
	
	if (GL_Wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (GL_Point)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	if (DebugVNormals && (meshData->normals != nullptr))
		for (uint i = 0; i < meshData->num_vertices * 3; i += 3)
		{
			glLineWidth(2.0f);
			glColor3f(1.0f, 0.0f, 0.0f);

			glBegin(GL_LINES);
			glVertex3f(meshData->vertices[i], meshData->vertices[i + 1], meshData->vertices[i + 2]);
			glVertex3f(meshData->vertices[i] + meshData->normals[i] * (float)NormalLength, meshData->vertices[i + 1] + meshData->normals[i + 1] * (float)NormalLength, meshData->vertices[i + 2] + meshData->normals[i + 2] * (float)NormalLength);
			glEnd();

			glLineWidth(1.0f);
			glColor3f(1.0f, 1.0f, 1.0f);
		}

	if (material != nullptr)
	{
		const MaterialData* materialData = &material->MaterialDataStruct;
		if ((materialData != nullptr) && glIsEnabled(GL_TEXTURE_2D))
		{
			if (materialData->texture_name != "")
				glBindTexture(GL_TEXTURE_2D, materialData->id_texture);
			else
			{
				glBindTexture(GL_TEXTURE_2D, id_checkImage);
				//glBindTexture(GL_TEXTURE_2D, Lenna_tex);
			}
		}
	}
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, meshData->id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (meshData->normals != nullptr)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, meshData->id_normals);
		glNormalPointer(GL_FLOAT, 0, NULL);
	}
	if (meshData->colors != nullptr)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, meshData->id_colors);
		glColorPointer(3, GL_FLOAT, 0, NULL);
	}
	if (meshData->texture_coords != nullptr) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, meshData->id_texture_coords);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	}

	if (transform != nullptr)
	{
		glPushMatrix();
		float4x4 matrix = float4x4::identity;
		matrix = transform->GetMatrix();
		glMultMatrixf(matrix.ptr());
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->id_indices);
	glDrawElements(GL_TRIANGLES, meshData->num_indices, GL_UNSIGNED_INT, NULL);

	if (transform != nullptr)
		glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (GL_Wireframe | GL_Point)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	return true;
}

bool ModuleRenderer3D::DrawDebugBox(const float3& p0, const float3& p1, const float3& p2, const float3& p3, const float3& p4, const float3& p5, const float3& p6, const float3& p7) const
{
	return DrawDebugBox(p0, p1, p2, p3, p4, p5, p6, p7, DebugDrawDefaultR, DebugDrawDefaultG, DebugDrawDefaultB, DebugDrawDefaultThickness);
}

bool ModuleRenderer3D::DrawDebugBox(const float3& p0, const float3& p1, const float3& p2, const float3& p3, const float3& p4, const float3& p5, const float3& p6, const float3& p7, float thickness) const
{
	return DrawDebugBox(p0, p1, p2, p3, p4, p5, p6, p7, DebugDrawDefaultR, DebugDrawDefaultG, DebugDrawDefaultB, thickness);
}

bool ModuleRenderer3D::DrawDebugBox(const float3& p0, const float3& p1, const float3& p2, const float3& p3, const float3& p4, const float3& p5, const float3& p6, const float3& p7, float r, float g, float b) const
{

	return DrawDebugBox(p0, p1, p2, p3, p4, p5, p6, p7, r, g, b, DebugDrawDefaultThickness);
}

bool ModuleRenderer3D::DrawDebugBox(const float3& p0, const float3& p1, const float3& p2, const float3& p3, const float3& p4, const float3& p5, const float3& p6, const float3& p7, float r, float g, float b, float thickness) const
{
	float R = r;
	CLAMP(R, 0.0f, 1.0f);
	float G = g;
	CLAMP(G, 0.0f, 1.0f);
	float B = b;
	CLAMP(B, 0.0f, 1.0f);

	glLineWidth(thickness);
	glColor3f(R, G, B);

	glBegin(GL_LINES);

	glVertex3f(p0.x, p0.y, p0.z);
	glVertex3f(p1.x, p1.y, p1.z);

	glVertex3f(p0.x, p0.y, p0.z);
	glVertex3f(p2.x, p2.y, p2.z);

	glVertex3f(p0.x, p0.y, p0.z);
	glVertex3f(p4.x, p4.y, p4.z);

	glVertex3f(p7.x, p7.y, p7.z);
	glVertex3f(p6.x, p6.y, p6.z);

	glVertex3f(p7.x, p7.y, p7.z);
	glVertex3f(p3.x, p3.y, p3.z);

	glVertex3f(p7.x, p7.y, p7.z);
	glVertex3f(p5.x, p5.y, p5.z);

	glVertex3f(p5.x, p5.y, p5.z);
	glVertex3f(p1.x, p1.y, p1.z);

	glVertex3f(p5.x, p5.y, p5.z);
	glVertex3f(p4.x, p4.y, p4.z);

	glVertex3f(p2.x, p2.y, p2.z);
	glVertex3f(p3.x, p3.y, p3.z);

	glVertex3f(p2.x, p2.y, p2.z);
	glVertex3f(p6.x, p6.y, p6.z);

	glVertex3f(p6.x, p6.y, p6.z);
	glVertex3f(p4.x, p4.y, p4.z);

	glVertex3f(p3.x, p3.y, p3.z);
	glVertex3f(p1.x, p1.y, p1.z);

	glEnd();

	glLineWidth(1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);

	return true;
}

bool ModuleRenderer3D::SaveConf(JSON_Object* conf) const
{
	App->parsonjson->SetBool(conf, "GL_DepthTest", GL_DepthTest);
	App->parsonjson->SetBool(conf, "GL_CullFace", GL_CullFace);
	App->parsonjson->SetBool(conf, "GL_Lighting", GL_Lighting);
	App->parsonjson->SetFloat(conf, "Red", LightModelAmbient[0]);
	App->parsonjson->SetFloat(conf,"Green", LightModelAmbient[1]);
	App->parsonjson->SetFloat(conf, "Blue", LightModelAmbient[2]);
	App->parsonjson->SetFloat(conf, "Alpha", LightModelAmbient[3]);
	App->parsonjson->SetBool(conf, "GL_ColorMaterial", GL_ColorMaterial);
	App->parsonjson->SetBool(conf, "GL_Texture2D", GL_Texture2D);
	App->parsonjson->SetBool(conf, "GL_Fog", GL_Fog);
	App->parsonjson->SetBool(conf, "GL_Diffuse", GL_Diffuse);
	App->parsonjson->SetBool(conf, "GL_Specular", GL_Specular);
	App->parsonjson->SetBool(conf, "GL_Wireframe", GL_Wireframe);
	App->parsonjson->SetBool(conf, "GL_Point", GL_Point);
	App->parsonjson->SetBool(conf, "DebugVNormals", DebugVNormals);
	App->parsonjson->SetFloat(conf, "NormalLength", NormalLength);
	return true;
}

bool ModuleRenderer3D::LoadConf(JSON_Object* conf)
{
	GL_DepthTest = App->parsonjson->GetBool(conf, "GL_DepthTest", true);
	GL_CullFace = App->parsonjson->GetBool(conf, "GL_CullFace", true);
	GL_Lighting = App->parsonjson->GetBool(conf, "GL_Lighting", true);
	LightModelAmbient[0] = App->parsonjson->GetFloat(conf, "Red", 0.0f);
	LightModelAmbient[1] = App->parsonjson->GetFloat(conf, "Green", 0.0f);
	LightModelAmbient[2] = App->parsonjson->GetFloat(conf, "Blue", 0.0f);
	LightModelAmbient[3] = App->parsonjson->GetFloat(conf, "Alpha", 1.0f);
	GL_ColorMaterial = App->parsonjson->GetBool(conf, "GL_ColorMaterial", true);
	GL_Texture2D = App->parsonjson->GetBool(conf, "GL_Texture2D", true);
	GL_Fog = App->parsonjson->GetBool(conf, "GL_Fog", false);
	GL_Diffuse = App->parsonjson->GetBool(conf, "GL_Diffuse", false);
	GL_Specular = App->parsonjson->GetBool(conf, "GL_Specular", false);
	GL_Wireframe = App->parsonjson->GetBool(conf, "GL_Wireframe", false);
	GL_Point = App->parsonjson->GetBool(conf, "GL_Point", false);
	DebugVNormals = App->parsonjson->GetBool(conf, "DebugVNormals", false);
	NormalLength = App->parsonjson->GetFloat(conf, "NormalLength", 1);
	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);
	SDL_SetWindowSize(App->window->window, width, height);
	App->camera->UpdateCamFov(width, height);

	/*
	glViewport(0, 0, width, height);
	SDL_SetWindowSize(App->window->window, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f); //Load from main camera editor frustrum, Or main camera if looking from component camera of game object
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	*/
}

void ModuleRenderer3D::ImGuiModuleVariables()
{
	if (ImGui::Checkbox("GL_DEPTH_TEST", &GL_DepthTest))
	{
		if (GL_DepthTest) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);
	}
	if (ImGui::Checkbox("GL_CULL_FACE", &GL_CullFace))
	{
		if (GL_CullFace) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);
	}
	if (ImGui::Checkbox("GL_LIGHTING", &GL_Lighting))
	{
		if (GL_Lighting) glEnable(GL_LIGHTING);
		else glDisable(GL_LIGHTING);
	}
	if (ImGui::ColorEdit4("Light color", LightModelAmbient))
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
	if (ImGui::Checkbox("GL_COLOR_MATERIAL", &GL_ColorMaterial))
	{
		if (GL_ColorMaterial) glEnable(GL_COLOR_MATERIAL);
		else glDisable(GL_COLOR_MATERIAL);
	}
	if (ImGui::Checkbox("GL_TEXTURE_2D", &GL_Texture2D))
	{
		if (GL_Texture2D) glEnable(GL_TEXTURE_2D);
		else glDisable(GL_TEXTURE_2D);
	}
	if (ImGui::Checkbox("GL_FOG", &GL_Fog))
	{
		if (GL_Fog) glEnable(GL_FOG);
		else glDisable(GL_FOG);
	}
	if (ImGui::Checkbox("GL_DIFFUSE", &GL_Diffuse))
	{
		if (GL_Diffuse) glEnable(GL_DIFFUSE);
		else glDisable(GL_DIFFUSE);
	}
	if (ImGui::Checkbox("GL_SPECULAR", &GL_Specular))
	{
		if (GL_Specular) glEnable(GL_SPECULAR);
		else glDisable(GL_SPECULAR);
	}
	ImGui::Checkbox("GL_Wireframe", &GL_Wireframe);
	ImGui::Checkbox("GL_Point", &GL_Point);
	ImGui::Checkbox("DebugVNormals", &DebugVNormals);

	char buffer[10];
	snprintf(buffer, sizeof buffer, "%.2f", NormalLength);
	if (ImGui::InputText("Normal Length", buffer, 10, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal))
		NormalLength = atof(buffer);
}