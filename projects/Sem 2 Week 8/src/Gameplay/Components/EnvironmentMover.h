#pragma once
#include "IComponent.h"
#include "Gameplay/Physics/RigidBody.h"
#include "GLFW/glfw3.h"
/// <summary>
/// This behaviour covers the blanket of possible movement options for environmental Objects
/// </summary>
class EnvironmentMover : public Gameplay::IComponent
{
public:
	typedef std::shared_ptr<EnvironmentMover> Sptr;

	EnvironmentMover();
	virtual ~EnvironmentMover();

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;

	virtual void RenderImGui() override;
	MAKE_TYPENAME(EnvironmentMover);

	virtual nlohmann::json ToJson() const override;
	static EnvironmentMover::Sptr FromJson(const nlohmann::json& blob);

private:
	Gameplay::Physics::RigidBody::Sptr _body;


};

