#include "Gameplay/Components/CharacterController.h"
#include <GLFW/glfw3.h>
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Utils/ImGuiHelper.h"
#include <iostream>
#include "Gameplay/Components/BeatTimer.h"
#include "Gameplay/Components/RenderComponent.h"
#include "Gameplay/Components/GUI/GuiPanel.h"
#include "Gameplay/Components/GUI/GuiText.h"
#include "FMOD/AudioEngine.h"
#include "Gameplay/InputEngine.h"
#include "Gameplay/Components/BeatGem.h"
#include "Application/Application.h"
#include "Gameplay/Scene.h"
#include<sstream>
#include<string.h>
#include <queue>
void CharacterController::Awake()
{
    _body = GetComponent<Gameplay::Physics::RigidBody>();
    if (_body == nullptr) {
        IsEnabled = false;
    }


    SFXS->SetVolume("event:/Coin Pickup", 0.25f);
    SFXS->SetVolume("event:/Jump", 0.05f);
    SFXS->SetVolume("event:/Death", 1.0f);
    SFXS->SetVolume("event:/Walk", 0.15f);
}

//don't mind me just giving this a touchy touch
void CharacterController::RenderImGui() {

}

nlohmann::json CharacterController::ToJson() const {
    return {
        { "impulse", _impulse }
    };
}

CharacterController::CharacterController() :
    IComponent()
{ 
    _CoyoteTimeUsed = true;
    _CoyoteTime = 0.0f;
    _AirSpeed = 1.0f;
    _Direction = 'U';
    _isJumping = false;
    _canJump = true;
    _OnPlatform = false;
    _PlatformName = "";
}

CharacterController::~CharacterController() = default;

CharacterController::Sptr CharacterController::FromJson(const nlohmann::json & blob) {
    CharacterController::Sptr result = std::make_shared<CharacterController>();
    return result;
}


//for collectibles
void CharacterController::OnEnteredTrigger(const std::shared_ptr<Gameplay::Physics::TriggerVolume>&trigger) {
    std::string name = trigger->GetGameObject()->Name;
    
    //beat gem logic
    if ((name[0] == 'B') && (name[1] == 'e') && (name[2] == 'a') && (name[3] == 't') && (name[4] == 'G')) {
        _PlatformName = "BeatGem";
        int beatNumber = trigger->GetGameObject()->Get<BeatGem>()->GetBeatNum();
        trigger->GetGameObject()->Get<BeatGem>()->GetBeatNum();
        if ((_GemJumpTimer > 0.6 * beatNumber - 0.6) && (_GemJumpTimer < 0.6 * beatNumber)) {
            _canJump = true;
            BeatGemsUsed.push_back(trigger);
            trigger->GetGameObject()->Get<RenderComponent>()->IsEnabled = false;
            _BeatGemHits++;
            score += 500; 
            SFXS->PlayEvent("event:/Coin Pickup");
        }
    }   
    //vinyl logic
    if (trigger->GetGameObject()->Name == "Vinyl") {
        score += 1000;
        _VinylScore++;
        SFXS->PlayEvent("event:/Coin Pickup");
        trigger->GetGameObject()->SetPostion(glm::vec3(0.0f, -100.0f, 0.0f));
    }
    //cd logic
    if (trigger->GetGameObject()->Name == "CD") {
        score += 100;
        _CDScore++;
        SFXS->PlayEvent("event:/Coin Pickup");
        trigger->GetGameObject()->SetPostion(glm::vec3(0.0f, -100.0f, 0.0f));
    }
    std::stringstream ss;
    ss << score;
    std::string stringScore;
    ss >> stringScore;

    GetGameObject()->GetScene()->FindObjectByName("HUD Score Text")->Get<GuiText>()->SetText(stringScore);

}

//for physical platforms
void CharacterController::OnTriggerVolumeEntered(const std::shared_ptr<Gameplay::Physics::RigidBody>&body) {
    speed = 3.0f;
    _OnPlatform = true;
    _CoyoteTimeUsed = false;
    LOG_INFO("Body has entered our trigger volume: {}", body->GetGameObject()->Name);
    if (_PlatformName != body->GetGameObject()->Name) {
        _canJump = true;
        _PlatformName = body->GetGameObject()->Name;
    }
    //make certain things fall when touched
    if (_PlatformName == "FallingPlatform") {
        body->SetType(RigidBodyType::Dynamic);
    }
    //rotate half circle platforms
    if (_PlatformName == "Half Circle Platform") {
        _rotPlat = (_body->GetGameObject()->GetPosition()) - body->GetGameObject()->GetPosition();
        body->GetGameObject()->SetRotation(body->GetGameObject()->GetRotationEuler() + glm::vec3(0.0f, -20 * _rotPlat.x, 0.0f));
        LOG_INFO(_rotPlat.x);
    }
}

void CharacterController::OnTriggerVolumeLeaving(const std::shared_ptr<Gameplay::Physics::RigidBody>&body) {
    LOG_INFO("Body has left our trigger volume: {}", body->GetGameObject()->Name);
    if (body->GetGameObject()->Name == "Half Circle Platform") {
        LOG_INFO("functions");
        body->GetGameObject()->SetRotation(glm::vec3(-90.000f, 0.0f, 180.0f));
    }
    //to make player move faster on wall jumps so that they are eZ
   
    if (_PlatformName == "Wall Jump") {
        speed = 4.0f;
    }

    // i forgor
    if ((_PlatformName != "BeatGem")&&(_PlatformName!="FallingPlatform")) {
        _PlatformName = "";
        _OnPlatform = false;
    }
 
}


void CharacterController::RespawnBeatGems(const std::vector<Gameplay::Physics::TriggerVolume::Sptr> trigger) {
    if (BeatGemsUsed.size() > 0) {
        int LoopLength = BeatGemsUsed.size();
        for (int i = 0; i < LoopLength; i++) {
            if (trigger[i]->GetGameObject()!= nullptr) {
                trigger[i]->GetGameObject()->Get<RenderComponent>()->IsEnabled = true;
            }   
           }
        BeatGemsUsed.clear();
    }
   
}

void CharacterController::AirControl(char Direction) {
    if (_isJumping == true) {
        //if character is now travelling in a direction they were not previously travelling
        if (_Direction != Direction) {
            _Direction = Direction;
            _AirSpeed = 2.0f;
        }
        //creates an initial burst of speed on movement startup creating greater agility
        else if (_Direction == Direction) {
            if (_AirSpeed > 1.0f) {
                _AirSpeed -= 0.05f;
            }
        }
   }
   
}

void CharacterController::CoyoteTime(float dt) {
    if ((_OnPlatform == false)&&(_PlatformName!="BeatGem")&&(_CoyoteTimeUsed==false)) {
       
        if (_CoyoteTime > 1.0f) {
            _CoyoteTimeUsed = true;
            _canJump = false;
            _CoyoteTime = 0.0f;
        }
        
        else { 
            _CoyoteTime += dt;
            _body->SetLinearVelocity(glm::vec3(_body->GetLinearVelocity().x, _body->GetLinearVelocity().y, 0.0f));
        }
    }
}
void CharacterController::Update(float deltaTime) {
    std::cout << _PlatformName << std::endl;
    _GemJumpTimer = GetGameObject()->GetScene()->FindObjectByName("GameManager")->Get<BeatTimer>()->GetBeatTime();
    glm::vec3 CurrentPosition = GetGameObject()->GetPosition();
    bool _A = InputEngine::IsKeyDown(GLFW_KEY_A);
    bool _D = InputEngine::IsKeyDown(GLFW_KEY_D);
    bool _W = InputEngine::IsKeyDown(GLFW_KEY_SPACE);
   
    if (_A) {
        AirControl('A');
       _body->SetLinearVelocity(glm::vec3(-speed*_AirSpeed, _body->GetLinearVelocity().y, _body->GetLinearVelocity().z));
        SFXS->PlayEvent("event:/Walk");
    }

    if (_D) {
        AirControl('D');
        _body->SetLinearVelocity(glm::vec3(speed*_AirSpeed, _body->GetLinearVelocity().y, _body->GetLinearVelocity().z));
    }

    if ((_W) && (_canJump == true)) {
        _CoyoteTimeUsed = true;
        _isJumping = true;
        _body->SetLinearVelocity(glm::vec3(_body->GetLinearVelocity().x, _body->GetLinearVelocity().y, _impulse.z * (speed / 3)));
        _canJump = false;
        SFXS->PlayEvent("event:/Jump");
    }
 
    if ((!_A) && (!_D) && (!_W) && (_PlatformName != "") && (_PlatformName != "BeatGem")) {
        _body->SetLinearVelocity(glm::vec3(-1.0f, 0.0f, 0.0f));
    }

    GetGameObject()->SetPostion({ CurrentPosition.x, 5.61f, CurrentPosition.z });
    _body->GetGameObject()->SetRotation(glm::vec3(90.0f, 0.0f, 90.0f));

    if (_PlatformName == "Wall Jump") {
        if (_body->GetLinearVelocity().z < 0) {
            _body->ApplyForce(glm::vec3(-6 * _body->GetLinearVelocity().x, 0.0f, 35.0f));
        }
    }
    CoyoteTime(deltaTime);
    // when the player is under the bottom frame of the screen aka. when the player dies
    if (GetGameObject()->GetPosition().z <= -14.5f)
    {
        SFXS->PlayEvent("event:/Death");
    
          RespawnBeatGems(BeatGemsUsed);
        
      
        // Activate GameOver U.I. When the player dies!
        //GetGameObject()->GetScene()->FindObjectByName("GameOver Dimmed Background")->Get<GuiPanel>()->IsEnabled = (GetGameObject()->GetScene()->FindObjectByName("GameOver Dimmed Background")->Get<GuiPanel>()->IsEnabled) = true;
        //GetGameObject()->GetScene()->FindObjectByName("GameOver Text")->Get<GuiPanel>()->IsEnabled = (GetGameObject()->GetScene()->FindObjectByName("GameOver Text")->Get<GuiPanel>()->IsEnabled) = true;
        //GetGameObject()->GetScene()->FindObjectByName("GameOver Score Breakdown")->Get<GuiPanel>()->IsEnabled = GetGameObject()->GetScene()->FindObjectByName("GameOver Score Breakdown")->Get<GuiPanel>()->IsEnabled = true;
        //GetGameObject()->GetScene()->FindObjectByName("GameOver Quit Button")->Get<GuiPanel>()->IsEnabled = GetGameObject()->GetScene()->FindObjectByName("GameOver Quit Button")->Get<GuiPanel>()->IsEnabled = true;
        //GetGameObject()->GetScene()->FindObjectByName("GameOver Continue Button")->Get<GuiPanel>()->IsEnabled = GetGameObject()->GetScene()->FindObjectByName("GameOver Continue Button")->Get<GuiPanel>()->IsEnabled = true;
        //GetGameObject()->GetScene()->FindObjectByName("GameOver Score Text")->Get<GuiText>()->IsEnabled = GetGameObject()->GetScene()->FindObjectByName("GameOver Score Text")->Get<GuiText>()->IsEnabled = true;

        // when the player dies set the position, score and velocity back to a default value
        GetGameObject()->SetPostion(glm::vec3(-15.820f, 5.710f, 0.0f));
        score = 0;
        _body->SetLinearVelocity(glm::vec3(0.0f, 0.0f, 0.0f));

        

        Application& app = Application::Get();
        //Change this to GameOver.Json once it exists :^)
        app.LoadScene("MainMenu.json");


    }
}


//GetGameObject()->GetScene()->Lights[1].Position = GetGameObject()->GetPosition();
