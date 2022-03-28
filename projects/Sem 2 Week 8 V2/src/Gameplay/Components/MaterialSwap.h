#pragma once
#include "IComponent.h"
#include "BeatTimer.h"
#include "Gameplay/Components/RenderComponent.h"
#include "GLFW/glfw3.h"
#include "Utils/ResourceManager/ResourceManager.h"
#include "Utils/ResourceManager/IResource.h"
#include "Utils/TypeHelpers.h"


/// <summary>
/// Provides an example behaviour that uses some of the trigger interface to change the material
/// of the game object the component is attached to when entering or leaving a trigger
/// </summary>
class MaterialSwap: public Gameplay::IComponent {

public:
	typedef std::shared_ptr<MaterialSwap > Sptr;
	MaterialSwap(int num);
	MaterialSwap();
	virtual ~MaterialSwap ();


	
	template<typename T>
	T Lerp(const T& p0, const T& p1, float t);

	Gameplay::Material::Sptr        OnMaterial;
	Gameplay::Material::Sptr		AnticipationMaterial;
	Gameplay::Material::Sptr        OffMaterial;
	void Swap();
	// Inherited from IComponent	
	virtual void Awake() override;
	virtual void RenderImGui() override;
	virtual void Update(float deltaTime) override;
	virtual nlohmann::json ToJson() const override;
	static MaterialSwap ::Sptr FromJson(const nlohmann::json& blob);
	MAKE_TYPENAME(MaterialSwap);
private:
	int beatNumber;
protected:

	RenderComponent::Sptr _renderer;
};