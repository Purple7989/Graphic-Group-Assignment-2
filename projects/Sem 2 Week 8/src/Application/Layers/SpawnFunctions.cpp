#pragma once
#include "SpawnFunctions.h"

//Resources
#include "Gameplay/GameObject.h"
#include "Gameplay/MeshResource.h"
#include "Graphics/Textures/Texture1D.h"
#include "Graphics/Textures/Texture2D.h"
#include "Graphics/Textures/Texture3D.h"
#include "Graphics/Textures/TextureCube.h"
#include "Graphics/Font.h"
#include "Gameplay/Material.h"

//Components
#include "Gameplay/Components/IComponent.h"
#include "Gameplay/Components/Camera.h"
#include "Gameplay/Components/RotatingBehaviour.h"
#include "Gameplay/Components/RenderComponent.h"
#include "Gameplay/Components/TriggerVolumeEnterBehaviour.h"
#include "Gameplay/Components/SimpleCameraControl.h"
#include "Gameplay/Components/ParticleSystem.h"
#include "Gameplay/Components/BeatTimer.h"
#include "Gameplay/Components/SeekBehaviour.h"
#include "Gameplay/Components/RotatingBehaviour.h"
#include "Gameplay/Components/CharacterController.h"
#include "Gameplay/Components/LevelMover.h"
#include "Gameplay/Components/BackgroundMover.h"
#include "Gameplay/Components/EnvironmentMover.h"
#include "Gameplay/Components/ScoreComponent.h"
#include "Gameplay/Components/VinylAnim.h"
#include "Gameplay/Components/ForegroundMover.h"
#include "Gameplay/Components/BuildingAnim.h"
#include "Gameplay/Components/MaterialSwap.h"
#include "Gameplay/Components/BeatGem.h"

//Physics
#include "Gameplay/Physics/RigidBody.h"
#include "Gameplay/Physics/ICollider.h"
#include "Gameplay/Physics/TriggerVolume.h"
#include "Gameplay/Physics/Colliders/BoxCollider.h"

using namespace Gameplay;
using namespace Gameplay::Physics;
// For spawning general objects
void SpawnFunctions::SpawnObj(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {

	// Tutorial Stuff
	
	GameObject::Sptr Startplatform = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		Startplatform->SetPostion(pos);
		Startplatform->SetRotation({90.0f, 0.0f, 0.0f});
		Startplatform->SetScale(scale);

		Startplatform->Add<LevelMover>();

		// Create and attach a renderer for the monkey
		RenderComponent::Sptr renderer = Startplatform->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this monkey
		RigidBody::Sptr physics = Startplatform->Add<RigidBody>(RigidBodyType::Kinematic);
		//physics->AddCollider(BoxCollider::Create(glm::vec3(1.0f, 1.0f, 1.0f)));


		// FIX THIS //
		ICollider::Sptr Box1 = physics->AddCollider(BoxCollider::Create(glm::vec3(0.87f, 0.5f, 0.4f)));
		Box1->SetPosition(glm::vec3(0.f, 0.f, 0.f));
		Box1->SetScale(glm::vec3(1, 1, 1));

		//if (parent != nullptr) {
		//	parent->AddChild(Startplatform);
		//}
	}
}
// For spawning start/end platforms
void SpawnFunctions::SpawnStartPlat(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr Startplatform = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		Startplatform->SetPostion(pos);
		Startplatform->SetRotation(rot);
		Startplatform->SetScale({ 0.300f, 0.300f, 0.300f });

		Startplatform->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = Startplatform->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = Startplatform->Add<RigidBody>(RigidBodyType::Kinematic);
		physics->AddCollider(BoxCollider::Create(glm::vec3(1.8f, 1.0f, 1.0f)))->SetPosition({ 0,5,0 });;
	}
}
// For spawning beat gems
void SpawnFunctions::SpawnGem(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr MaterialOn, Gameplay::Material::Sptr MaterialOff, std::string ObjName = "DeezNuts",int BeatNum=0,
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr Gem = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		Gem->SetPostion(pos);
		Gem->SetRotation(rot);
		Gem->SetScale(scale);

		//Add Components
		Gem->Add<LevelMover>();
		Gem->Add<RotatingBehaviour>();
		Gem->Add<MaterialSwap>(BeatNum);
		Gem->Add <BeatGem>(BeatNum);
		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = Gem->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(MaterialOff);

		TriggerVolume::Sptr volume = Gem->Add<TriggerVolume>();
		volume->AddCollider(BoxCollider::Create(glm::vec3(0.5f, 0.5f, 0.5f)));
		//volume->SetFlags(TriggerTypeFlags::Statics | TriggerTypeFlags::Kinematics);

		// Add a dynamic rigid body to this object
		//RigidBody::Sptr physics = Startplatform->Add<RigidBody>(RigidBodyType::Kinematic);
		//physics->AddCollider(BoxCollider::Create(glm::vec3(0.5f, 0.5f, 0.5f)));
		// For Gem Colliders X = left/right Y = Up/Down Z = Towards/Away
	}
}
// For spawning Vinyls
void SpawnFunctions::SpawnCollectable(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr Collectable = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		Collectable->SetPostion(pos);
		Collectable->SetRotation(rot);
		Collectable->SetScale(scale);

		//Add Components
		Collectable->Add<LevelMover>();
		Collectable->Add<VinylAnim>();
		Collectable->Add<RotatingBehaviour>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = Collectable->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		//RigidBody::Sptr physics = Startplatform->Add<RigidBody>(RigidBodyType::Kinematic);
		// For Colliders X is towards Cam, Y is up/down , Z is Left and Right
		//ICollider::Sptr CollectCollider = physics->AddCollider(BoxCollider::Create(glm::vec3(0.5f, 0.5f, 0.5f)));

		TriggerVolume::Sptr volume = Collectable->Add<TriggerVolume>();
		volume->AddCollider(BoxCollider::Create(glm::vec3(0.5f, 0.5f, 0.5f)));
		//volume->SetPostion(glm::vec3(0.0f, 0.5f, 0.0f));
	}
}
// For spawning CD Collectables
void SpawnFunctions::SpawnCD(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr CD = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		CD->SetPostion(pos);
		CD->SetRotation(rot);
		CD->SetScale(scale);

		//Add Components
		CD->Add<LevelMover>();
		CD->Add<RotatingBehaviourCD>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = CD->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		//RigidBody::Sptr physics = Startplatform->Add<RigidBody>(RigidBodyType::Kinematic);
		//physics->AddCollider(BoxCollider::Create(glm::vec3(0.5f, 0.5f, 0.5f)));

		TriggerVolume::Sptr volume = CD->Add<TriggerVolume>();
		volume->AddCollider(BoxCollider::Create(glm::vec3(0.5f, 0.5f, 0.5f)));
	}
}
// For spawning Wall Jump Platforms
void SpawnFunctions::SpawnWallJump(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr WallJump = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		WallJump->SetPostion(pos);
		WallJump->SetRotation(rot);
		WallJump->SetScale(scale);

		WallJump->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = WallJump->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = WallJump->Add<RigidBody>(RigidBodyType::Kinematic);
		// For Wall Jump Colliders, X = Left/Right Y = towards/away, z = Up/Down
		ICollider::Sptr CollectCollider = physics->AddCollider(BoxCollider::Create(glm::vec3(0.3f, 0.5f, 3.2f)));
		CollectCollider->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	}
}
// For spawning building1 object
void SpawnFunctions::SpawnBuilding(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr Building = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		Building->SetPostion(pos);
		Building->SetRotation(rot);
		Building->SetScale(scale);

		Building->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = Building->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = Building->Add<RigidBody>(RigidBodyType::Kinematic);
		// For Wall Jump Colliders, X = Left/Right Y = towards/away, z = Up/Down
		ICollider::Sptr CollectCollider = physics->AddCollider(BoxCollider::Create(glm::vec3(1.590f, 6.250f, 1.800f)));
		CollectCollider->SetPosition(glm::vec3(-0.020f, 0.110f, -0.010f));
	}
}
// For spawning building2 object
void SpawnFunctions::SpawnBuilding2(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr Building2 = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		Building2->SetPostion(pos);
		Building2->SetRotation(rot);
		Building2->SetScale(scale);

		Building2->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = Building2->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = Building2->Add<RigidBody>(RigidBodyType::Kinematic);
		// For Wall Jump Colliders, X = Left/Right Y = towards/away, z = Up/Down
		ICollider::Sptr CollectCollider = physics->AddCollider(BoxCollider::Create(glm::vec3(1.700f, 3.500f, 2.000f)));
		CollectCollider->SetPosition(glm::vec3(0.570f, -3.230f, 1.150f));
	}
}
// For spawning building3 object
void SpawnFunctions::SpawnBuilding3(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr Building3 = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		Building3->SetPostion(pos);
		Building3->SetRotation(rot);
		Building3->SetScale(scale);

		Building3->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = Building3->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = Building3->Add<RigidBody>(RigidBodyType::Kinematic);
		// For Wall Jump Colliders, X = Left/Right Y = towards/away, z = Up/Down
		ICollider::Sptr CollectCollider = physics->AddCollider(BoxCollider::Create(glm::vec3(1.500f, 4.800f, 1.700f)));
		CollectCollider->SetPosition(glm::vec3(-0.040f, -0.990f, 0.000f));
	}
}

// For spawning Small Wall Jump Platforms
void SpawnFunctions::SpawnSmallWallJump(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr SmallWallJump = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		SmallWallJump->SetPostion(pos);
		SmallWallJump->SetRotation(rot);
		SmallWallJump->SetScale(scale);

		SmallWallJump->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = SmallWallJump->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = SmallWallJump->Add<RigidBody>(RigidBodyType::Kinematic);
		// For Wall Jump Colliders, X = Left/Right Y = towards/away, z = Up/Down
		ICollider::Sptr CollectCollider = physics->AddCollider(BoxCollider::Create(glm::vec3(0.3f, 0.5f, 2.5f)));
		CollectCollider->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	}
}
// For spawning very small wall jumps
void SpawnFunctions::SpawnSuperSmallWallJump(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr SuperSmallWallJump = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		SuperSmallWallJump->SetPostion(pos);
		SuperSmallWallJump->SetRotation(rot);
		SuperSmallWallJump->SetScale(scale);

		SuperSmallWallJump->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = SuperSmallWallJump->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = SuperSmallWallJump->Add<RigidBody>(RigidBodyType::Kinematic);
		// For Wall Jump Colliders, X = Left/Right Y = towards/away, z = Up/Down
		ICollider::Sptr CollectCollider = physics->AddCollider(BoxCollider::Create(glm::vec3(0.100f, 0.200f, 0.800f)));
		CollectCollider->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	}
}
// For spawning right facing stairs
void SpawnFunctions::SpawnStairsRight(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr StairsRight = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		StairsRight->SetPostion(pos);
		StairsRight->SetRotation(rot);
		StairsRight->SetScale(scale);

		StairsRight->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = StairsRight->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = StairsRight->Add<RigidBody>(RigidBodyType::Kinematic);
		// For Wall Jump Colliders, X = Left/Right Y = towards/away, z = Up/Down
		ICollider::Sptr CollectCollider = physics->AddCollider(BoxCollider::Create(glm::vec3(0.100f, 0.200f, 0.800f)));
		CollectCollider->SetPosition(glm::vec3(-0.120f, 0.460f, 1.030f));
		CollectCollider->SetScale(glm::vec3(13.138, 7.218, 0.5));
		CollectCollider->SetRotation(glm::vec3(68.0f, 0.000f, 0.00f));
	}
}
// For spawning Left facing stairs
void SpawnFunctions::SpawnStairsLeft(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr StairsLeft = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		StairsLeft->SetPostion(pos);
		StairsLeft->SetRotation(rot);
		StairsLeft->SetScale(scale);

		StairsLeft->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = StairsLeft->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = StairsLeft->Add<RigidBody>(RigidBodyType::Kinematic);
		// For Wall Jump Colliders, X = Left/Right Y = towards/away, z = Up/Down
		ICollider::Sptr CollectCollider = physics->AddCollider(BoxCollider::Create(glm::vec3(0.100f, 0.200f, 0.800f)));
		CollectCollider->SetPosition(glm::vec3(-0.120f, 0.460f, 1.030f));
		CollectCollider->SetScale(glm::vec3(13.138, 7.218, 0.5));
		CollectCollider->SetRotation(glm::vec3(68.0f, 0.000f, 0.00f));
	}
}
// For spawning small speakers
void SpawnFunctions::SpawnSpeaker(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr Speaker = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		Speaker->SetPostion(pos);
		Speaker->SetRotation(rot);
		Speaker->SetScale(scale);

		Speaker->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = Speaker->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);
	}
}
// For spawning Square platforms
void SpawnFunctions::SpawnSquarePlat(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr SquarePlat = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		SquarePlat->SetPostion(pos);
		SquarePlat->SetRotation(rot);
		SquarePlat->SetScale(scale);

		SquarePlat->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = SquarePlat->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = SquarePlat->Add<RigidBody>(RigidBodyType::Kinematic);
		// For Wall Jump Colliders, X = Left/Right Y = towards/away, z = Up/Down
		ICollider::Sptr CollectCollider = physics->AddCollider(BoxCollider::Create(glm::vec3(0.100f, 0.200f, 0.800f)));
		CollectCollider->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	}
}
// For spawning Floating Street Lights
void SpawnFunctions::SpawnFloatingLights(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr FloatingLights = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		FloatingLights->SetPostion(pos);
		FloatingLights->SetRotation(rot);
		FloatingLights->SetScale(scale);

		FloatingLights->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = FloatingLights->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = FloatingLights->Add<RigidBody>(RigidBodyType::Kinematic);
		// For Wall Jump Colliders, X = Left/Right Y = towards/away, z = Up/Down
		ICollider::Sptr CollectCollider = physics->AddCollider(BoxCollider::Create(glm::vec3(0.100f, 0.200f, 0.800f)));
		CollectCollider->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	}
}
// For spawning background cars
void SpawnFunctions::SpawnBackGroundCar(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr Car1 = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		Car1->SetPostion(pos);
		Car1->SetRotation(rot);
		Car1->SetScale(scale);

		//Add Components
		Car1->Add<BackgroundMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = Car1->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Is background Object and therefore has no colliders
		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = Car1->Add<RigidBody>(RigidBodyType::Kinematic);
	}
}
// For spawning Foreground cars
void SpawnFunctions::SpawnForeGroundCar(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr Car1 = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		Car1->SetPostion(pos);
		Car1->SetRotation(rot);
		Car1->SetScale(scale);

		//Add Components
		Car1->Add<ForeGroundMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = Car1->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Is background Object and therefore has no colliders
		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = Car1->Add<RigidBody>(RigidBodyType::Kinematic);
	}
}
// For spawning Background Buildings
void SpawnFunctions::SpawnBackGroundBuilding(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr KBuilding = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		KBuilding->SetPostion(pos);
		KBuilding->SetRotation(rot);
		KBuilding->SetScale(scale);

		//Add Components
		// Background mover ment for cars
		KBuilding->Add<BackgroundBuildingMover>();
		KBuilding->Add<BuildingAnim>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = KBuilding->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Is background Object and therefore has no colliders
		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = KBuilding->Add<RigidBody>(RigidBodyType::Kinematic);
	}
}
