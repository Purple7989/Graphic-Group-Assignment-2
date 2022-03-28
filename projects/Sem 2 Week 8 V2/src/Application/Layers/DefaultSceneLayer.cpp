#include "DefaultSceneLayer.h"


// GLM math library
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/random.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/common.hpp> // for fmod (floating modulus)

#include <filesystem>

// Graphics
#include "Graphics/Buffers/IndexBuffer.h"
#include "Graphics/Buffers/VertexBuffer.h"
#include "Graphics/VertexArrayObject.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Textures/Texture2D.h"
#include "Graphics/Textures/TextureCube.h"
#include "Graphics/VertexTypes.h"
#include "Graphics/Font.h"
#include "Graphics/GuiBatcher.h"
#include "Graphics/Framebuffer.h"

// Utilities
#include "Utils/MeshBuilder.h"
#include "Utils/MeshFactory.h"
#include "Utils/ObjLoader.h"
#include "Utils/ImGuiHelper.h"
#include "Utils/ResourceManager/ResourceManager.h"
#include "Utils/FileHelpers.h"
#include "Utils/JsonGlmHelpers.h"
#include "Utils/StringUtils.h"
#include "Utils/GlmDefines.h"

// Gameplay
#include "Gameplay/Material.h"
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Gameplay/Components/Light.h"

// Components
#include "Gameplay/Components/IComponent.h"
#include "Gameplay/Components/Camera.h"
#include "Gameplay/Components/RotatingBehaviour.h"
#include "Gameplay/Components/JumpBehaviour.h"
#include "Gameplay/Components/RenderComponent.h"
#include "Gameplay/Components/MaterialSwapBehaviour.h"
#include "Gameplay/Components/TriggerVolumeEnterBehaviour.h"
#include "Gameplay/Components/SimpleCameraControl.h"

// Physics
#include "Gameplay/Physics/RigidBody.h"
#include "Gameplay/Physics/Colliders/BoxCollider.h"
#include "Gameplay/Physics/Colliders/PlaneCollider.h"
#include "Gameplay/Physics/Colliders/SphereCollider.h"
#include "Gameplay/Physics/Colliders/ConvexMeshCollider.h"
#include "Gameplay/Physics/Colliders/CylinderCollider.h"
#include "Gameplay/Physics/TriggerVolume.h"
#include "Graphics/DebugDraw.h"

// GUI
#include "Gameplay/Components/GUI/RectTransform.h"
#include "Gameplay/Components/GUI/GuiPanel.h"
#include "Gameplay/Components/GUI/GuiText.h"
#include "Gameplay/InputEngine.h"

#include "Application/Application.h"
#include "Gameplay/Components/ParticleSystem.h"
#include "Graphics/Textures/Texture3D.h"
#include "Graphics/Textures/Texture1D.h"

DefaultSceneLayer::DefaultSceneLayer() :
	ApplicationLayer()
{
	Name = "Default Scene";
	Overrides = AppLayerFunctions::OnAppLoad;
}

DefaultSceneLayer::~DefaultSceneLayer() = default;

void DefaultSceneLayer::OnAppLoad(const nlohmann::json& config) {
	_CreateScene();
}

using namespace Gameplay;
using namespace Gameplay::Physics;
void DefaultSceneLayer::SpawnObj(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {

	// Tutorial Stuff

	GameObject::Sptr Startplatform = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		Startplatform->SetPostion(pos);
		Startplatform->SetRotation({ 90.0f, 0.0f, 0.0f });
		Startplatform->SetScale(scale);

		//Startplatform->Add<LevelMover>();

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
void DefaultSceneLayer::SpawnStartPlat(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
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

		//Startplatform->Add<LevelMover>();

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
void DefaultSceneLayer::SpawnGem(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr MaterialOn, Gameplay::Material::Sptr MaterialOff, std::string ObjName = "DeezNuts", int BeatNum = 0,
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
		//Gem->Add<LevelMover>();
		Gem->Add<RotatingBehaviour>();
		//Gem->Add<MaterialSwap>(BeatNum);
		//Gem->Add <BeatGem>(BeatNum);
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
void DefaultSceneLayer::SpawnCollectable(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
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
		//Collectable->Add<LevelMover>();
		//Collectable->Add<VinylAnim>();
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
void DefaultSceneLayer::SpawnCD(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
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
		//CD->Add<LevelMover>();
		//CD->Add<RotatingBehaviourCD>();

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
void DefaultSceneLayer::SpawnWallJump(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
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

		//WallJump->Add<LevelMover>();

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
void DefaultSceneLayer::SpawnBuilding(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
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

		//Building->Add<LevelMover>();

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
void DefaultSceneLayer::SpawnBuilding2(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
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

		//Building2->Add<LevelMover>();

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
void DefaultSceneLayer::SpawnBuilding3(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
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

	//	Building3->Add<LevelMover>();

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
void DefaultSceneLayer::SpawnSmallWallJump(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
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

		//SmallWallJump->Add<LevelMover>();

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
void DefaultSceneLayer::SpawnSuperSmallWallJump(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
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

		//SuperSmallWallJump->Add<LevelMover>();

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
void DefaultSceneLayer::SpawnStairsRight(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
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

		//StairsRight->Add<LevelMover>();

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
void DefaultSceneLayer::SpawnStairsLeft(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
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

		//StairsLeft->Add<LevelMover>();

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
void DefaultSceneLayer::SpawnSpeaker(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
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

		//Speaker->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = Speaker->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);
	}
}
// For spawning Square platforms
void DefaultSceneLayer::SpawnSquarePlat(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
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

		//SquarePlat->Add<LevelMover>();

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
void DefaultSceneLayer::SpawnFloatingLights(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
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

		//FloatingLights->Add<LevelMover>();

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
void DefaultSceneLayer::SpawnBackGroundCar(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
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
		//Car1->Add<BackgroundMover>();

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
void DefaultSceneLayer::SpawnForeGroundCar(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
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
		//Car1->Add<ForeGroundMover>();

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
void DefaultSceneLayer::SpawnBackGroundBuilding(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
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
		//KBuilding->Add<BackgroundBuildingMover>();
		//KBuilding->Add<BuildingAnim>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = KBuilding->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Is background Object and therefore has no colliders
		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = KBuilding->Add<RigidBody>(RigidBodyType::Kinematic);
	}
}


void DefaultSceneLayer::_CreateScene()
{
	using namespace Gameplay;
	using namespace Gameplay::Physics;

	Application& app = Application::Get();

	bool loadScene = false;
	// For now we can use a toggle to generate our scene vs load from file
	if (loadScene && std::filesystem::exists("scene.json")) {
		app.LoadScene("scene.json");
	} else {
		 
		// Basic gbuffer generation with no vertex manipulation
		ShaderProgram::Sptr deferredForward = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/basic.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/deferred_forward.glsl" }
		});
		deferredForward->SetDebugName("Deferred - GBuffer Generation");  

		// Our foliage shader which manipulates the vertices of the mesh
		ShaderProgram::Sptr foliageShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/foliage.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/deferred_forward.glsl" }
		});  
		foliageShader->SetDebugName("Foliage");   

		// This shader handles our multitexturing example
		ShaderProgram::Sptr multiTextureShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/vert_multitextured.glsl" },  
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/frag_multitextured.glsl" }
		});
		multiTextureShader->SetDebugName("Multitexturing"); 

		// This shader handles our displacement mapping example
		ShaderProgram::Sptr displacementShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/displacement_mapping.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/deferred_forward.glsl" }
		});
		displacementShader->SetDebugName("Displacement Mapping");

		// This shader handles our cel shading example
		ShaderProgram::Sptr celShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/displacement_mapping.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/cel_shader.glsl" }
		});
		celShader->SetDebugName("Cel Shader");


		// Load in the meshes
		MeshResource::Sptr monkeyMesh = ResourceManager::CreateAsset<MeshResource>("Monkey.obj");
		MeshResource::Sptr shipMesh   = ResourceManager::CreateAsset<MeshResource>("fenrir.obj");
		
		// Load in some textures
		Texture2D::Sptr    boxTexture   = ResourceManager::CreateAsset<Texture2D>("textures/box-diffuse.png");
		Texture2D::Sptr    boxSpec      = ResourceManager::CreateAsset<Texture2D>("textures/box-specular.png");
		Texture2D::Sptr    monkeyTex    = ResourceManager::CreateAsset<Texture2D>("textures/monkey-uvMap.png");
		Texture2D::Sptr    leafTex      = ResourceManager::CreateAsset<Texture2D>("textures/leaves.png");

		MeshResource::Sptr SmallPlatform = ResourceManager::CreateAsset<MeshResource>("HaloBasicPlatform.obj");
		MeshResource::Sptr WallJump = ResourceManager::CreateAsset<MeshResource>("WallJumpV6.obj");
		MeshResource::Sptr BeatGem = ResourceManager::CreateAsset<MeshResource>("Gem.obj");
		MeshResource::Sptr Vinyl = ResourceManager::CreateAsset<MeshResource>("VinylV2.obj");
		MeshResource::Sptr CD = ResourceManager::CreateAsset<MeshResource>("CDwithUnwrap.obj");
		MeshResource::Sptr Building = ResourceManager::CreateAsset<MeshResource>("RBuilding.obj");
		MeshResource::Sptr KBuilding1Mesh = ResourceManager::CreateAsset<MeshResource>("KBuilding.obj");
		MeshResource::Sptr KBuilding2Mesh = ResourceManager::CreateAsset<MeshResource>("KBuilding2.obj");
		MeshResource::Sptr KBuilding3Mesh = ResourceManager::CreateAsset<MeshResource>("KBuilding3.obj");
		MeshResource::Sptr OvalBuilding = ResourceManager::CreateAsset<MeshResource>("OvalBuilding.obj");
		MeshResource::Sptr CharacterMesh = ResourceManager::CreateAsset<MeshResource>("discobot.obj");
		MeshResource::Sptr DiscoBallMesh = ResourceManager::CreateAsset<MeshResource>("DiscoBall2.obj");
		MeshResource::Sptr StartPlatform = ResourceManager::CreateAsset<MeshResource>("startbuilding.obj");
		MeshResource::Sptr Car1Mesh = ResourceManager::CreateAsset<MeshResource>("FutureCar1.obj");
		MeshResource::Sptr SemiTruckMesh = ResourceManager::CreateAsset<MeshResource>("Semitruck.obj");
		MeshResource::Sptr PickupTruckMesh = ResourceManager::CreateAsset<MeshResource>("FuturePickup.obj");
		MeshResource::Sptr SmallWallJump = ResourceManager::CreateAsset<MeshResource>("SmallWallJump.obj");
		MeshResource::Sptr SuperSmallWallJump = ResourceManager::CreateAsset<MeshResource>("SuperSmallWallJump.obj");
		MeshResource::Sptr FallingPlat = ResourceManager::CreateAsset<MeshResource>("pianoplatform.obj");
		MeshResource::Sptr HalfCirclePlat = ResourceManager::CreateAsset<MeshResource>("HalfCriclePlat.obj");
		MeshResource::Sptr StairsRight = ResourceManager::CreateAsset<MeshResource>("SlantedStairCase.obj");
		MeshResource::Sptr StairsLeft = ResourceManager::CreateAsset<MeshResource>("StairCaseL.obj");
		MeshResource::Sptr Speaker = ResourceManager::CreateAsset<MeshResource>("speaker.obj");
		MeshResource::Sptr SquarePlat = ResourceManager::CreateAsset<MeshResource>("SquarePlatform.obj");
		MeshResource::Sptr FloatingLight = ResourceManager::CreateAsset<MeshResource>("FloatingStreetLight.obj");
		MeshResource::Sptr DiscoBotMesh1 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/run1.obj");
		MeshResource::Sptr DiscoBotMesh2 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/run2.obj");
		MeshResource::Sptr DiscoBotMesh3 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/run3.obj");
		MeshResource::Sptr DiscoBotMesh4 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/run4.obj");
		MeshResource::Sptr DiscoBotMesh5 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/run5.obj");
		MeshResource::Sptr DiscoBotMesh6 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/run6.obj");
		MeshResource::Sptr DiscoBotMesh7 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/run7.obj");
		MeshResource::Sptr DiscoBotMesh8 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/run8.obj");
		MeshResource::Sptr DiscoBotMesh9 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/run9.obj");
		MeshResource::Sptr BotJump1 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/jump1.obj");
		MeshResource::Sptr BotJump2 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/jump2.obj");
		MeshResource::Sptr BotJump3 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/jump3.obj");
		MeshResource::Sptr BotJump4 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/jump4.obj");
		MeshResource::Sptr BotJump5 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/jump5.obj");
		MeshResource::Sptr BotJump6 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/jump6.obj");
		MeshResource::Sptr BotJump7 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/jump7.obj");

		Texture2D::Sptr StartTex = ResourceManager::CreateAsset<Texture2D>("textures/startbuildingtex.png");
		Texture2D::Sptr SmallTex = ResourceManager::CreateAsset<Texture2D>("textures/HaloBasicPlatformTex.png");
		Texture2D::Sptr VinylTex = ResourceManager::CreateAsset<Texture2D>("textures/VinylTex.png");
		Texture2D::Sptr CDTex = ResourceManager::CreateAsset<Texture2D>("textures/CDTex.png");
		Texture2D::Sptr GemTex = ResourceManager::CreateAsset<Texture2D>("textures/Gem.png");
		Texture2D::Sptr GemOff = ResourceManager::CreateAsset<Texture2D>("textures/GemOff.png");
		Texture2D::Sptr GemAnticipation = ResourceManager::CreateAsset<Texture2D>("textures/GemYellow.png");
		Texture2D::Sptr CharacterTex = ResourceManager::CreateAsset<Texture2D>("textures/shirt.png");
		Texture2D::Sptr LoseScreenTex = ResourceManager::CreateAsset<Texture2D>("textures/Game_Over_Screen.png");
		Texture2D::Sptr SmallWallJumpTex = ResourceManager::CreateAsset<Texture2D>("textures/SmallWallJumpTexBlue.png");
		Texture2D::Sptr SuperSmallWallJumpTex = ResourceManager::CreateAsset<Texture2D>("textures/SmallWallJumpTexRed.png");
		Texture2D::Sptr WallJumpTex = ResourceManager::CreateAsset<Texture2D>("textures/WallJumpTex.png");
		Texture2D::Sptr Car1Tex = ResourceManager::CreateAsset<Texture2D>("textures/FutureCarReTex2.png");
		Texture2D::Sptr SemiTruckTex = ResourceManager::CreateAsset<Texture2D>("textures/SemiTruckTexV2.png");
		Texture2D::Sptr PickupTruckTex = ResourceManager::CreateAsset<Texture2D>("textures/PickupTruckTexV3.png");
		Texture2D::Sptr KBuilding1Tex = ResourceManager::CreateAsset<Texture2D>("textures/KBuildingTex.png");
		Texture2D::Sptr KBuilding2Tex = ResourceManager::CreateAsset<Texture2D>("textures/KBuilding2Tex.png");
		Texture2D::Sptr KBuilding3Tex = ResourceManager::CreateAsset<Texture2D>("textures/KBuilding3Tex.png");
		Texture2D::Sptr BuildingTex = ResourceManager::CreateAsset<Texture2D>("textures/RBuildingTex.png");
		Texture2D::Sptr OvalBuildingTex = ResourceManager::CreateAsset<Texture2D>("textures/OvalBuildingTex.png");
		Texture2D::Sptr DiscoBallTex = ResourceManager::CreateAsset<Texture2D>("textures/DiscoBallTexV2.png");
		Texture2D::Sptr FallingPlatTex = ResourceManager::CreateAsset<Texture2D>("textures/pianotex.png");
		Texture2D::Sptr HalfCirclePlatTex = ResourceManager::CreateAsset<Texture2D>("textures/halfCircleTex.png");
		Texture2D::Sptr TexBeatLogo = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BeatLogo.png");
		Texture2D::Sptr TexPlayButton = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BPlay.png");
		Texture2D::Sptr TexOptionsButton = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BOptions.png");
		Texture2D::Sptr TexMusicButton = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BMusic.png");
		Texture2D::Sptr TexCreditsButton = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BCredits.png");
		Texture2D::Sptr TexQuitButton = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BQuit.png");
		Texture2D::Sptr TexResumeButton = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BResume.png");
		Texture2D::Sptr TexResyncButton = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BResync.png");
		Texture2D::Sptr TexContinueButton = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BContinue.png");
		Texture2D::Sptr TexPauseMenu = ResourceManager::CreateAsset<Texture2D>("textures/GUI/PauseMenuBG.png");
		Texture2D::Sptr TexDimmedBG = ResourceManager::CreateAsset<Texture2D>("textures/GUI/DimBG.png");
		Texture2D::Sptr TexScoreBreakdown = ResourceManager::CreateAsset<Texture2D>("textures/GUI/ScoreBreakdown.png");
		Texture2D::Sptr TexGameOverText = ResourceManager::CreateAsset<Texture2D>("textures/GUI/GameOverText.png");
		Texture2D::Sptr TexMovementTutorial = ResourceManager::CreateAsset<Texture2D>("textures/GUI/Movement.png");
		Texture2D::Sptr TexWallJumpTutorial = ResourceManager::CreateAsset<Texture2D>("textures/GUI/WallJump.png");
		Texture2D::Sptr TexBeatGemTutorial = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BeatGems.png");
		Texture2D::Sptr TexVinylsTutorial = ResourceManager::CreateAsset<Texture2D>("textures/GUI/Vinyls.png");
		Texture2D::Sptr StairsRightTex = ResourceManager::CreateAsset<Texture2D>("textures/StairCaseTex.png");
		Texture2D::Sptr StairsLeftTex = ResourceManager::CreateAsset<Texture2D>("textures/StarTexL.png");
		Texture2D::Sptr SpeakerTex = ResourceManager::CreateAsset<Texture2D>("textures/speakertex.png");
		Texture2D::Sptr SquarePlatTex = ResourceManager::CreateAsset<Texture2D>("textures/SquarePlatformTex.png");
		Texture2D::Sptr FloatingLightTex = ResourceManager::CreateAsset<Texture2D>("textures/StreetLightTex.png");
		Texture2D::Sptr TexBeatBar = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BeatBar.png");
		Texture2D::Sptr TexBeatBarTick = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BeatBarTick.png");
		Texture2D::Sptr TexScoreDisplay = ResourceManager::CreateAsset<Texture2D>("textures/GUI/ScoreDisplay.png");
		Texture2D::Sptr TexNavigationLeftRight = ResourceManager::CreateAsset<Texture2D>("textures/GUI/NavigationLeftRight.png");
		Texture2D::Sptr TexNavigationUpDown = ResourceManager::CreateAsset<Texture2D>("textures/GUI/NavigationUpDown.png");
		//leafTex->SetMinFilter(MinFilter::Nearest);
		//leafTex->SetMagFilter(MagFilter::Nearest);

#pragma region Basic Texture Creation
		Texture2DDescription singlePixelDescriptor;
		singlePixelDescriptor.Width = singlePixelDescriptor.Height = 1;
		singlePixelDescriptor.Format = InternalFormat::RGB8;

		float normalMapDefaultData[3] = { 0.5f, 0.5f, 1.0f };
		Texture2D::Sptr normalMapDefault = ResourceManager::CreateAsset<Texture2D>(singlePixelDescriptor);
		normalMapDefault->LoadData(1, 1, PixelFormat::RGB, PixelType::Float, normalMapDefaultData);

		float solidBlack[3] = { 0.5f, 0.5f, 0.5f };
		Texture2D::Sptr solidBlackTex = ResourceManager::CreateAsset<Texture2D>(singlePixelDescriptor);
		solidBlackTex->LoadData(1, 1, PixelFormat::RGB, PixelType::Float, solidBlack);

		float solidGrey[3] = { 0.0f, 0.0f, 0.0f };
		Texture2D::Sptr solidGreyTex = ResourceManager::CreateAsset<Texture2D>(singlePixelDescriptor);
		solidGreyTex->LoadData(1, 1, PixelFormat::RGB, PixelType::Float, solidGrey);

		float solidWhite[3] = { 1.0f, 1.0f, 1.0f };
		Texture2D::Sptr solidWhiteTex = ResourceManager::CreateAsset<Texture2D>(singlePixelDescriptor);
		solidWhiteTex->LoadData(1, 1, PixelFormat::RGB, PixelType::Float, solidWhite);

#pragma endregion 

		// Loading in a 1D LUT
		Texture1D::Sptr toonLut = ResourceManager::CreateAsset<Texture1D>("luts/toon-1D.png"); 
		toonLut->SetWrap(WrapMode::ClampToEdge);

		// Here we'll load in the cubemap, as well as a special shader to handle drawing the skybox
		TextureCube::Sptr testCubemap = ResourceManager::CreateAsset<TextureCube>("cubemaps/ocean/ocean.jpg");
		ShaderProgram::Sptr      skyboxShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/skybox_vert.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/skybox_frag.glsl" } 
		});
		  
		// Create an empty scene
		Scene::Sptr scene = std::make_shared<Scene>();  

		// Setting up our enviroment map
		scene->SetSkyboxTexture(testCubemap); 
		scene->SetSkyboxShader(skyboxShader);
		// Since the skybox I used was for Y-up, we need to rotate it 90 deg around the X-axis to convert it to z-up 
		scene->SetSkyboxRotation(glm::rotate(MAT4_IDENTITY, glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f)));

		// Loading in a color lookup table
		Texture3D::Sptr lut = ResourceManager::CreateAsset<Texture3D>("luts/cool.CUBE");   

		// Configure the color correction LUT
		scene->SetColorLUT(lut);

		// Create our materials
		// This will be our box material, with no environment reflections
		Material::Sptr boxMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			boxMaterial->Name = "Box";
			boxMaterial->Set("u_Material.AlbedoMap", boxTexture);
			boxMaterial->Set("u_Material.Shininess", 0.1f);
			boxMaterial->Set("u_Material.NormalMap", normalMapDefault);
		}

		// This will be the reflective material, we'll make the whole thing 90% reflective
		Material::Sptr monkeyMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			monkeyMaterial->Name = "Monkey";
			monkeyMaterial->Set("u_Material.AlbedoMap", monkeyTex);
			monkeyMaterial->Set("u_Material.NormalMap", normalMapDefault);
			monkeyMaterial->Set("u_Material.Shininess", 0.5f);
		}

		// This will be the reflective material, we'll make the whole thing 50% reflective
		Material::Sptr testMaterial = ResourceManager::CreateAsset<Material>(deferredForward); 
		{
			testMaterial->Name = "Box-Specular";
			testMaterial->Set("u_Material.AlbedoMap", boxTexture); 
			testMaterial->Set("u_Material.Specular", boxSpec);
			testMaterial->Set("u_Material.NormalMap", normalMapDefault);
		}

		// Our foliage vertex shader material 
		Material::Sptr foliageMaterial = ResourceManager::CreateAsset<Material>(foliageShader);
		{
			foliageMaterial->Name = "Foliage Shader";
			foliageMaterial->Set("u_Material.AlbedoMap", leafTex);
			foliageMaterial->Set("u_Material.Shininess", 0.1f);
			foliageMaterial->Set("u_Material.DiscardThreshold", 0.1f);
			foliageMaterial->Set("u_Material.NormalMap", normalMapDefault);

			foliageMaterial->Set("u_WindDirection", glm::vec3(1.0f, 1.0f, 0.0f));
			foliageMaterial->Set("u_WindStrength", 0.5f);
			foliageMaterial->Set("u_VerticalScale", 1.0f);
			foliageMaterial->Set("u_WindSpeed", 1.0f);
		}

		// Our toon shader material
		Material::Sptr toonMaterial = ResourceManager::CreateAsset<Material>(celShader);
		{
			toonMaterial->Name = "Toon"; 
			toonMaterial->Set("u_Material.AlbedoMap", boxTexture);
			toonMaterial->Set("u_Material.NormalMap", normalMapDefault);
			toonMaterial->Set("s_ToonTerm", toonLut);
			toonMaterial->Set("u_Material.Shininess", 0.1f); 
			toonMaterial->Set("u_Material.Steps", 8);
		}


		Material::Sptr displacementTest = ResourceManager::CreateAsset<Material>(displacementShader);
		{
			Texture2D::Sptr displacementMap = ResourceManager::CreateAsset<Texture2D>("textures/displacement_map.png");
			Texture2D::Sptr normalMap       = ResourceManager::CreateAsset<Texture2D>("textures/normal_map.png");
			Texture2D::Sptr diffuseMap      = ResourceManager::CreateAsset<Texture2D>("textures/bricks_diffuse.png");

			displacementTest->Name = "Displacement Map";
			displacementTest->Set("u_Material.AlbedoMap", diffuseMap);
			displacementTest->Set("u_Material.NormalMap", normalMap);
			displacementTest->Set("s_Heightmap", displacementMap);
			displacementTest->Set("u_Material.Shininess", 0.5f);
			displacementTest->Set("u_Scale", 0.1f);
		}

		Material::Sptr normalmapMat = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			Texture2D::Sptr normalMap       = ResourceManager::CreateAsset<Texture2D>("textures/normal_map.png");
			Texture2D::Sptr diffuseMap      = ResourceManager::CreateAsset<Texture2D>("textures/bricks_diffuse.png");

			normalmapMat->Name = "Tangent Space Normal Map";
			normalmapMat->Set("u_Material.AlbedoMap", diffuseMap);
			normalmapMat->Set("u_Material.NormalMap", normalMap);
			normalmapMat->Set("u_Material.Shininess", 0.5f);
			normalmapMat->Set("u_Scale", 0.1f);
		}

		Material::Sptr multiTextureMat = ResourceManager::CreateAsset<Material>(multiTextureShader);
		{
			Texture2D::Sptr sand  = ResourceManager::CreateAsset<Texture2D>("textures/terrain/sand.png");
			Texture2D::Sptr grass = ResourceManager::CreateAsset<Texture2D>("textures/terrain/grass.png");

			multiTextureMat->Name = "Multitexturing";
			multiTextureMat->Set("u_Material.DiffuseA", sand);
			multiTextureMat->Set("u_Material.DiffuseB", grass);
			multiTextureMat->Set("u_Material.NormalMapA", normalMapDefault);
			multiTextureMat->Set("u_Material.NormalMapB", normalMapDefault);
			multiTextureMat->Set("u_Material.Shininess", 0.5f);
			multiTextureMat->Set("u_Scale", 0.1f); 
		}
		
		Material::Sptr StartPlatformMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			StartPlatformMaterial->Name = "Monkey";
			StartPlatformMaterial->Set("u_Material.AlbedoMap", StartTex);
			StartPlatformMaterial->Set("u_Material.NormalMap", normalMapDefault);
			StartPlatformMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr UIMat = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			UIMat->Name = "UIButton";
			UIMat->Set("u_Material.Diffuse", StartTex);
			UIMat->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr SmallPlatformMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			SmallPlatformMaterial->Name = "SmallPlatform";
			SmallPlatformMaterial->Set("u_Material.AlbedoMap", StartTex);
			SmallPlatformMaterial->Set("u_Material.NormalMap", normalMapDefault);
			SmallPlatformMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr WallJumpMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			WallJumpMaterial->Name = "WallJump";
			WallJumpMaterial->Set("u_Material.AlbedoMap", StartTex);
			WallJumpMaterial->Set("u_Material.NormalMap", normalMapDefault);
			WallJumpMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr BeatGemMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			BeatGemMaterial->Name = "BeatGem";
			BeatGemMaterial->Set("u_Material.AlbedoMap", GemTex);
			BeatGemMaterial->Set("u_Material.NormalMap", normalMapDefault);
			BeatGemMaterial->Set("u_Material.Shininess", 0.1f);
		}
		//	GemAnticipation	 
		Material::Sptr BeatGemAnticipationMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			BeatGemAnticipationMaterial->Name = "BeatGemAnticipationMaterial";
			BeatGemAnticipationMaterial->Set("u_Material.AlbedoMap", GemAnticipation);
			BeatGemAnticipationMaterial->Set("u_Material.NormalMap", normalMapDefault);
			BeatGemAnticipationMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr BeatGemOffMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			BeatGemOffMaterial->Name = "BeatGemOff";
			BeatGemOffMaterial->Set("u_Material.AlbedoMap", GemOff);
			BeatGemOffMaterial->Set("u_Material.NormalMap", normalMapDefault);
			BeatGemOffMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr VinylMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			VinylMaterial->Name = "Vinyl";
			VinylMaterial->Set("u_Material.AlbedoMap", StartTex);
			VinylMaterial->Set("u_Material.NormalMap", normalMapDefault);
			VinylMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr CDMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			CDMaterial->Name = "CD";
			CDMaterial->Set("u_Material.AlbedoMap", StartTex);
			CDMaterial->Set("u_Material.NormalMap", normalMapDefault);
			CDMaterial->Set("u_Material.Shininess", 0.1f);
		}

		// Material::Sptr CharacterMaterial = ResourceManager::CreateAsset<Material>(AnimatedShader);
		Material::Sptr CharacterMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			CharacterMaterial->Name = "Character";
			CharacterMaterial->Set("u_Material.AlbedoMap", StartTex);
			CharacterMaterial->Set("u_Material.NormalMap", normalMapDefault);
			CharacterMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr DiscoBallMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			DiscoBallMaterial->Name = "DiscoBall";
			DiscoBallMaterial->Set("u_Material.AlbedoMap", StartTex);
			DiscoBallMaterial->Set("u_Material.NormalMap", normalMapDefault);
			DiscoBallMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr LoseScreenMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			LoseScreenMaterial->Name = "Lose Screen";
			LoseScreenMaterial->Set("u_Material.AlbedoMap", LoseScreenTex);
			LoseScreenMaterial->Set("u_Material.NormalMap", normalMapDefault);
			LoseScreenMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr Car1Material = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			Car1Material->Name = "Car1";
			Car1Material->Set("u_Material.AlbedoMap", Car1Tex);
			Car1Material->Set("u_Material.NormalMap", normalMapDefault);
			Car1Material->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr SemiTruckMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			SemiTruckMaterial->Name = "Semi1";
			SemiTruckMaterial->Set("u_Material.AlbedoMap", SemiTruckTex);
			SemiTruckMaterial->Set("u_Material.NormalMap", normalMapDefault);
			SemiTruckMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr PickupTruckMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			PickupTruckMaterial->Name = "Pickup1";
			PickupTruckMaterial->Set("u_Material.AlbedoMap", PickupTruckTex);
			PickupTruckMaterial->Set("u_Material.NormalMap", normalMapDefault);
			PickupTruckMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr BuildingMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			BuildingMaterial->Name = "Building";
			BuildingMaterial->Set("u_Material.AlbedoMap", BuildingTex);
			BuildingMaterial->Set("u_Material.NormalMap", normalMapDefault);
			BuildingMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr KBuildingMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			KBuildingMaterial->Name = "KBuilding";
			KBuildingMaterial->Set("u_Material.AlbedoMap", KBuilding1Tex);
			KBuildingMaterial->Set("u_Material.NormalMap", normalMapDefault);
			KBuildingMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr KBuilding2Material = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			KBuilding2Material->Name = "KBuilding2";
			KBuilding2Material->Set("u_Material.AlbedoMap", KBuilding2Tex);
			KBuilding2Material->Set("u_Material.NormalMap", normalMapDefault);
			KBuilding2Material->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr KBuilding3Material = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			KBuilding3Material->Name = "KBuilding3";
			KBuilding3Material->Set("u_Material.AlbedoMap", KBuilding3Tex);
			KBuilding3Material->Set("u_Material.NormalMap", normalMapDefault);
			KBuilding3Material->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr SmallWallJumpMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			SmallWallJumpMaterial->Name = "Small Wall Jump";
			SmallWallJumpMaterial->Set("u_Material.AlbedoMap", SmallWallJumpTex);
			SmallWallJumpMaterial->Set("u_Material.NormalMap", normalMapDefault);
			SmallWallJumpMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr SuperSmallWallJumpMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			SuperSmallWallJumpMaterial->Name = "Super Small Wall Jump";
			SuperSmallWallJumpMaterial->Set("u_Material.AlbedoMap", SuperSmallWallJumpTex);
			SuperSmallWallJumpMaterial->Set("u_Material.NormalMap", normalMapDefault);
			SuperSmallWallJumpMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr PianoMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			PianoMaterial->Name = "Piano";
			PianoMaterial->Set("u_Material.AlbedoMap", FallingPlatTex);
			PianoMaterial->Set("u_Material.NormalMap", normalMapDefault);
			PianoMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr HalfCirclePlatMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			HalfCirclePlatMaterial->Name = "Half Circle Plat";
			HalfCirclePlatMaterial->Set("u_Material.AlbedoMap", HalfCirclePlatTex);
			HalfCirclePlatMaterial->Set("u_Material.NormalMap", normalMapDefault);
			HalfCirclePlatMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr StairsRightMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			StairsRightMaterial->Name = "Stairs Right";
			StairsRightMaterial->Set("u_Material.AlbedoMap", StairsRightTex);
			StairsRightMaterial->Set("u_Material.NormalMap", normalMapDefault);
			StairsRightMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr StairsLeftMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			StairsLeftMaterial->Name = "Stairs Left";
			StairsLeftMaterial->Set("u_Material.AlbedoMap", StairsLeftTex);
			StairsLeftMaterial->Set("u_Material.NormalMap", normalMapDefault);
			StairsLeftMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr SpeakerMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			SpeakerMaterial->Name = "Speaker Material";
			SpeakerMaterial->Set("u_Material.AlbedoMap", SpeakerTex);
			SpeakerMaterial->Set("u_Material.NormalMap", normalMapDefault);
			SpeakerMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr SquarePlatMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			SquarePlatMaterial->Name = "Square Platform";
			SquarePlatMaterial->Set("u_Material.AlbedoMap", SquarePlatTex);
			SquarePlatMaterial->Set("u_Material.NormalMap", normalMapDefault);
			SquarePlatMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr FloatingLightMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			FloatingLightMaterial->Name = "FLoating Light";
			FloatingLightMaterial->Set("u_Material.AlbedoMap", FloatingLightTex);
			FloatingLightMaterial->Set("u_Material.NormalMap", normalMapDefault);
			FloatingLightMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr OvalBuildingMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			OvalBuildingMaterial->Name = "Oval Building";
			OvalBuildingMaterial->Set("u_Material.AlbedoMap", OvalBuildingTex);
			OvalBuildingMaterial->Set("u_Material.NormalMap", normalMapDefault);
			OvalBuildingMaterial->Set("u_Material.Shininess", 0.1f);
		}

		// Create some lights for our scene
		GameObject::Sptr lightParent = scene->CreateGameObject("Lights");

		for (int ix = 0; ix < 50; ix++) {
			GameObject::Sptr light = scene->CreateGameObject("Light");
			light->SetPostion(glm::vec3(glm::diskRand(25.0f), 1.0f));
			lightParent->AddChild(light);

			Light::Sptr lightComponent = light->Add<Light>();
			lightComponent->SetColor(glm::linearRand(glm::vec3(0.0f), glm::vec3(1.0f)));
			lightComponent->SetRadius(glm::linearRand(0.1f, 10.0f));
			lightComponent->SetIntensity(glm::linearRand(1.0f, 2.0f));
		}

		// We'll create a mesh that is a simple plane that we can resize later
		MeshResource::Sptr planeMesh = ResourceManager::CreateAsset<MeshResource>();
		planeMesh->AddParam(MeshBuilderParam::CreatePlane(ZERO, UNIT_Z, UNIT_X, glm::vec2(1.0f)));
		planeMesh->GenerateMesh();

		MeshResource::Sptr sphere = ResourceManager::CreateAsset<MeshResource>();
		sphere->AddParam(MeshBuilderParam::CreateIcoSphere(ZERO, ONE, 5));
		sphere->GenerateMesh();

		GameObject::Sptr camera = scene->MainCamera->GetGameObject()->SelfRef();
		{
			camera->SetPostion({ 2.280, -6, 2.830 });
			camera->SetRotation({ 90, 0, 0 });
			//camera->LookAt(glm::vec3(0.0f));

			camera->Add<SimpleCameraControl>();

			// This is now handled by scene itself!
			//Camera::Sptr cam = camera->Add<Camera>();
			// Make sure that the camera is set as the scene's main camera!
			//scene->MainCamera = cam;
		}
		// Set up all our sample objects
		//GameObject::Sptr plane = scene->CreateGameObject("Plane");
		//{
		//	// Make a big tiled mesh
		//	MeshResource::Sptr tiledMesh = ResourceManager::CreateAsset<MeshResource>();
		//	tiledMesh->AddParam(MeshBuilderParam::CreatePlane(ZERO, UNIT_Z, UNIT_X, glm::vec2(100.0f), glm::vec2(20.0f)));
		//	tiledMesh->GenerateMesh();
		//
		//	// Create and attach a RenderComponent to the object to draw our mesh
		//	RenderComponent::Sptr renderer = plane->Add<RenderComponent>();
		//	renderer->SetMesh(tiledMesh);
		//	renderer->SetMaterial(boxMaterial);
		//
		//	// Attach a plane collider that extends infinitely along the X/Y axis
		//	RigidBody::Sptr physics = plane->Add<RigidBody>(/*static by default*/);
		//	physics->AddCollider(BoxCollider::Create(glm::vec3(50.0f, 50.0f, 1.0f)))->SetPosition({ 0,0,-1 });
		//}
		/*
		
		GameObject::Sptr monkey1 = scene->CreateGameObject("Monkey 1");
		{
			// Set position in the scene
			monkey1->SetPostion(glm::vec3(1.5f, 0.0f, 1.0f));

			// Add some behaviour that relies on the physics body
			monkey1->Add<JumpBehaviour>();

			// Create and attach a renderer for the monkey
			RenderComponent::Sptr renderer = monkey1->Add<RenderComponent>();
			renderer->SetMesh(monkeyMesh);
			renderer->SetMaterial(monkeyMaterial);

			// Example of a trigger that interacts with static and kinematic bodies as well as dynamic bodies
			TriggerVolume::Sptr trigger = monkey1->Add<TriggerVolume>();
			trigger->SetFlags(TriggerTypeFlags::Statics | TriggerTypeFlags::Kinematics);
			trigger->AddCollider(BoxCollider::Create(glm::vec3(1.0f)));

			monkey1->Add<TriggerVolumeEnterBehaviour>();
		}


		GameObject::Sptr ship = scene->CreateGameObject("Fenrir");
		{
			// Set position in the scene
			ship->SetPostion(glm::vec3(1.5f, 0.0f, 4.0f));
			ship->SetScale(glm::vec3(0.1f));

			// Create and attach a renderer for the monkey
			RenderComponent::Sptr renderer = ship->Add<RenderComponent>();
			renderer->SetMesh(shipMesh);
			renderer->SetMaterial(monkeyMaterial);
		}

		GameObject::Sptr demoBase = scene->CreateGameObject("Demo Parent");

		// Box to showcase the specular material
		GameObject::Sptr specBox = scene->CreateGameObject("Specular Object");
		{
			MeshResource::Sptr boxMesh = ResourceManager::CreateAsset<MeshResource>();
			boxMesh->AddParam(MeshBuilderParam::CreateCube(ZERO, ONE));
			boxMesh->GenerateMesh();

			// Set and rotation position in the scene
			specBox->SetPostion(glm::vec3(0, -4.0f, 1.0f));

			// Add a render component
			RenderComponent::Sptr renderer = specBox->Add<RenderComponent>();
			renderer->SetMesh(boxMesh);
			renderer->SetMaterial(testMaterial); 

			demoBase->AddChild(specBox);
		}

		// sphere to showcase the foliage material
		GameObject::Sptr foliageBall = scene->CreateGameObject("Foliage Sphere");
		{
			// Set and rotation position in the scene
			foliageBall->SetPostion(glm::vec3(-4.0f, -4.0f, 1.0f));

			// Add a render component
			RenderComponent::Sptr renderer = foliageBall->Add<RenderComponent>();
			renderer->SetMesh(sphere);
			renderer->SetMaterial(foliageMaterial);

			demoBase->AddChild(foliageBall);
		}

		// Box to showcase the foliage material
		GameObject::Sptr foliageBox = scene->CreateGameObject("Foliage Box");
		{
			MeshResource::Sptr box = ResourceManager::CreateAsset<MeshResource>();
			box->AddParam(MeshBuilderParam::CreateCube(glm::vec3(0, 0, 0.5f), ONE));
			box->GenerateMesh();

			// Set and rotation position in the scene
			foliageBox->SetPostion(glm::vec3(-6.0f, -4.0f, 1.0f));

			// Add a render component
			RenderComponent::Sptr renderer = foliageBox->Add<RenderComponent>();
			renderer->SetMesh(box);
			renderer->SetMaterial(foliageMaterial);

			demoBase->AddChild(foliageBox);
		}

		// Box to showcase the specular material
		GameObject::Sptr toonBall = scene->CreateGameObject("Toon Object");
		{
			// Set and rotation position in the scene
			toonBall->SetPostion(glm::vec3(-2.0f, -4.0f, 1.0f));

			// Add a render component
			RenderComponent::Sptr renderer = toonBall->Add<RenderComponent>();
			renderer->SetMesh(sphere);
			renderer->SetMaterial(toonMaterial);

			demoBase->AddChild(toonBall);
		}

		GameObject::Sptr displacementBall = scene->CreateGameObject("Displacement Object");
		{
			// Set and rotation position in the scene
			displacementBall->SetPostion(glm::vec3(2.0f, -4.0f, 1.0f));

			// Add a render component
			RenderComponent::Sptr renderer = displacementBall->Add<RenderComponent>();
			renderer->SetMesh(sphere);
			renderer->SetMaterial(displacementTest);

			demoBase->AddChild(displacementBall);
		}

		GameObject::Sptr multiTextureBall = scene->CreateGameObject("Multitextured Object");
		{
			// Set and rotation position in the scene 
			multiTextureBall->SetPostion(glm::vec3(4.0f, -4.0f, 1.0f));

			// Add a render component 
			RenderComponent::Sptr renderer = multiTextureBall->Add<RenderComponent>();
			renderer->SetMesh(sphere);
			renderer->SetMaterial(multiTextureMat);

			demoBase->AddChild(multiTextureBall);
		}

		GameObject::Sptr normalMapBall = scene->CreateGameObject("Normal Mapped Object");
		{
			// Set and rotation position in the scene 
			normalMapBall->SetPostion(glm::vec3(6.0f, -4.0f, 1.0f));

			// Add a render component 
			RenderComponent::Sptr renderer = normalMapBall->Add<RenderComponent>();
			renderer->SetMesh(sphere);
			renderer->SetMaterial(normalmapMat);

			demoBase->AddChild(normalMapBall);
		}
		*/
		// Background and forground vehicles\\
		Give these Parents for Foreground/Background Blocks if we have enough objects to do that with!
DefaultSceneLayer::SpawnBackGroundCar(scene, Car1Mesh, Car1Material, "Car1", glm::vec3(14.870f, 7.80f, 2.7f), glm::vec3(90.0f, 0.0f, -90.0f), glm::vec3(0.250f, 0.250f, 0.250f));
DefaultSceneLayer::SpawnBackGroundCar(scene, SemiTruckMesh, SemiTruckMaterial, "Semi1", glm::vec3(28.870f, 7.80f, 2.7f), glm::vec3(90.0f, 0.0f, -90.0f), glm::vec3(0.250f, 0.250f, 0.250f));
DefaultSceneLayer::SpawnForeGroundCar(scene, Car1Mesh, Car1Material, "Car2", glm::vec3(-9.970f, 0.470f, -1.90f), glm::vec3(90.0f, 0.0f, 90.0f), glm::vec3(0.250f, 0.250f, 0.250f));
DefaultSceneLayer::SpawnForeGroundCar(scene, PickupTruckMesh, PickupTruckMaterial, "Pickup1", glm::vec3(-18.970f, 0.470f, -1.90f), glm::vec3(90.0f, 0.0f, 90.0f), glm::vec3(0.250f, 0.250f, 0.250f));
DefaultSceneLayer::SpawnBackGroundBuilding(scene, KBuilding1Mesh, KBuildingMaterial, "KBuilding1", glm::vec3(-1.0f, 21.880f, -46.040f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.780f, 1.470f, 1.0f));
DefaultSceneLayer::SpawnBackGroundBuilding(scene, KBuilding2Mesh, KBuilding2Material, "KBuilding2", glm::vec3(25.670, 21.880f, -46.040f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.780f, 1.470f, 1.0f));
DefaultSceneLayer::SpawnBackGroundBuilding(scene, KBuilding3Mesh, KBuilding3Material, "KBuilding3", glm::vec3(-30.530f, 21.880f, -46.040f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.780f, 1.470f, 1.0f));
DefaultSceneLayer::SpawnBackGroundBuilding(scene, OvalBuilding, OvalBuildingMaterial, "OvalBuilding", glm::vec3(13.730f, 27.510f, -46.040f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.720f, 3.500f, 1.0f));

// Tutorial
DefaultSceneLayer::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "StartPlatform", glm::vec3(-15.820f, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
DefaultSceneLayer::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-11.640f, 5.610f, -4.150f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
DefaultSceneLayer::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-8.620f, 5.610f, -2.200f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
DefaultSceneLayer::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-5.290f, 5.610f, -4.040f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
DefaultSceneLayer::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(0.340f, 5.610f, 5.440f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
DefaultSceneLayer::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(1.150f, 5.610f, -4.850f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
DefaultSceneLayer::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(4.360f, 5.610f, -4.850f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
DefaultSceneLayer::SpawnWallJump(scene, WallJump, WallJumpMaterial, "Wall Jump", glm::vec3(-3.270f, 5.610f, 3.070f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
DefaultSceneLayer::SpawnWallJump(scene, WallJump, WallJumpMaterial, "Wall Jump", glm::vec3(-1.100f, 5.610f, 2.010f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
DefaultSceneLayer::SpawnGem(scene, BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem 4", 4, glm::vec3(-2.280f, 5.610f, -2.450f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
DefaultSceneLayer::SpawnCollectable(scene, Vinyl, VinylMaterial, "Vinyl", glm::vec3(0.180f, 5.610f, 6.010f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
DefaultSceneLayer::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "EndPlatform", glm::vec3(9.180f, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
DefaultSceneLayer::SpawnBackGroundCar(scene, Car1Mesh, Car1Material, "Car1", glm::vec3(14.870f, 9.80f, 2.7f), glm::vec3(90.0f, 0.0f, -90.0f), glm::vec3(0.250f, 0.250f, 0.250f));
DefaultSceneLayer::SpawnBackGroundCar(scene, SemiTruckMesh, SemiTruckMaterial, "Semi1", glm::vec3(28.870f, 9.80f, 2.7f), glm::vec3(90.0f, 0.0f, -90.0f), glm::vec3(0.250f, 0.250f, 0.250f));
DefaultSceneLayer::SpawnForeGroundCar(scene, Car1Mesh, Car1Material, "Car2", glm::vec3(-9.970f, 0.470f, -1.90f), glm::vec3(90.0f, 0.0f, 90.0f), glm::vec3(0.250f, 0.250f, 0.250f));
DefaultSceneLayer::SpawnForeGroundCar(scene, PickupTruckMesh, PickupTruckMaterial, "Pickup1", glm::vec3(-18.970f, 0.470f, -1.90f), glm::vec3(90.0f, 0.0f, 90.0f), glm::vec3(0.250f, 0.250f, 0.250f));
float adjustment = 30;

// 1st Block		
DefaultSceneLayer::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "StartPlatform", glm::vec3(-15.820f + adjustment, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
DefaultSceneLayer::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-10.460f + adjustment, 5.610f, -4.150f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
DefaultSceneLayer::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-3.940f + adjustment, 5.610f, -4.150f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
DefaultSceneLayer::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(2.200f + adjustment, 5.610f, -1.770f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
DefaultSceneLayer::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(6.500f + adjustment, 5.610f, -4.150f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
DefaultSceneLayer::SpawnGem(scene, BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem 4", 4, glm::vec3(-0.520f + adjustment, 5.610f, -3.160f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
DefaultSceneLayer::SpawnGem(scene, BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem 4", 4, glm::vec3(-6.940f + adjustment, 5.610f, -3.160f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
DefaultSceneLayer::SpawnCollectable(scene, Vinyl, VinylMaterial, "Vinyl", glm::vec3(5.210f + adjustment, 5.610f, 0.080f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
DefaultSceneLayer::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "EndPlatform", glm::vec3(9.180f + adjustment, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));

// CDs for Block 1
DefaultSceneLayer::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-10.220f + adjustment, 5.610f, -3.220f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
DefaultSceneLayer::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-6.970f + adjustment, 5.610f, -2.810f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
DefaultSceneLayer::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-3.770f + adjustment, 5.610f, -3.190f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
DefaultSceneLayer::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-0.620f + adjustment, 5.610f, -2.380f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
DefaultSceneLayer::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(2.140f + adjustment, 5.610f, -0.770f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));



		// Create a trigger volume for testing how we can detect collisions with objects!
		GameObject::Sptr trigger = scene->CreateGameObject("Trigger");
		{
			TriggerVolume::Sptr volume = trigger->Add<TriggerVolume>();
			CylinderCollider::Sptr collider = CylinderCollider::Create(glm::vec3(3.0f, 3.0f, 1.0f));
			collider->SetPosition(glm::vec3(0.0f, 0.0f, 0.5f));
			volume->AddCollider(collider);

			trigger->Add<TriggerVolumeEnterBehaviour>();
		}


		GameObject::Sptr particles = scene->CreateGameObject("Particles");
		{
			ParticleSystem::Sptr particleManager = particles->Add<ParticleSystem>();  
			particleManager->AddEmitter(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 10.0f), 10.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)); 
		}

		GuiBatcher::SetDefaultTexture(ResourceManager::CreateAsset<Texture2D>("textures/ui-sprite.png"));
		GuiBatcher::SetDefaultBorderRadius(8);

		// Save the asset manifest for all the resources we just loaded
		ResourceManager::SaveManifest("scene-manifest.json");
		// Save the scene to a JSON file
		scene->Save("scene.json");

		// Send the scene to the application
		app.LoadScene(scene);
	}
}
