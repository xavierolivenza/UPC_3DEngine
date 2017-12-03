#pragma once
#include <vector>
#include "Globals.h"
#include "MathGeoLib\Math\float3.h"
#include "MathGeoLib\Math\float4.h"
#include "MathGeoLib\Math\Quat.h"
#include "MathGeoLib\Geometry\AABB.h"
#include "MathGeoLib\Geometry\Sphere.h"
#include "MathGeoLib\Geometry\Cone.h"
#include "MathGeoLib\Geometry\Circle.h"
#include "ResourceMesh.h"

#ifdef NULL
#undef NULL
#endif
#define NULL  0

// Deletes a buffer
#define RELEASE( x )	\
{                       \
	if( x != NULL )     \
	{                   \
		delete x;       \
	    x = NULL;       \
	}                   \
}

// Deletes an array of buffers
#define RELEASE_ARRAY( x )	\
{                           \
	if( x != NULL )         \
	{                       \
		delete[] x;         \
	    x = NULL;           \
	}                       \
}

struct ParticleEmitter
{
	uint Lifetime = 0;								//Lifetime of emitted particles
	uint LifetimeVariation = 0;						//Lifetime variation of emitted particles
	uint EmissionDuration = 0;						//If loop is false, emission is played EmissionDuration ms
	bool Loop = true;								//Ignore EmissionDuration and keep emitting
	uint ParticleNumber = 0;						//Max particles emitted at the same time
	float3 Position = float3::zero;					//Emitter position
	Quat Rotation = Quat::identity;					//Emitter rotation
	float3 Scale = float3::zero;					//Emitter scale
	float3 ExternalForce = float3::zero;			//Particles influenced by external force - Direction + force (vector magnitude)
	float3 ExternalForceVariation = float3::zero;
	//Accel +- Variation							//We can add some acceleleration to particles?
	AABB BoundingBox;								//User can set AABB for camera culling purpose (we can add physics...)

	enum
	{
		EmitterType_Sphere,
		EmitterType_SemiSphere,
		EmitterType_Cone,
		EmitterType_Box,
		EmitterType_Circle,
		EmitterType_Edge
	} Type = EmitterType_Box;
	union EmitterShapeUnion
	{
		EmitterShapeUnion();
		AABB Box_Shape;
		Sphere Sphere_Shape;
		Cone Cone_Shape;
		Circle Circle_Shape;
	} EmitterShape;
};

struct ParticleState
{
	float Speed = 0.0f;								//Particle Speed
	float SpeedVariation = 0.0f;
	float4 RGBATint = float4::zero;					//Particle Texture tint
	float4 RGBATintVariation = float4::zero;
};

struct ParticleProperties
{
	float3 Position = float3::zero;					//Particle Actual Position
	Quat Rotation = Quat::identity;					//Particle Actual Rotation
	float3 Scale = float3::zero;					//Particle Actual Scale
	ResourceMesh* MeshResource = nullptr;			//Mesh used by this particle plane/other mesh
	float Speed = 0.0f;								//Particle Speed
	//Accel											//We can add some acceleleration to particles?
	uint LifetimeMax = 0;							//Max Particle Lifetime
	uint LifetimeActual = 0;						//Actual Particle Lifetime
	uint TextureID = 0;								//Texture ID used by this particle
	float4 RGBATint = float4::zero;					//Particle Texture tint
};

class Particle
{
public:
	Particle();
	~Particle();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	void DrawParticle();

public:
	ParticleProperties Properties;					//Particle Properties
	ParticleState* InitialState = nullptr;			//Particle Initial State Properties
	ParticleState* FinalState = nullptr;			//Particle Final State Properties
};

struct ParticleMeshData								//Very similar to MeshDataResource, but we copy it here to separate as much as we can the particle code from engine code (eayer to export/make a library)
{
	ParticleMeshData();
	~ParticleMeshData();
	void Copy(ParticleMeshData& Other);
	void Clean();

	uint num_faces = 0;
	uint id_vertices = 0;							// ID in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;
	uint id_indices = 0;							// ID in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;
	uint id_normals = 0;							// ID in VRAM
	float* normals = nullptr;
	uint id_texture_coords = 0;						// ID in VRAM
	float* texture_coords = nullptr;
};

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	void SetMeshResource(ParticleMeshData& MeshData);//Set Mesh resource to use
	void SetMeshResourcePlane();					//Delete actual mesh + load a plane
	void DrawImGuiEditorWindow();					//Draw Particle Editor Window

private:
	void DrawColorSelector();
	bool CreateParticle();

public:
	bool EditorWindowOpen = false;
	
private:
	ParticleMeshData ParticleMesh;
	std::vector<Particle*> Particles;
	ParticleState InitialState;
	ParticleState FinalState;
	ParticleEmitter Emitter;
};