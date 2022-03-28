#include "Gameplay/Components/BuildingAnim.h"
#include <GLFW/glfw3.h>
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Utils/ImGuiHelper.h"
#include "BeatTimer.h"

void BuildingAnim::Awake()
{
    _body = GetComponent<Gameplay::Physics::RigidBody>();



    if (_body == nullptr) {
        IsEnabled = false;
    }
    // glm::vec2 ScaleXY = { GetGameObject()->GetScale().x, GetGameObject()->GetScale().y };

  //  keypoints.push_back(ScaleXY + glm::vec2{ 1,0 });
    _BuildingPresets = 1;
    switch (_BuildingPresets)
    {
        // Present 0 Goes up stays for half a seconds goes down stays half a second
        case 0: 
            keypoints.push_back(GetGameObject()->GetScale().y + 0.1);
            keypoints.push_back(GetGameObject()->GetScale().y + 0.01);
            keypoints.push_back(GetGameObject()->GetScale().y + 0.001);
            keypoints.push_back(GetGameObject()->GetScale().y - 0.4);
            keypoints.push_back(GetGameObject()->GetScale().y - 0.41);
            keypoints.push_back(GetGameObject()->GetScale().y - 0.411);
            keypoints.push_back(GetGameObject()->GetScale().y + 0.1);
            break;
        // Preset 1 Goes up then middle then down
        case 1:
            keypoints.push_back(GetGameObject()->GetScale().y + 0.01);
           // keypoints.push_back(GetGameObject()->GetScale().y);
            keypoints.push_back(GetGameObject()->GetScale().y - 0.4);
           // keypoints.push_back(GetGameObject()->GetScale().y);
            keypoints.push_back(GetGameObject()->GetScale().y + 0.01);
            break;
        // Preset 2 Description
        case 2:
            keypoints.push_back(GetGameObject()->GetScale().y + 0.1);
            keypoints.push_back(GetGameObject()->GetScale().y + 0.01);
            keypoints.push_back(GetGameObject()->GetScale().y + 0.001);
            keypoints.push_back(GetGameObject()->GetScale().y - 0.4);
            keypoints.push_back(GetGameObject()->GetScale().y - 0.41);
            keypoints.push_back(GetGameObject()->GetScale().y - 0.411);
            keypoints.push_back(GetGameObject()->GetScale().y + 0.1);

            keypoints.push_back(GetGameObject()->GetScale().y + 0.01);
            keypoints.push_back(GetGameObject()->GetScale().y + 0.001);
            keypoints.push_back(GetGameObject()->GetScale().y - 0.4);
            keypoints.push_back(GetGameObject()->GetScale().y - 0.41);
            keypoints.push_back(GetGameObject()->GetScale().y - 0.411);
            keypoints.push_back(GetGameObject()->GetScale().y - 0.4111);
            keypoints.push_back(GetGameObject()->GetScale().y - 0.41111);
            keypoints.push_back(GetGameObject()->GetScale().y - 0.411111);
            keypoints.push_back(GetGameObject()->GetScale().y - 0.4111111);
            keypoints.push_back(GetGameObject()->GetScale().y + 0.1);
            break;
        // The Same as preset 1
        default:
            keypoints.push_back(GetGameObject()->GetScale().y + 0.1);
            keypoints.push_back(GetGameObject()->GetScale().y + 0.01);
            keypoints.push_back(GetGameObject()->GetScale().y + 0.001);
            keypoints.push_back(GetGameObject()->GetScale().y - 0.4);
            keypoints.push_back(GetGameObject()->GetScale().y - 0.41);
            keypoints.push_back(GetGameObject()->GetScale().y - 0.411);
            keypoints.push_back(GetGameObject()->GetScale().y + 0.1);
            break;
    }


    // Preset 2 Goes up then middle then down
  //  keypoints.push_back(GetGameObject()->GetScale().y + 0.5);
  //  keypoints.push_back(GetGameObject()->GetScale().y);
  //  keypoints.push_back(GetGameObject()->GetScale().y - 0.5);
  //  keypoints.push_back(GetGameObject()->GetScale().y);
  //  keypoints.push_back(GetGameObject()->GetScale().y + 0.5);
    

    _journeyLength = std::abs(keypoints[keyframe] - keypoints[keyframe + 1]);
}

void BuildingAnim::RenderImGui() {

}

nlohmann::json BuildingAnim::ToJson() const {
    return {
    };
}
// Constructor Initializes Values for LERP and Set Position but Only SetPosition is being used atm
BuildingAnim::BuildingAnim() :
    IComponent(),
    _segmentIndex(0),
    // for preset 0 make _Travel Time 2.0f
    _TravelTime(1.2f),
    _startTime(0.f),
    _timeStored(0.f),
    keyframe(0.f),
    _speed(0.5f),
    _journeyLength(0.f),
    _BuildingPresets(0),


    _timer(1.0f),
    ObjY(0.0f),
    ObjZ(0.0f),
    ObjX(0.0f)

{ }

BuildingAnim::~BuildingAnim() = default;

BuildingAnim::Sptr BuildingAnim::FromJson(const nlohmann::json & blob) {
    BuildingAnim::Sptr result = std::make_shared<BuildingAnim>();
    return result;
}


void BuildingAnim::Update(float deltaTime)
{

    // Object with behavior attached Y and Z Scale
    ObjZ = GetGameObject()->GetScale().z;
    ObjX = GetGameObject()->GetScale().x;

    _timer += deltaTime;

    //Make this sync with beat Exactly for sem 2
    // problems: timer resets, so it also resets our lerp t value :(
    //_timer = GetGameObject()->GetScene()->FindObjectByName("GameManager")->Get<BeatTimer>()->GetBeatTime();

    // Distance moved equals elapsed time times speed..
    float distCovered = (_timer - _startTime - _timeStored) * _speed;

    // Fraction of journey completed equals current distance divided by total distance.
   
    float fractionOfJourney;

    // if you use any of the presets you will need to adjust _travelTime
    if (_BuildingPresets == 1)
    {
        fractionOfJourney = (distCovered / _journeyLength) * _TravelTime;
    }
    else if (_BuildingPresets == 0)
    {
        fractionOfJourney = (distCovered / _journeyLength) / _TravelTime;
    }
    else
    {
        fractionOfJourney = (distCovered / _journeyLength);
    }
   

    if (keyframe == keypoints.size() - 1)
    {
        keyframe = 0;
    }

    float sqt = (fractionOfJourney) * (fractionOfJourney);

    float SlowInOut = sqt / (2.0f * (sqt - fractionOfJourney) + 1.0f);

    GetGameObject()->SetScale(glm::vec3(ObjX, Lerp(keypoints[keyframe], keypoints[keyframe + 1],  SlowInOut), ObjZ));

    if ((fractionOfJourney >= 1.f) && (keyframe != keypoints.size() - 1))
    {
        _timeStored = _timer - _startTime;
        keyframe++;
    }

}

// Templated LERP function returns Scale at current time for LERP
template <typename T>
T BuildingAnim::Lerp(const T & p0, const T & p1, float t)
{
    return (1.0f - t) * p0 + t * p1;
}