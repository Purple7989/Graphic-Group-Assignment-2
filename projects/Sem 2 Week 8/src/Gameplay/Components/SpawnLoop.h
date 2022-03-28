#pragma once
#include "IComponent.h"
#include "Gameplay/Material.h"
#include "Gameplay/Scene.h"
#include "Gameplay/MeshResource.h"
#include "Application/Layers/SpawnFunctions.h"
#include "Utils/FileHelpers.h"

//Meant to be a singleton class please ONLY add this to the GameManager GameObject
class SpawnLoop : public Gameplay::IComponent
{
public:

	typedef std::shared_ptr<SpawnLoop> Sptr;

	enum Difficulty {
		Easy,
		Medium,
		Hard
	};

	Difficulty GetDifficulty() {
		return _Difficulty;
	}

	SpawnLoop();
	virtual ~SpawnLoop() = default;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;

	void ToSpawn();
	float GetSpawnTimer();
	void SpawnBlock();
	void CreateList(const nlohmann::json&);

	virtual void RenderImGui() override;
	MAKE_TYPENAME(SpawnLoop);

	virtual nlohmann::json ToJson() const override;
	static SpawnLoop::Sptr FromJson(const nlohmann::json& blob);
private:
	Difficulty _Difficulty;
	
	std::map<std::string, Guid> MaterialList;
	std::map<std::string, Guid> MeshList;
	int _BlockToSpawn;
	float _SpawnTimer = 0;
	bool _isDirty=false;

	//This is Disgusting.
	
	Gameplay::MeshResource::Sptr monkeyMesh;
	Gameplay::MeshResource::Sptr SmallPlatform;
	Gameplay::MeshResource::Sptr WallJump;
	Gameplay::MeshResource::Sptr BeatGem;
	Gameplay::MeshResource::Sptr Vinyl;
	Gameplay::MeshResource::Sptr CD;
	Gameplay::MeshResource::Sptr Building;
	Gameplay::MeshResource::Sptr KBuilding1Mesh;
	Gameplay::MeshResource::Sptr KBuilding2Mesh;
	Gameplay::MeshResource::Sptr KBuilding3Mesh;
	Gameplay::MeshResource::Sptr OvalBuilding;
	Gameplay::MeshResource::Sptr CharacterMesh;
	Gameplay::MeshResource::Sptr DiscoBallMesh;
	Gameplay::MeshResource::Sptr StartPlatform;
	Gameplay::MeshResource::Sptr Car1Mesh;
	Gameplay::MeshResource::Sptr SemiTruckMesh;
	Gameplay::MeshResource::Sptr PickupTruckMesh;
	Gameplay::MeshResource::Sptr SmallWallJump;
	Gameplay::MeshResource::Sptr SuperSmallWallJump;
	Gameplay::MeshResource::Sptr FallingPlat;
	Gameplay::MeshResource::Sptr HalfCirclePlat;
	Gameplay::MeshResource::Sptr StairsRight;
	Gameplay::MeshResource::Sptr StairsLeft;
	Gameplay::MeshResource::Sptr Speaker;
	Gameplay::MeshResource::Sptr SquarePlat;
	Gameplay::MeshResource::Sptr FloatingLight;
	Gameplay::MeshResource::Sptr DiscoBotMesh1;
	Gameplay::MeshResource::Sptr DiscoBotMesh2;
	Gameplay::MeshResource::Sptr DiscoBotMesh3;
	Gameplay::MeshResource::Sptr DiscoBotMesh4;
	Gameplay::MeshResource::Sptr DiscoBotMesh5;
	Gameplay::MeshResource::Sptr DiscoBotMesh6;
	Gameplay::MeshResource::Sptr DiscoBotMesh7;
	Gameplay::MeshResource::Sptr DiscoBotMesh8;
	Gameplay::MeshResource::Sptr DiscoBotMesh9;
	Gameplay::MeshResource::Sptr BotJump1;
	Gameplay::MeshResource::Sptr BotJump2;
	Gameplay::MeshResource::Sptr BotJump3;
	Gameplay::MeshResource::Sptr BotJump4;
	Gameplay::MeshResource::Sptr BotJump5;
	Gameplay::MeshResource::Sptr BotJump6;
	Gameplay::MeshResource::Sptr BotJump7;

	Gameplay::Material::Sptr StartPlatformMaterial;
	Gameplay::Material::Sptr UIMat;
	Gameplay::Material::Sptr SmallPlatformMaterial;
	Gameplay::Material::Sptr WallJumpMaterial;
	Gameplay::Material::Sptr BeatGemMaterial;
	Gameplay::Material::Sptr BeatGemAnticipationMaterial;
	Gameplay::Material::Sptr BeatGemOffMaterial;
	Gameplay::Material::Sptr VinylMaterial;
	Gameplay::Material::Sptr CDMaterial;
	Gameplay::Material::Sptr CharacterMaterial;
	Gameplay::Material::Sptr DiscoBallMaterial;
	Gameplay::Material::Sptr LoseScreenMaterial;
	Gameplay::Material::Sptr Car1Material;
	Gameplay::Material::Sptr SemiTruckMaterial;
	Gameplay::Material::Sptr PickupTruckMaterial;
	Gameplay::Material::Sptr BuildingMaterial;
	Gameplay::Material::Sptr KBuildingMaterial;
	Gameplay::Material::Sptr KBuilding2Material;
	Gameplay::Material::Sptr KBuilding3Material;
	Gameplay::Material::Sptr SmallWallJumpMaterial;
	Gameplay::Material::Sptr SuperSmallWallJumpMaterial;
	Gameplay::Material::Sptr PianoMaterial;
	Gameplay::Material::Sptr HalfCirclePlatMaterial;
	Gameplay::Material::Sptr StairsRightMaterial;
	Gameplay::Material::Sptr StairsLeftMaterial;
	Gameplay::Material::Sptr SpeakerMaterial;
	Gameplay::Material::Sptr SquarePlatMaterial;
	Gameplay::Material::Sptr FloatingLightMaterial;
	Gameplay::Material::Sptr OvalBuildingMaterial;
};

