#pragma once
#include "IComponent.h"
#include "GLFW/glfw3.h"
#include "Gameplay/GameObject.h"
/// <summary>
/// A simple behaviour that applies an impulse along the Z axis to the 
/// rigidbody of the parent when the space key is pressed
/// </summary>
class BeatGem : public Gameplay::IComponent {
public:
	//engine stuffs
	typedef std::shared_ptr<BeatGem> Sptr;
	BeatGem();
	BeatGem(int num);
	virtual ~BeatGem();
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	template<typename T>
	T Lerp(const T& p0, const T& p1, float t);
	virtual void RenderImGui() override;
	MAKE_TYPENAME(BeatGem);
	virtual nlohmann::json ToJson() const override;
	static BeatGem::Sptr FromJson(const nlohmann::json& blob);

	//only purpose of this class is to give the character access to the 
	int GetBeatNum();
protected:
	int beatNum;
};