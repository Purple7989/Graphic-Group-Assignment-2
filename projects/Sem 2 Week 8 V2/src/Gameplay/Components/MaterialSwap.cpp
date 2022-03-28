#include "Gameplay/Components/MaterialSwap.h"
#include "Gameplay/Components/ComponentManager.h"
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include<string.h>

MaterialSwap::MaterialSwap(int num) :
	IComponent(),
	_renderer(nullptr),
	OnMaterial(nullptr),
	AnticipationMaterial(nullptr),
	OffMaterial(nullptr)
{ 
	beatNumber = num;
}
MaterialSwap::MaterialSwap() {}
MaterialSwap ::~MaterialSwap () = default;



void MaterialSwap::Swap(){
	//swaps based on BeatTime
	float beatTime = GetGameObject()->GetScene()->FindObjectByName("GameManager")->Get<BeatTimer>()->GetBeatTime();
	OnMaterial = GetGameObject()->GetScene()->FindObjectByName("Material Dummy On")->Get<RenderComponent>()->GetMaterial();
	AnticipationMaterial = GetGameObject()->GetScene()->FindObjectByName("MaterialDummyAnticipation")->Get<RenderComponent>()->GetMaterial();
	OffMaterial = GetGameObject()->GetScene()->FindObjectByName("Material Dummy Off")->Get<RenderComponent>()->GetMaterial();
	_renderer = GetComponent<RenderComponent>();

	//beat gem material swap behaviour
	
		if ((beatTime >= (0.6 * beatNumber) - 0.6) && (beatTime <= 0.6 * beatNumber)) {			
			_renderer->SetMaterial(OnMaterial);
			//GetGameObject()->SetScale(glm::vec3(1.2f, 1.2f, 1.2f));
		}
		else {
			//_renderer->SetMaterial(Lerp(OffMaterial,AnticipationMaterial,beatTime/1.2f));
			_renderer->SetMaterial(OffMaterial);
		}
		if (beatNumber != 1) {
			if ((beatTime >= (0.6 * (beatNumber - 1)) - 0.6) && (beatTime <= 0.6 * (beatNumber - 1))) {
				_renderer->SetMaterial(AnticipationMaterial);
			}
		}
		else if (beatNumber == 1) {
			if ((beatTime >=( 0.6 * (beatNumber +3) - 0.6)) && (beatTime <= 0.6 * (beatNumber +3))) {
				_renderer->SetMaterial(AnticipationMaterial);
			}
		}
}

void MaterialSwap ::Awake() {
	_renderer = GetComponent<RenderComponent>();
}

void MaterialSwap ::RenderImGui() { }

nlohmann::json MaterialSwap ::ToJson() const {
	return {
		{ "On_material", OnMaterial != nullptr ? OnMaterial->GetGUID().str() : "null" },
			{ "AnticipationMaterial", AnticipationMaterial != nullptr ? OnMaterial->GetGUID().str() : "null" },
		{ "Off_material", OffMaterial != nullptr ? OffMaterial->GetGUID().str() : "null" }
	};
}

MaterialSwap ::Sptr MaterialSwap ::FromJson(const nlohmann::json& blob) {
	MaterialSwap ::Sptr result = std::make_shared<MaterialSwap >();
	result->OnMaterial = ResourceManager::Get<Gameplay::Material>(Guid(blob["On_material"]));
	result->AnticipationMaterial = ResourceManager::Get<Gameplay::Material>(Guid(blob["AnticipationMaterial"]));
	result->OffMaterial  = ResourceManager::Get<Gameplay::Material>(Guid(blob["Off_material"]));
	return result;
}
template <typename T>
T MaterialSwap::Lerp(const T& p0, const T& p1, float t)
{
	return (1.0f - t) * p0 + t * p1;
}
void MaterialSwap::Update(float dt) {

	Swap();
}
