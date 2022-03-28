#pragma once
#include "Application/Layers/PostProcessingLayer.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Textures/Texture3D.h"
#include "Graphics/Textures/Texture1D.h"


class CelShaderEffect : public PostProcessingLayer::Effect {
public:
	MAKE_PTRS(CelShaderEffect);
	Texture1D::Sptr toonLUT;

	CelShaderEffect();
	CelShaderEffect(bool toon1DLut);
	virtual ~CelShaderEffect();

	virtual void Apply(const Framebuffer::Sptr& gBuffer) override;
	virtual void RenderImGui() override;

	// Inherited from IResource

	CelShaderEffect::Sptr FromJson(const nlohmann::json& data);
	virtual nlohmann::json ToJson() const override;

protected:
	ShaderProgram::Sptr _shader;
	float _strength;
};

