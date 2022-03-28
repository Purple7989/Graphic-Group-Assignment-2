#pragma once
#include "SpawnLoop.h"
#include "Gameplay/Components/BeatTimer.h"

#include "Gameplay/Components/LevelMover.h"
#include "Gameplay/Components/RenderComponent.h"
#include "Gameplay/Physics/TriggerVolume.h"
#include "Gameplay/Components/RotatingBehaviour.h"

#include <GLFW/glfw3.h>
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Utils/ImGuiHelper.h"
#include "Application/Layers/SpawnFunctions.h"
#include "Application/Application.h"
#include "Gameplay/MeshResource.h"
#include "Gameplay/Material.h"
#include "Gameplay/GameObject.h"
#include "Utils/ResourceManager/ResourceManager.h"
#include "Utils/FileHelpers.h"
#include "Gameplay/GameObject.h"

//GLM stuffs
#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/matrix_transform_2d.hpp>
#include "Utils/GlmDefines.h"
#include "Utils/ImGuiHelper.h"
#include "Utils/JsonGlmHelpers.h"
#include "Graphics/GuiBatcher.h"


SpawnLoop::SpawnLoop() {
	_BlockToSpawn = 1;

}
void SpawnLoop::Awake() {

	nlohmann::ordered_json data = ResourceManager::GetManifest();

	CreateList(data);
	using namespace Gameplay;
	
	Scene::Sptr scene = Application::Get().CurrentScene();
	
	//Loads The Resource From the Manifest using the human-readable names
	
	//Meshes
	monkeyMesh = ResourceManager::Get<MeshResource>(MeshList.find("Monkey.obj")->second);
	SmallPlatform = ResourceManager::Get<MeshResource>(MeshList.find("HaloBasicPlatform.obj")->second);
	WallJump = ResourceManager::Get<MeshResource>(MeshList.find("WallJumpV6.obj")->second);
	BeatGem = ResourceManager::Get<MeshResource>(MeshList.find("Gem.obj")->second);
	Vinyl = ResourceManager::Get<MeshResource>(MeshList.find("VinylV2.obj")->second);
	CD = ResourceManager::Get<MeshResource>(MeshList.find("CDwithUnwrap.obj")->second);
	Building = ResourceManager::Get<MeshResource>(MeshList.find("RBuilding.obj")->second);
	KBuilding1Mesh = ResourceManager::Get<MeshResource>(MeshList.find("KBuilding.obj")->second);
	KBuilding2Mesh = ResourceManager::Get<MeshResource>(MeshList.find("KBuilding2.obj")->second);
	KBuilding3Mesh = ResourceManager::Get<MeshResource>(MeshList.find("KBuilding3.obj")->second);
	OvalBuilding = ResourceManager::Get<MeshResource>(MeshList.find("OvalBuilding.obj")->second);
	CharacterMesh = ResourceManager::Get<MeshResource>(MeshList.find("discobot.obj")->second);
	DiscoBallMesh = ResourceManager::Get<MeshResource>(MeshList.find("DiscoBall2.obj")->second);
	StartPlatform = ResourceManager::Get<MeshResource>(MeshList.find("startbuilding.obj")->second);
	Car1Mesh = ResourceManager::Get<MeshResource>(MeshList.find("FutureCar1.obj")->second);
	SemiTruckMesh = ResourceManager::Get<MeshResource>(MeshList.find("Semitruck.obj")->second);
	PickupTruckMesh = ResourceManager::Get<MeshResource>(MeshList.find("FuturePickup.obj")->second);
	SmallWallJump = ResourceManager::Get<MeshResource>(MeshList.find("SmallWallJump.obj")->second);
	SuperSmallWallJump = ResourceManager::Get<MeshResource>(MeshList.find("SuperSmallWallJump.obj")->second);
	FallingPlat = ResourceManager::Get<MeshResource>(MeshList.find("pianoplatform.obj")->second);
	HalfCirclePlat = ResourceManager::Get<MeshResource>(MeshList.find("HalfCriclePlat.obj")->second);
	StairsRight = ResourceManager::Get<MeshResource>(MeshList.find("SlantedStairCase.obj")->second);
	StairsLeft = ResourceManager::Get<MeshResource>(MeshList.find("StairCaseL.obj")->second);
	Speaker = ResourceManager::Get<MeshResource>(MeshList.find("speaker.obj")->second);
	SquarePlat = ResourceManager::Get<MeshResource>(MeshList.find("SquarePlatform.obj")->second);
	FloatingLight = ResourceManager::Get<MeshResource>(MeshList.find("FloatingStreetLight.obj")->second);
	DiscoBotMesh1 = ResourceManager::Get<MeshResource>(MeshList.find("CharacterAnims/run1.obj")->second);
	DiscoBotMesh2 = ResourceManager::Get<MeshResource>(MeshList.find("CharacterAnims/run2.obj")->second);
	DiscoBotMesh3 = ResourceManager::Get<MeshResource>(MeshList.find("CharacterAnims/run3.obj")->second);
	DiscoBotMesh4 = ResourceManager::Get<MeshResource>(MeshList.find("CharacterAnims/run4.obj")->second);
	DiscoBotMesh5 = ResourceManager::Get<MeshResource>(MeshList.find("CharacterAnims/run5.obj")->second);
	DiscoBotMesh6 = ResourceManager::Get<MeshResource>(MeshList.find("CharacterAnims/run6.obj")->second);
	DiscoBotMesh7 = ResourceManager::Get<MeshResource>(MeshList.find("CharacterAnims/run7.obj")->second);
	DiscoBotMesh8 = ResourceManager::Get<MeshResource>(MeshList.find("CharacterAnims/run8.obj")->second);
	DiscoBotMesh9 = ResourceManager::Get<MeshResource>(MeshList.find("CharacterAnims/run9.obj")->second);
	BotJump1 = ResourceManager::Get<MeshResource>(MeshList.find("CharacterAnims/jump1.obj")->second);
	BotJump2 = ResourceManager::Get<MeshResource>(MeshList.find("CharacterAnims/jump2.obj")->second);
	BotJump3 = ResourceManager::Get<MeshResource>(MeshList.find("CharacterAnims/jump3.obj")->second);
	BotJump4 = ResourceManager::Get<MeshResource>(MeshList.find("CharacterAnims/jump4.obj")->second);
	BotJump5 = ResourceManager::Get<MeshResource>(MeshList.find("CharacterAnims/jump5.obj")->second);
	BotJump6 = ResourceManager::Get<MeshResource>(MeshList.find("CharacterAnims/jump6.obj")->second);
	BotJump7 = ResourceManager::Get<MeshResource>(MeshList.find("CharacterAnims/jump7.obj")->second);

	//Materials
	StartPlatformMaterial = ResourceManager::Get<Material>(MaterialList.find("StartPlatform")->second);
	UIMat = ResourceManager::Get<Material>(MaterialList.find("UIButton")->second);
	SmallPlatformMaterial = ResourceManager::Get<Material>(MaterialList.find("SmallPlatform")->second);
	WallJumpMaterial = ResourceManager::Get<Material>(MaterialList.find("WallJump")->second);
	BeatGemMaterial = ResourceManager::Get<Material>(MaterialList.find("BeatGem")->second);
	BeatGemOffMaterial = ResourceManager::Get<Material>(MaterialList.find("BeatGemOff")->second);
	VinylMaterial = ResourceManager::Get<Material>(MaterialList.find("Vinyl")->second);
	CDMaterial = ResourceManager::Get<Material>(MaterialList.find("CD")->second);
	CharacterMaterial = ResourceManager::Get<Material>(MaterialList.find("Character")->second);
	DiscoBallMaterial = ResourceManager::Get<Material>(MaterialList.find("DiscoBall")->second);
	LoseScreenMaterial = ResourceManager::Get<Material>(MaterialList.find("Lose Screen")->second);
	Car1Material = ResourceManager::Get<Material>(MaterialList.find("Car1")->second);
	SemiTruckMaterial = ResourceManager::Get<Material>(MaterialList.find("Semi1")->second);
	PickupTruckMaterial = ResourceManager::Get<Material>(MaterialList.find("Pickup1")->second);
	BuildingMaterial = ResourceManager::Get<Material>(MaterialList.find("Building")->second);
	KBuildingMaterial = ResourceManager::Get<Material>(MaterialList.find("KBuilding")->second);
	KBuilding2Material = ResourceManager::Get<Material>(MaterialList.find("KBuilding2")->second);
	KBuilding3Material = ResourceManager::Get<Material>(MaterialList.find("KBuilding3")->second);
	SmallWallJumpMaterial = ResourceManager::Get<Material>(MaterialList.find("Small Wall Jump")->second);
	SuperSmallWallJumpMaterial = ResourceManager::Get<Material>(MaterialList.find("Super Small Wall Jump")->second);
	PianoMaterial = ResourceManager::Get<Material>(MaterialList.find("Piano")->second);
	HalfCirclePlatMaterial = ResourceManager::Get<Material>(MaterialList.find("Half Circle Plat")->second);
	StairsRightMaterial = ResourceManager::Get<Material>(MaterialList.find("Stairs Right")->second);
	StairsLeftMaterial = ResourceManager::Get<Material>(MaterialList.find("Stairs Left")->second);
	SpeakerMaterial = ResourceManager::Get<Material>(MaterialList.find("Speaker Material")->second);
	SquarePlatMaterial = ResourceManager::Get<Material>(MaterialList.find("Square Platform")->second);
	FloatingLightMaterial = ResourceManager::Get<Material>(MaterialList.find("FLoating Light")->second);
	OvalBuildingMaterial = ResourceManager::Get<Material>(MaterialList.find("Oval Building")->second);

}


void SpawnLoop::CreateList(const nlohmann::json& blob) {
	using namespace Gameplay;
	//It's probably more efficient to grab these values Directly form the manifest since
	//they're already loaded there but iterating through JSON's is Icky....

	//Performs some Operation(s) for Every Resource of each type
	ResourceManager::Each<Material>([&](const Material::Sptr& Mat) {

		if (Mat == nullptr) {
			return;
		}
		//Map the GUID created on App start to a human-readable name
		MaterialList.insert(std::pair<std::string, Guid>(Mat->Name, Mat->GetGUID()));

		});

	ResourceManager::Each<MeshResource>([&](const MeshResource::Sptr& Mesh) {
		if (Mesh == nullptr) {
			return;
		}
		//Map the GUID created on App start to a human-readable name
		MeshList.insert(std::pair<std::string, Guid>(Mesh->Filename, Mesh->GetGUID()));

		});
}

void SpawnLoop::SpawnBlock() {
	//This is Disgusting.

	//using namespace Gameplay;
	Application& app = Application::Get();
	Gameplay::Scene::Sptr scene = app.CurrentScene();

	float distanceFromBlock = 23.5;

	switch (_BlockToSpawn) {
	case 0:
		// 1st Block		
		SpawnFunctions::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "StartPlatform", glm::vec3(-15.820f + distanceFromBlock, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-10.460f + distanceFromBlock, 5.610f, -4.150f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-3.940f + distanceFromBlock, 5.610f, -4.150f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(2.200f + distanceFromBlock, 5.610f, -1.770f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(6.500f + distanceFromBlock, 5.610f, -4.150f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnGem(scene, BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem", 4, glm::vec3(-0.520f + distanceFromBlock, 5.610f, -3.160f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
		SpawnFunctions::SpawnGem(scene, BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem", 4, glm::vec3(-6.940f + distanceFromBlock, 5.610f, -3.160f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
		SpawnFunctions::SpawnCollectable(scene, Vinyl, VinylMaterial, "Vinyl", glm::vec3(5.210f + distanceFromBlock, 5.610f, 0.080f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "EndPlatform", glm::vec3(9.180f + distanceFromBlock, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));

		// CDs for Block 1
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-10.220f + distanceFromBlock, 5.610f, -3.220f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-6.970f + distanceFromBlock, 5.610f, -2.810f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-3.770f + distanceFromBlock, 5.610f, -3.190f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-0.620f + distanceFromBlock, 5.610f, -2.380f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(2.140f + distanceFromBlock, 5.610f, -0.770f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		
		break;
	case 1:
		//spawn block 2
		SpawnFunctions::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "StartPlatform", glm::vec3(-15.820f + distanceFromBlock, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnWallJump(scene, WallJump, WallJumpMaterial, "Wall Jump", glm::vec3(-9.320f + distanceFromBlock, 5.610f, 3.210f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.040f, 1.500f));
		SpawnFunctions::SpawnWallJump(scene, WallJump, WallJumpMaterial, "Wall Jump", glm::vec3(-7.320f + distanceFromBlock, 5.610f, 2.000f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.040f, 1.500f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-12.130f + distanceFromBlock, 5.610f, -3.850f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-4.880f + distanceFromBlock, 5.610f, 4.000f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(1.940f + distanceFromBlock, 5.610f, -4.150f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(6.190f + distanceFromBlock, 5.610f, -4.160f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnGem(scene, BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem", 4, glm::vec3(-9.190f + distanceFromBlock, 5.610f, -2.060f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
		SpawnFunctions::SpawnGem(scene, BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem", 4, glm::vec3(-1.340f + distanceFromBlock, 5.610f, 0.500f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
		SpawnFunctions::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "EndPlatform", glm::vec3(9.180f + distanceFromBlock, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));

		// CDs for Block 2
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-8.380f + distanceFromBlock, 5.610f, -0.030f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-8.380f + distanceFromBlock, 5.610f, 2.130f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-8.380f + distanceFromBlock, 5.610f, 4.610f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-4.910f + distanceFromBlock, 5.610f, 4.750f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-1.340f + distanceFromBlock, 5.610f, 0.810f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(1.920f + distanceFromBlock, 5.610f, -3.610f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		break;
	case 2:
		//spawn block 3
		//spawn block 3
		SpawnFunctions::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "StartPlatform", glm::vec3(-15.820f + distanceFromBlock, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-9.000f + distanceFromBlock, 5.610f, -0.290f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-4.360f + distanceFromBlock, 5.610f, -0.290f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(0.350f + distanceFromBlock, 5.610f, -0.290f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, FallingPlat, PianoMaterial, "Falling Platform", glm::vec3(0.390f + distanceFromBlock, 5.610f, -4.150f), glm::vec3(180.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(3.220f + distanceFromBlock, 5.610f, -4.150f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(6.490f + distanceFromBlock, 5.610f, -4.150f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnGem(scene, BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem", 4, glm::vec3(-11.530f + distanceFromBlock, 5.610f, -1.970f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
		SpawnFunctions::SpawnGem(scene, BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem", 4, glm::vec3(-6.870f + distanceFromBlock, 5.610f, -1.970f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
		SpawnFunctions::SpawnGem(scene, BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem", 4, glm::vec3(-1.870f + distanceFromBlock, 5.610f, -1.970f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
		SpawnFunctions::SpawnCollectable(scene, Vinyl, VinylMaterial, "Vinyl", glm::vec3(0.370f + distanceFromBlock, 5.610f, -2.830f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "EndPlatform", glm::vec3(9.180f + distanceFromBlock, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));

		// CDs for Block 3
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-11.530f + distanceFromBlock, 5.610f, -1.360f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-9.000f + distanceFromBlock, 5.610f, 0.440f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-4.390f + distanceFromBlock, 5.610f, 0.440f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(0.350f + distanceFromBlock, 5.610f, 0.290f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(3.260f + distanceFromBlock, 5.610f, -3.210f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-6.690f + distanceFromBlock, 5.610f, -1.180f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		break;
	case 3:
		//spawn block 4
		SpawnFunctions::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "StartPlatform", glm::vec3(-15.820f + distanceFromBlock, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-12.030f + distanceFromBlock, 5.610f, -4.220f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-8.540f + distanceFromBlock, 5.610f, -4.220f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-4.230f + distanceFromBlock, 5.610f, -4.220f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(2.290f + distanceFromBlock, 5.610f, 4.700f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnSpeaker(scene, Speaker, SpeakerMaterial, "Speaker", glm::vec3(3.400f + distanceFromBlock, 6.410f, -0.090f), glm::vec3(90.0f, 0.0f, -32.000f), glm::vec3(0.500f, 0.500f, 0.500f));
		SpawnFunctions::SpawnObj(scene, HalfCirclePlat, HalfCirclePlatMaterial, "Half Circle Platform", glm::vec3(-0.720f + distanceFromBlock, 5.610f, -4.220f), glm::vec3(-90.000f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
		SpawnFunctions::SpawnBuilding(scene, Building, BuildingMaterial, "Building", glm::vec3(4.580f + distanceFromBlock, 5.610f, -7.110f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnWallJump(scene, WallJump, WallJumpMaterial, "Wall Jump", glm::vec3(-1.590f + distanceFromBlock, 5.610f, 2.650f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		SpawnFunctions::SpawnWallJump(scene, WallJump, WallJumpMaterial, "Wall Jump", glm::vec3(0.460f + distanceFromBlock, 5.610f, 1.610), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		SpawnFunctions::SpawnGem(scene, BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem", 4, glm::vec3(1.770f + distanceFromBlock, 5.610f, -3.520f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
		//SpawnFunctions::SpawnCollectable(scene, Vinyl, VinylMaterial, "Vinyl", glm::vec3(2.190f + distanceFromBlock, 5.610f, 5.390f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "EndPlatform", glm::vec3(9.180f + distanceFromBlock, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));

		// CDs for Block 4
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-11.730f + distanceFromBlock, 5.610f, -3.310f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-8.540f + distanceFromBlock, 5.610f, -3.310f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-4.230f + distanceFromBlock, 5.610f, -3.160f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-0.630f + distanceFromBlock, 5.610f, -0.520f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-0.640f + distanceFromBlock, 5.610f, 2.130f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-0.640f + distanceFromBlock, 5.610f, 4.770f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(2.190f + distanceFromBlock, 5.610f, 5.390f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(1.770f + distanceFromBlock, 5.610f, -1.180f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		break;
	case 4:
		//spawn block 5
		SpawnFunctions::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "StartPlatform", glm::vec3(-15.820f + distanceFromBlock, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-12.820f + distanceFromBlock, 5.610f, -4.220f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-10.240f + distanceFromBlock, 5.610f, -2.830f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-7.960f + distanceFromBlock, 5.610f, -1.410f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-3.330f + distanceFromBlock, 5.610f, 5.950f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(2.280f + distanceFromBlock, 5.610f, 4.180f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(2.280f + distanceFromBlock, 5.610f, -4.010f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnSquarePlat(scene, SquarePlat, SquarePlatMaterial, "Square Platform", glm::vec3(-6.180f + distanceFromBlock, 5.610f, -0.170f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnSquarePlat(scene, SquarePlat, SquarePlatMaterial, "Square Platform", glm::vec3(4.630f + distanceFromBlock, 5.610f, 2.260f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnSquarePlat(scene, SquarePlat, SquarePlatMaterial, "Square Platform", glm::vec3(5.010f + distanceFromBlock, 5.610f, -3.850f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnSmallWallJump(scene, SmallWallJump, SmallWallJumpMaterial, "Small Wall Jump", glm::vec3(-6.730f + distanceFromBlock, 5.610f, 4.460f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		SpawnFunctions::SpawnSmallWallJump(scene, SmallWallJump, SmallWallJumpMaterial, "Small Wall Jump", glm::vec3(-5.030f + distanceFromBlock, 5.610f, 4.110f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		SpawnFunctions::SpawnSmallWallJump(scene, SmallWallJump, SmallWallJumpMaterial, "Small Wall Jump", glm::vec3(-1.590f + distanceFromBlock, 5.610f, 2.650f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		SpawnFunctions::SpawnSmallWallJump(scene, SmallWallJump, SmallWallJumpMaterial, "Small Wall Jump", glm::vec3(0.460f + distanceFromBlock, 5.610f, 1.610f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		SpawnFunctions::SpawnGem(scene, BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem", 4, glm::vec3(-0.580f + distanceFromBlock, 5.610f, -1.970f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
		SpawnFunctions::SpawnCollectable(scene, Vinyl, VinylMaterial, "Vinyl", glm::vec3(2.190f + distanceFromBlock, 5.610f, 5.390f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "EndPlatform", glm::vec3(9.180f + distanceFromBlock, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));

		// CDs for Block 5
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-6.040f + distanceFromBlock, 5.610f, 2.340f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-6.040f + distanceFromBlock, 5.610f, 5.170f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-0.630f + distanceFromBlock, 5.610f, 0.460f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-0.640f + distanceFromBlock, 5.610f, 2.710f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(2.170f + distanceFromBlock, 5.610f, -2.880f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		break;
	case 5:
		//block 6
		SpawnFunctions::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "StartPlatform", glm::vec3(-15.820f + distanceFromBlock, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-12.110f + distanceFromBlock, 5.610f, -4.220f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-8.870f + distanceFromBlock, 5.610f, -2.830f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-5.920f + distanceFromBlock, 5.610f, -1.410f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-3.530f + distanceFromBlock, 5.610f, 0.030f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnGem(scene, BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem", 4, glm::vec3(5.770f + distanceFromBlock, 5.610f, -5.060f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
		SpawnFunctions::SpawnSpeaker(scene, Speaker, SpeakerMaterial, "Speaker", glm::vec3(0.840f + distanceFromBlock, 6.410f, 2.360f), glm::vec3(90.0f, 0.0f, -32.000f), glm::vec3(0.500f, 0.500f, 0.500f));
		SpawnFunctions::SpawnSmallWallJump(scene, SmallWallJump, SmallWallJumpMaterial, "Small Wall Jump", glm::vec3(-2.600f + distanceFromBlock, 5.610f, 5.940f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		SpawnFunctions::SpawnSmallWallJump(scene, SmallWallJump, SmallWallJumpMaterial, "Small Wall Jump", glm::vec3(-1.170f + distanceFromBlock, 5.610f, 6.950f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		SpawnFunctions::SpawnBuilding(scene, Building, BuildingMaterial, "Building Block6 1", glm::vec3(-1.010f + distanceFromBlock, 5.610f, -4.960f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.310f, 0.310f, 0.310f));
		SpawnFunctions::SpawnBuilding(scene, Building, BuildingMaterial, "Building Block6 2", glm::vec3(2.070f + distanceFromBlock, 5.610f, -3.810f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.310f, 0.310f, 0.310f));
		SpawnFunctions::SpawnCollectable(scene, Vinyl, VinylMaterial, "Vinyl", glm::vec3(-1.890f + distanceFromBlock, 5.610f, 5.390f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "EndPlatform", glm::vec3(9.180f + distanceFromBlock, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));

		// CDs for Block 6
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-11.690f + distanceFromBlock, 5.610f, -3.190f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-8.860f + distanceFromBlock, 5.610f, -1.820f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-5.720f + distanceFromBlock, 5.610f, -0.610f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-3.470f + distanceFromBlock, 5.610f, 0.790f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(5.660f + distanceFromBlock, 5.610f, -4.080f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		
		break;
	case 6:
		//block 7
		SpawnFunctions::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "StartPlatform", glm::vec3(-15.820f + distanceFromBlock, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnStairsRight(scene, StairsRight, StairsRightMaterial, "Stairs Right", glm::vec3(-11.200f + distanceFromBlock, 5.610f, -4.200f), glm::vec3(90.0f, 0.0f, 90.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-5.130f + distanceFromBlock, 5.610f, 2.210f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-0.810f + distanceFromBlock, 5.610f, 2.270f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-0.100f + distanceFromBlock, 5.610f, -1.110f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnSpeaker(scene, Speaker, SpeakerMaterial, "Speaker", glm::vec3(3.630f + distanceFromBlock, 6.410f, 2.560f), glm::vec3(90.0f, 0.0f, -32.000f), glm::vec3(0.500f, 0.500f, 0.500f));
		SpawnFunctions::SpawnStairsLeft(scene, StairsLeft, StairsLeftMaterial, "Stairs Left", glm::vec3(-1.250f + distanceFromBlock, 5.610f, -0.920f), glm::vec3(90.0f, 0.0f, 90.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnSmallWallJump(scene, SmallWallJump, SmallWallJumpMaterial, "Small Wall Jump", glm::vec3(-8.890f + distanceFromBlock, 5.610f, 2.050f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		SpawnFunctions::SpawnSmallWallJump(scene, SmallWallJump, SmallWallJumpMaterial, "Small Wall Jump", glm::vec3(-6.950f + distanceFromBlock, 5.610f, 0.380f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		SpawnFunctions::SpawnBuilding(scene, Building, BuildingMaterial, "Building", glm::vec3(4.730f + distanceFromBlock, 5.610f, -3.610f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.310f, 0.310f, 0.310f));
		SpawnFunctions::SpawnGem(scene, BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem", 4, glm::vec3(1.350f + distanceFromBlock, 5.610f, 1.180f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
		SpawnFunctions::SpawnCollectable(scene, Vinyl, VinylMaterial, "Vinyl", glm::vec3(-0.740f + distanceFromBlock, 5.610f, 2.750f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "EndPlatform", glm::vec3(9.180f + distanceFromBlock, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));

		// CDs for Block 7
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-7.840f + distanceFromBlock, 5.610f, -0.950f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-7.850f + distanceFromBlock, 5.610f, 2.340f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-5.030f + distanceFromBlock, 5.610f, 2.760f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-0.120f + distanceFromBlock, 5.610f, -0.410f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		break;
	case 7:
		// 8th Block
		SpawnFunctions::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "StartPlatform", glm::vec3(-15.820f + distanceFromBlock, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-12.960f + distanceFromBlock, 5.610f, -4.910f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-9.270f + distanceFromBlock, 5.610f, -4.140f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-4.890f + distanceFromBlock, 5.610f, -3.310f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnStairsRight(scene, StairsRight, StairsRightMaterial, "Stairs Right", glm::vec3(-0.180f + distanceFromBlock, 5.610f, -5.570f), glm::vec3(90.0f, 0.0f, -90.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnSpeaker(scene, Speaker, SpeakerMaterial, "Speaker", glm::vec3(3.410f + distanceFromBlock, 6.410f, -0.350f), glm::vec3(90.0f, 0.0f, -32.000f), glm::vec3(0.500f, 0.500f, 0.500f));
		SpawnFunctions::SpawnBuilding2(scene, Building, BuildingMaterial, "Building", glm::vec3(-7.960f + distanceFromBlock, 5.610f, 6.710f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnBuilding(scene, Building, BuildingMaterial, "Building", glm::vec3(4.570f + distanceFromBlock, 5.610f, -6.630f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.310f, 0.310f, 0.310f));
		SpawnFunctions::SpawnBuilding2(scene, Building, BuildingMaterial, "Building", glm::vec3(-4.500f + distanceFromBlock, 5.610f, 7.810f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnBuilding3(scene, Building, BuildingMaterial, "Building", glm::vec3(-1.350f + distanceFromBlock, 5.610f, 5.180f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.290f, 0.290f, 0.290f));
		SpawnFunctions::SpawnGem(scene, BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem", 4, glm::vec3(-2.630f + distanceFromBlock, 5.610f, -4.550f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
		SpawnFunctions::SpawnSuperSmallWallJump(scene, SuperSmallWallJump, SuperSmallWallJumpMaterial, "Super Small Wall Jump", glm::vec3(2.400f + distanceFromBlock, 5.610f, -3.180f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		SpawnFunctions::SpawnSuperSmallWallJump(scene, SuperSmallWallJump, SuperSmallWallJumpMaterial, "Super Small Wall Jump", glm::vec3(0.700f + distanceFromBlock, 5.610f, -1.510f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		SpawnFunctions::SpawnSuperSmallWallJump(scene, SuperSmallWallJump, SuperSmallWallJumpMaterial, "Super Small Wall Jump", glm::vec3(2.400f + distanceFromBlock, 5.610f, -0.590f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		SpawnFunctions::SpawnSuperSmallWallJump(scene, SuperSmallWallJump, SuperSmallWallJumpMaterial, "Super Small Wall Jump", glm::vec3(0.700f + distanceFromBlock, 5.610f, 0.700f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		SpawnFunctions::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "EndPlatform", glm::vec3(9.180f + distanceFromBlock, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));

		// CDs for Block 8
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-12.620f + distanceFromBlock, 5.610f, -4.080f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-9.160f + distanceFromBlock, 5.610f, -3.410f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-4.850f + distanceFromBlock, 5.610f, -2.620f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(1.540f + distanceFromBlock, 5.610f, -0.020f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(1.500f + distanceFromBlock, 5.610f, -4.260f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(4.170f + distanceFromBlock, 5.610f, 0.400f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		break;
	case 8:
		// 9th Block
		SpawnFunctions::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "StartPlatform", glm::vec3(-15.820f + distanceFromBlock, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-12.960f + distanceFromBlock, 5.610f, -4.910f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-6.240f + distanceFromBlock, 5.610f, -7.700f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-3.750f + distanceFromBlock, 5.610f, -6.440), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-0.970f + distanceFromBlock, 5.610f, -5.180f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(1.760f + distanceFromBlock, 5.610f, -3.880f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnSpeaker(scene, Speaker, SpeakerMaterial, "Speaker", glm::vec3(3.410f + distanceFromBlock, 6.410f, -0.350f), glm::vec3(90.0f, 0.0f, -32.000f), glm::vec3(0.500f, 0.500f, 0.500f));
		SpawnFunctions::SpawnWallJump(scene, WallJump, WallJumpMaterial, "Wall Jump", glm::vec3(-14.120f + distanceFromBlock, 5.610f, 3.070f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		SpawnFunctions::SpawnWallJump(scene, WallJump, WallJumpMaterial, "Wall Jump", glm::vec3(-11.360f + distanceFromBlock, 5.610f, 2.010f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		SpawnFunctions::SpawnBuilding(scene, Building, BuildingMaterial, "Building", glm::vec3(-8.640f + distanceFromBlock, 5.610f, -6.770f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.310f, 0.310f, 0.310f));
		SpawnFunctions::SpawnBuilding(scene, Building, BuildingMaterial, "Building", glm::vec3(-1.730f + distanceFromBlock, 5.610f, 4.910f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.310f, 0.310f, 0.310f));
		SpawnFunctions::SpawnBuilding(scene, Building, BuildingMaterial, "Building", glm::vec3(4.570f + distanceFromBlock, 5.610f, -6.630f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.310f, 0.310f, 0.310f));
		SpawnFunctions::SpawnGem(scene, BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem", 4, glm::vec3(1.760f + distanceFromBlock, 5.610f, -0.210f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
		SpawnFunctions::SpawnSuperSmallWallJump(scene, SuperSmallWallJump, SuperSmallWallJumpMaterial, "Super Small Wall Jump", glm::vec3(0.640f + distanceFromBlock, 5.610f, 0.640f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		SpawnFunctions::SpawnCollectable(scene, Vinyl, VinylMaterial, "Vinyl", glm::vec3(1.550f + distanceFromBlock, 5.610f, 1.260f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "EndPlatform", glm::vec3(9.180f + distanceFromBlock, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));

		// CDs for Block 9
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-12.620f, 5.610f, -4.080f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-12.820f, 5.610f, 0.540f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-12.820f, 5.610f, 2.660f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-12.820f, 5.610f, 4.580f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-8.610f, 5.610f, -0.360f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-6.120f, 5.610f, -7.060f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-3.730f, 5.610f, -5.880f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-0.960f, 5.610f, -4.590f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(1.690f, 5.610f, -3.270f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(4.490f, 5.610f, -0.460f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		break;
	case 9:
		// 10th Block
		SpawnFunctions::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "StartPlatform", glm::vec3(-15.820f + distanceFromBlock, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-12.500f + distanceFromBlock, 5.610f, -3.520f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-9.250f + distanceFromBlock, 5.610f, -3.520f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-5.870f + distanceFromBlock, 5.610f, -2.670f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-3.500f + distanceFromBlock, 5.610f, 4.630f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-0.840f + distanceFromBlock, 5.610f, 3.470f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnBuilding(scene, Building, BuildingMaterial, "Building", glm::vec3(4.570f + distanceFromBlock, 5.610f, -1.190f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.310f, 0.310f, 0.310f));
		SpawnFunctions::SpawnBuilding(scene, Building, BuildingMaterial, "Building", glm::vec3(4.570f + distanceFromBlock, 5.610f, -13.170f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.310f, 0.310f, 0.310f));
		SpawnFunctions::SpawnGem(scene, BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem", 4, glm::vec3(1.390f + distanceFromBlock, 5.610f + distanceFromBlock, 4.040f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
		SpawnFunctions::SpawnSuperSmallWallJump(scene, SuperSmallWallJump, SuperSmallWallJumpMaterial, "Super Small Wall Jump", glm::vec3(-4.870f + distanceFromBlock, 5.610f, 0.090f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		SpawnFunctions::SpawnSuperSmallWallJump(scene, SuperSmallWallJump, SuperSmallWallJumpMaterial, "Super Small Wall Jump", glm::vec3(-6.310f + distanceFromBlock, 5.610f, 1.520f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		SpawnFunctions::SpawnSuperSmallWallJump(scene, SuperSmallWallJump, SuperSmallWallJumpMaterial, "Super Small Wall Jump", glm::vec3(-4.870f + distanceFromBlock, 5.610f, 3.260f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		SpawnFunctions::SpawnSuperSmallWallJump(scene, SuperSmallWallJump, SuperSmallWallJumpMaterial, "Super Small Wall Jump", glm::vec3(-6.310f + distanceFromBlock, 5.610f, 4.720f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		SpawnFunctions::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "EndPlatform", glm::vec3(9.180f, 5.610f, -9.10f), glm::vec3(90.0f + distanceFromBlock, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));

		// CDs for Block 10
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-12.380f + distanceFromBlock, 5.610f, -2.980f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-9.160f + distanceFromBlock, 5.610f, -3.070f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-5.850f + distanceFromBlock, 5.610f, -2.180f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-3.450f + distanceFromBlock, 5.610f, 5.070f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-0.860f + distanceFromBlock, 5.610f, 3.940f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(1.310f + distanceFromBlock, 5.610f, 4.460f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(8.470f + distanceFromBlock, 5.610f, 2.110f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(8.470f + distanceFromBlock, 5.610f, -0.010f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(8.470f + distanceFromBlock, 5.610f, -2.180f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		break;
	default:
		// 1st Block		
		SpawnFunctions::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "StartPlatform", glm::vec3(-15.820f + distanceFromBlock, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-10.460f + distanceFromBlock, 5.610f, -4.150f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-3.940f + distanceFromBlock, 5.610f, -4.150f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(2.200f + distanceFromBlock, 5.610f, -1.770f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(6.500f + distanceFromBlock, 5.610f, -4.150f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		SpawnFunctions::SpawnGem(scene, BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem", 4, glm::vec3(-0.520f + distanceFromBlock, 5.610f, -3.160f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
		SpawnFunctions::SpawnGem(scene, BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem", 4, glm::vec3(-6.940f + distanceFromBlock, 5.610f, -3.160f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
		SpawnFunctions::SpawnCollectable(scene, Vinyl, VinylMaterial, "Vinyl", glm::vec3(5.210f + distanceFromBlock, 5.610f, 0.080f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "EndPlatform", glm::vec3(9.180f + distanceFromBlock, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));

		// CDs for Block 1
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-10.220f + distanceFromBlock, 5.610f, -3.220f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-6.970f + distanceFromBlock, 5.610f, -2.810f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-3.770f + distanceFromBlock, 5.610f, -3.190f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-0.620f + distanceFromBlock, 5.610f, -2.380f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		SpawnFunctions::SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(2.140f + distanceFromBlock, 5.610f, -0.770f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		break;
	}
}

void SpawnLoop::ToSpawn() {
	_SpawnTimer -= 22;
	_BlockToSpawn = rand() % 8;
	SpawnBlock();


}

void SpawnLoop::RenderImGui(){
	//ImGui::Text("Difficulty:   %s", "Easy");
	//ImGui::Separator();
	//ImGui::Text("Currently Spawning:	%s", _BlockToSpawn);
	//ImGui::Text("Time to Next Spawn:	%s", _SpawnTimer);
}
float SpawnLoop::GetSpawnTimer() {
	return _SpawnTimer;
}
SpawnLoop::Sptr SpawnLoop::FromJson(const nlohmann::json& blob) {
	SpawnLoop::Sptr result = std::make_shared<SpawnLoop>();
	result->_Difficulty = blob["Difficulty"];
	return result;
}

nlohmann::json SpawnLoop::ToJson() const {
	return {
		{ "Difficulty", _Difficulty }
	};
}
void SpawnLoop::Update(float dt) {

	_SpawnTimer += dt;

	Application& app = Application::Get();

	//Rotates our Skybox, it doesn't NEED to be in SpawnLoop, it could be any Component Attached to GameManager tbh
	glm::mat3 newRotation = glm::rotate(glm::mat3(1.0f), _SpawnTimer) * glm::rotate(glm::mat3(1.0f), 90.f);
	app.CurrentScene()->SetSkyboxRotation(newRotation);

	//LOG_INFO(_SpawnTimer);
	if (_SpawnTimer >= 22) {
		ToSpawn();
		//SpawnBlock();
	}

}
