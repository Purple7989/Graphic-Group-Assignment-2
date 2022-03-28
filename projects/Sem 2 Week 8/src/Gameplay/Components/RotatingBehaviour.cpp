#include "Gameplay/Components/RotatingBehaviour.h"

#include "Gameplay/GameObject.h"

#include "Utils/ImGuiHelper.h"
#include "Utils/JsonGlmHelpers.h"

void RotatingBehaviour::Update(float deltaTime) {
	GetGameObject()->SetRotation(GetGameObject()->GetRotationEuler() + RotationSpeed * deltaTime);
}
glm::vec3 RotatingBehaviour::GetRotationSpeed() {
	return RotationSpeed;
}

void RotatingBehaviour::SetRotationSpeed(glm::vec3 value) {
	RotationSpeed = value;
}

void RotatingBehaviour::RenderImGui() {
	LABEL_LEFT(ImGui::DragFloat3, "Speed", &RotationSpeed.x);
}

nlohmann::json RotatingBehaviour::ToJson() const {
	return {
		{ "speed", RotationSpeed }
	};
}

RotatingBehaviour::Sptr RotatingBehaviour::FromJson(const nlohmann::json& data) {
	RotatingBehaviour::Sptr result = std::make_shared<RotatingBehaviour>();
	result->RotationSpeed = JsonGet(data, "speed", result->RotationSpeed);
	return result;
}

// ROTATION BEHAVIOURS FOR THE CDs

void RotatingBehaviourCD::Update(float deltaTime) {
	GetGameObject()->SetRotation(GetGameObject()->GetRotationEuler() + RotationCDSpeed * deltaTime);
}

void RotatingBehaviourCD::RenderImGui() {
	LABEL_LEFT(ImGui::DragFloat3, "Speed", &RotationCDSpeed.x);
}

nlohmann::json RotatingBehaviourCD::ToJson() const {
	return {
		{ "speed", RotationCDSpeed }
	};
}

RotatingBehaviourCD::Sptr RotatingBehaviourCD::FromJson(const nlohmann::json& data) {
	RotatingBehaviourCD::Sptr result = std::make_shared<RotatingBehaviourCD>();
	result->RotationCDSpeed = JsonGet(data, "speed", result->RotationCDSpeed);
	return result;
}

