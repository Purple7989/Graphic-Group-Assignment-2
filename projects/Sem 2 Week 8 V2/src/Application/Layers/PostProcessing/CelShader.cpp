#include "CelShader.h"
#include "Utils/ResourceManager/ResourceManager.h"
#include "Utils/JsonGlmHelpers.h"
#include "Utils/ImGuiHelper.h"

CelShaderEffect::CelShaderEffect() :
	CelShaderEffect(true) { }

CelShaderEffect::CelShaderEffect(bool Toon1DLut) :
	PostProcessingLayer::Effect(),
	_shader(nullptr),
	//_strength(1.0f),
	toonLUT(nullptr)
{
	Name = "Cel Shade Effect";
	_format = RenderTargetType::ColorRgb8;

	_shader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
		{ ShaderPartType::Vertex, "shaders/vertex_shaders/fullscreen_quad.glsl" },
		{ ShaderPartType::Fragment, "shaders/fragment_shaders/post_effects/Cel_Shader.glsl" }
	});

	if (Toon1DLut) {
		toonLUT = ResourceManager::CreateAsset<Texture1D>("luts/toon-1D.png");
	}

}

CelShaderEffect::~CelShaderEffect() = default;

void CelShaderEffect::Apply(const Framebuffer::Sptr & gBuffer)
{
	_shader->Bind();
	//Lut->Bind(1);
	//_shader->SetUniform("u_Strength", _strength);
}

void CelShaderEffect::RenderImGui()
{
	//LABEL_LEFT(ImGui::LabelText, "LUT", Lut ? Lut->GetDebugName().c_str() : "none");
	//LABEL_LEFT(ImGui::SliderFloat, "Strength", &_strength, 0, 1);
}

CelShaderEffect::Sptr CelShaderEffect::FromJson(const nlohmann::json & data)
{
	CelShaderEffect::Sptr result = std::make_shared<CelShaderEffect>(false);
	result->Enabled = JsonGet(data, "enabled", true);
	result->_strength = JsonGet(data, "strength", result->_strength);
	//result->Lut = ResourceManager::Get<Texture3D>(Guid(data["lut"].get<std::string>()));
	return result;
}

nlohmann::json CelShaderEffect::ToJson() const
{
	return {
		{ "enabled", Enabled },
		//{ "lut", Lut != nullptr ? Lut->GetGUID().str() : "null" },
		{ "strength", _strength }
	};
}