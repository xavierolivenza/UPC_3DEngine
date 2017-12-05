#ifndef __ParticleSystem_H__
#define __ParticleSystem_H__

#include <vector>
#include "MathGeoLib\Math\float3.h"
#include "MathGeoLib\Math\float4.h"
#include "MathGeoLib\Math\Quat.h"
#include "MathGeoLib\Geometry\AABB.h"
#include "MathGeoLib\Geometry\Sphere.h"
#include "MathGeoLib\Geometry\Cone.h"
#include "MathGeoLib\Geometry\Circle.h"

#ifdef NULL
#undef NULL
#endif
#define NULL  0

// Deletes a buffer
#ifdef RELEASE
#undef RELEASE
#endif
#define RELEASE( x )	\
{                       \
	if( x != NULL )     \
	{                   \
		delete x;       \
	    x = NULL;       \
	}                   \
}

// Deletes an array of buffers
#ifdef RELEASE_ARRAY
#undef RELEASE_ARRAY
#endif
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
	
	int Lifetime = 0;								//Lifetime of emitted particles
	int LifetimeVariation = 0;						//Lifetime variation of emitted particles
	int EmissionDuration = 0;						//If loop is false, emission is played EmissionDuration ms
	bool Loop = true;								//Ignore EmissionDuration and keep emitting
	int ParticleNumber = 0;							//Max particles emitted at the same time
	float Speed = 0.0f;
	float SpeedVariation = 0.0f;
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
	float Size = 0.0f;
	float SizeVariation = 0.0f;
	float4 RGBATint = float4::one;					//Particle Texture tint
	float4 RGBATintVariation = float4::zero;

	bool alpha_preview = true;
	bool alpha_half_preview = false;
	bool options_menu = true;
	bool alpha = true;
	bool alpha_bar = true;
	bool side_preview = true;
	int inputs_mode = 2;
	int picker_mode = 0;
};

struct ParticleMeshData;

struct ParticleProperties
{
	float3 Position = float3::zero;					//Particle Actual Position
	Quat Rotation = Quat::identity;					//Particle Actual Rotation
	float3 Scale = float3::zero;					//Particle Actual Scale
	ParticleMeshData* MeshResource = nullptr;		//Mesh used by this particle plane/other mesh
	float Speed = 0.0f;								//Particle Speed
	//Accel											//We can add some acceleleration to particles?
	unsigned int LifetimeMax = 0;					//Max Particle Lifetime
	unsigned int LifetimeActual = 0;				//Actual Particle Lifetime
	unsigned int TextureID = 0;						//Texture ID used by this particle
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

	unsigned int num_faces = 0;
	unsigned int id_vertices = 0;					// ID in VRAM
	unsigned int num_vertices = 0;
	float* vertices = nullptr;
	unsigned int id_indices = 0;					// ID in VRAM
	unsigned int num_indices = 0;
	unsigned int* indices = nullptr;
	unsigned int id_normals = 0;					// ID in VRAM
	float* normals = nullptr;
	unsigned int id_texture_coords = 0;				// ID in VRAM
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
	void DrawTexturePreview();
	void DrawColorSelector();
	void DrawEmitterOptions();
	bool CreateParticle();

public:
	bool EditorWindowOpen = false;
	
private:
	ParticleMeshData ParticleMesh;
	std::vector<Particle*> Particles;
	ParticleState InitialState;
	ParticleState FinalState;
	ParticleEmitter Emitter;

	enum
	{
		InitialState_Enum,
		FinalState_Enum
	}ParticleStateEnum = InitialState_Enum;
};

#endif // __ParticleSystem_H__