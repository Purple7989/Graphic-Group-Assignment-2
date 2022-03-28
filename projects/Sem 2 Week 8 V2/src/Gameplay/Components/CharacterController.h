#pragma once
#include "IComponent.h"
#include "Gameplay/Physics/RigidBody.h"
#include "GLFW/glfw3.h"
#include "Gameplay/Physics/TriggerVolume.h"
#include "Utils/ResourceManager/ResourceManager.h"
#include "Utils/ResourceManager/IResource.h"
#include "Utils/TypeHelpers.h"
#include "FMOD/AudioEngine.h"

/// <summary>
/// This Behaviour is our Main Component for the movement of player characters
/// </summary>
class CharacterController : public Gameplay::IComponent {
public:
	typedef std::shared_ptr<CharacterController> Sptr;

	CharacterController();
	virtual ~CharacterController();

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	
public:
	virtual void RenderImGui() override;
	MAKE_TYPENAME(CharacterController);

	virtual nlohmann::json ToJson() const override;
	static CharacterController::Sptr FromJson(const nlohmann::json& blob);
	virtual void OnEnteredTrigger(const std::shared_ptr<Gameplay::Physics::TriggerVolume>& trigger);
	virtual void OnTriggerVolumeEntered(const std::shared_ptr<Gameplay::Physics::RigidBody>& body);
	virtual void OnTriggerVolumeLeaving(const std::shared_ptr<Gameplay::Physics::RigidBody>& body);
	void RespawnBeatGems(const std::vector<Gameplay::Physics::TriggerVolume::Sptr> trigger);
	void AirControl(char);
	void CoyoteTime(float);
protected:
	float _GemJumpTimer, _AirSpeed, speed = 3.0f, _CoyoteTime;
	bool _canJump, _isJumping, _OnPlatform,_CoyoteTimeUsed;
	int score = 0, _VinylScore = 0, _CDScore = 0, _BeatGemHits = 0;
	char _Direction;
	std::string _PlatformName;
	glm::vec3 _impulse = glm::vec3(0, 0, 12.0);
	Gameplay::Physics::RigidBody::Sptr _body;
	Gameplay::Physics::RigidBody::Sptr _curvePlatform;
	//vector to store which gems have been used so that they may be reactivated upon respawn 
	std::vector  <Gameplay::Physics::TriggerVolume::Sptr> BeatGemsUsed;
	glm::vec3 _rotPlat = glm::vec3(0.0f, 0.0f, 0.0f);
	int _LoadSceneC = 0;

	ToneFire::StudioSound* SFXS = AudioEngine::GetContextBanks();

};