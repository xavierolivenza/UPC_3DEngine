#ifndef __ParticleSystem_H__
#define __ParticleSystem_H__

#include <vector>
#include <list>
#include "MathGeoLib\Math\float2.h"
#include "MathGeoLib\Math\float3.h"
#include "MathGeoLib\Math\float4.h"
#include "MathGeoLib\Math\float4x4.h"
#include "MathGeoLib\Math\Quat.h"
#include "MathGeoLib\Geometry\AABB.h"
#include "MathGeoLib\Geometry\Sphere.h"
#include "MathGeoLib\Geometry\Circle.h"

#ifdef NULL
#undef NULL
#endif
#define NULL  0

#ifdef DEGTORAD
#undef DEGTORAD
#endif
#define DEGTORAD 0.0174532925199432957f

#ifdef RADTODEG
#undef RADTODEG
#endif
#define RADTODEG 57.295779513082320876f

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

#define LERP(v0,v1,time) ( v0 + time * (v1 - v0))

#define DEBUG_THICKNESS 4.0f
#define DEBUG_COLOR_R 1.0f
#define DEBUG_COLOR_G 1.0f
#define DEBUG_COLOR_B 0.0f
#define CIRCLES_SEGMENTS 10.0f

struct ConeTrunk
{
	Circle Upper_Circle;
	Circle Bottom_Circle;
	float heigth = 1.0f;
};

class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter();

	void DebugDrawEmitter();
	void DebugDrawEmitterAABB();
	void ResetEmitterValues();

	void SetTransform(const float4x4& transform);

	void GetPosition(float3& pos);
	void GetRotation(Quat& rot);
	void GetScale(float3& sca);

private:
	void DrawSphere(const Sphere& shape);
	void DrawSemiSphere(const Sphere& shape);
	void DrawCone(const ConeTrunk& shape);
	void DrawBox(const AABB& shape);
	void DrawCircle(const Circle& shape);

public:
	float EmitterLifeMax = -1.0f;					//EmitterLife, after that, we can execute another child particle system, if -1, infinite life
	float EmitterLife = 0.0f;						//Emitter Real Life
	float4x4 Transform = float4x4::identity;
	unsigned int SpawnRate = 2;						//How many particles are emitted every second
	float PreviewState = 0.0f;						//Preview of the particle, 0 = initial state, 1 = final state
	float Lifetime = 1.0f;							//Lifetime of emitted particles
	float LifetimeVariation = 0.0f;					//Lifetime variation of emitted particles
	float EmissionDuration = 0.0f;					//If loop is false, emission is played EmissionDuration
	bool Loop = true;								//Ignore EmissionDuration and keep emitting
	int ParticleNumber = 0;							//Alive particles emitted
	float Speed = 5.0f;
	float SpeedVariation = 0.0f;
	float3 Position = float3::zero;					//Emitter position
	Quat Rotation = Quat::identity;					//Emitter rotation
	float3 Scale = float3::zero;					//Emitter scale
	AABB BoundingBox;								//User can set AABB for camera culling purpose (we can add physics...)

	enum
	{
		LocalEmission,
		WorldEmission
	} EmissionType = WorldEmission;
	enum
	{
		EmitterType_Sphere,
		EmitterType_SemiSphere,
		EmitterType_Cone,
		EmitterType_Box,
		EmitterType_Circle
	} Type = EmitterType_Box;
	enum
	{
		Null,
		Billboard,
		VerticalBillboard,
		HorizontalBillboard
	} ParticleFacingOptions = Billboard;
	union EmitterShapeUnion
	{
		EmitterShapeUnion();
		AABB Box_Shape;
		Sphere Sphere_Shape;
		ConeTrunk ConeTrunk_Shape;
		Circle Circle_Shape;
	} EmitterShape;
};

struct ParticleState
{
	float3 force = float3(0.0f, -9.81f, 0.0f);		//Force that effects that particle
	float3 forceVariation = float3(0.0f, 0.0f, 0.0f);//Force that effects that particle variation
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

struct ParticleAssignedState
{
	float3 force = float3::zero;					//Gravity that affects that particle
	float Size = 0.0f;
	float4 RGBATint = float4::one;					//Particle Texture tint
};

struct ParticleMeshData;

struct ParticleProperties
{
	float3 Position = float3::zero;					//Particle Actual Position
	Quat Rotation = Quat::identity;					//Particle Actual Rotation
	float3 Scale = float3::one;						//Particle Actual Scale
	float Size = 0.0f;								//Scale Multiplicator to modify
	float3 Speed = float3::zero;					//Particle Speed
	float3 EmissionDirection = float3::zero;		//Particle Emission Direction
	float3 force = float3::zero;					//Force that effects that particle
	float LifetimeMax = 0;							//Max Particle Lifetime
	float LifetimeActual = 0;						//Actual Particle Lifetime
	unsigned int TextureID = 0;						//Texture ID used by this particle
	float4 RGBATint = float4::zero;					//Particle Texture tint
};

class ParticleSystem;

class Particle
{
public:
	Particle(ParticleSystem* parent, const ParticleState& Initial, const ParticleState& Final, float3 Speed, float LifetimeMax);
	~Particle();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	bool isDead();

	void DrawParticle();

private:
	void SetAssignedStateFromVariables(ParticleAssignedState& AState, const ParticleState& State);
	void CalculateStatesInterpolation();
	void CalculatePosition(float LifetimeFloat);
	void CalculateGravity(float LifetimeFloat);
	void CalculateSize(float LifetimeFloat);
	void CalculateColor(float LifetimeFloat);

public:
	ParticleSystem* ParentParticleSystem = nullptr;
	ParticleProperties Properties;					//Particle Properties
	ParticleAssignedState InitialState;				//Particle Initial State Properties with no variations, all are final values (calculated from ParticleState +- Var)
	ParticleAssignedState FinalState;				//Particle Final State Properties with no variations, all are final values (calculated from ParticleState +- Var)
	bool MeshChanged = false;

private:
	bool ToDelete = false;
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
	//unsigned int id_texture_coords = 0;				// ID in VRAM
	//float* texture_coords = nullptr;
};

struct ParticleTextureData
{
	void Set(unsigned int ID, unsigned int width, unsigned int heigth);

	unsigned int TextureID = 0;
	unsigned int TextureW = 0;
	unsigned int TextureH = 0;
};

struct KeyInput
{
	enum
	{
		Idle,
		Up,
		Down,
		Repeat
	}State = Idle;
};

class ParticleSystem
{
	friend class Particle;
	bool GenerateBuffers = true;

public:
	ParticleSystem();
	~ParticleSystem();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	//void SetMeshResource(ParticleMeshData& MeshData);//Set Mesh resource to use
	void SetMeshResourcePlane();					//Delete actual mesh + load a plane
	void SetTextureResource(unsigned int ID, unsigned int width, unsigned int heigth);

	void SetInitialStateResource(const ParticleState& state);	//Set Particle Initial State if you want to load it as a resource
	void SetFinalStateResource(const ParticleState& state);		//Set Particle Final State if you want to load it as a resource
	void SetEmitterResource(const ParticleEmitter& emitter);	//Set Particle Emitter if you want to load it as a resource
	void GetInitialState(ParticleState& state) const;			//Get Particle Initial State if you want to save it as a resource
	void GetFinalState(ParticleState& state) const;				//Get Particle Final State if you want to save it as a resource
	void GetEmitter(ParticleEmitter& emitter) const;			//Get Particle Emitter if you want to save it as a resource

	void SetEmitterTransform(const float4x4& transform);

	void DebugDrawEmitter();
	void DebugDrawEmitterAABB();
	void DrawImGuiEditorWindow();					//Draw Particle Editor Window

	AABB& GetEmitterAABB();							//You can get the Emitter AABB and edit min and max point with gizmos
	const ParticleMeshData& GetParticleMeshData() const;

	void SetCameraPosToFollow(float3 position);		//Set Camera position which the billboards will face

private:
	void GenerateMeshResourceBuffers();
	void GenerateUVBuffers();
	void GenerateTexturesUVs();
	void DrawTexturePreview();
	void DrawColorSelector();
	void DrawEmitterOptions();
	bool CreateParticle();

public:
	bool EditorWindowOpen = false;
	KeyInput MouseLeftClick;
	float3 CameraPosition = float3::zero;
	bool ShowEmitterBoundBox = false;
	bool ShowEmitter = true;

private:
	ParticleMeshData ParticleMesh;
	std::list<Particle*> Particles; //Should change to list
	ParticleState InitialState;
	ParticleState FinalState;
	ParticleEmitter Emitter;
	ParticleTextureData TextureData;
	std::vector<float4> TexturesUV_Data;		//UV0 = X-Y		UV1 = Z-W
	std::vector<unsigned int> TexturesUV_ID;	//IDs

	float NextParticleTime = 0.0f;

	int columns = 1;
	int rows = 1;
	int numberOfFrames = 1;

	enum
	{
		Right,
		Down
	}AnimationOrder = Right;

	enum
	{
		InitialState_Enum,
		FinalState_Enum
	}ParticleStateEnum = InitialState_Enum;
};

#endif // __ParticleSystem_H__