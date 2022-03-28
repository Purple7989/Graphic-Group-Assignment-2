
#include <GLFW/glfw3.h>
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Utils/ImGuiHelper.h"
#include "BeatGem.h"
void BeatGem::Awake()
{

}

void BeatGem::RenderImGui() {

}

nlohmann::json BeatGem::ToJson() const {
    return {
    };
}
// Constructor Initializes Values for LERP and Set Position but Only SetPosition is being used atm
BeatGem::BeatGem()  : IComponent()
{ }
BeatGem::BeatGem(int num) 
{ 
    beatNum = num;
}


BeatGem::~BeatGem() = default;

BeatGem::Sptr BeatGem::FromJson(const nlohmann::json & blob) {
    BeatGem::Sptr result = std::make_shared<BeatGem>();
    return result;
}
int BeatGem::GetBeatNum(){
    return beatNum;
}


void BeatGem::Update(float deltaTime)
{


}