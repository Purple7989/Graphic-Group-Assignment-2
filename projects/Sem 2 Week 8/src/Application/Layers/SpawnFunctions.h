#pragma once
#include "Application/ApplicationLayer.h"
#include "json.hpp"
#include "../../Gameplay/Scene.h"
#include "../../Gameplay/MeshResource.h"
#include "../../Gameplay/Material.h"
#include "../../Gameplay/GameObject.h"

class SpawnFunctions {

public:
	static void SpawnObj(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName, glm::vec3 pos,
		glm::vec3 rot, glm::vec3 scale);
	static void SpawnStartPlat(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName, glm::vec3 pos,
		glm::vec3 rot, glm::vec3 scale);
	static void SpawnGem(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr MaterialOn, Gameplay::Material::Sptr MaterialOff, std::string ObjName,int beatNum, glm::vec3 pos,
		glm::vec3 rot, glm::vec3 scale);
	static void SpawnCollectable(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName, glm::vec3 pos,
		glm::vec3 rot, glm::vec3 scale);
	static void SpawnCD(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName, glm::vec3 pos,
		glm::vec3 rot, glm::vec3 scale);
	static void SpawnWallJump(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName, glm::vec3 pos,
		glm::vec3 rot, glm::vec3 scale);
	static void SpawnBuilding(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName, glm::vec3 pos,
		glm::vec3 rot, glm::vec3 scale);
	static void SpawnBuilding2(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName, glm::vec3 pos,
		glm::vec3 rot, glm::vec3 scale);
	static void SpawnBuilding3(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName, glm::vec3 pos,
		glm::vec3 rot, glm::vec3 scale);
	static void SpawnSmallWallJump(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName, glm::vec3 pos,
		glm::vec3 rot, glm::vec3 scale);
	static void SpawnSuperSmallWallJump(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName, glm::vec3 pos,
		glm::vec3 rot, glm::vec3 scale);
	static void SpawnStairsRight(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName, glm::vec3 pos,
		glm::vec3 rot, glm::vec3 scale);
	static void SpawnStairsLeft(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName, glm::vec3 pos,
		glm::vec3 rot, glm::vec3 scale);
	static void SpawnSpeaker(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName, glm::vec3 pos,
		glm::vec3 rot, glm::vec3 scale);
	static void SpawnSquarePlat(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName, glm::vec3 pos,
		glm::vec3 rot, glm::vec3 scale);
	static void SpawnFloatingLights(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName, glm::vec3 pos,
		glm::vec3 rot, glm::vec3 scale);
	static void SpawnBackGroundCar(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName, glm::vec3 pos,
		glm::vec3 rot, glm::vec3 scale);
	static void SpawnForeGroundCar(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName, glm::vec3 pos,
		glm::vec3 rot, glm::vec3 scale);
	static void SpawnBackGroundBuilding(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName, glm::vec3 pos,
		glm::vec3 rot, glm::vec3 scale);

};