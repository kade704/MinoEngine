#pragma once

#include "AWidget.h"
#include "../Math/FVector2.h"
#include "../Converter.h"
#include "../TextureID.h"
#include <imgui.h>

namespace Widget
{
	class Image : public AWidget
	{
	public:
		Image(uint32_t p_textureID, const FVector2& p_size) :
			textureID{ p_textureID },
			size(p_size)
		{
		}

	protected:
		void _Draw_Impl() override
		{
			ImGui::Image((ImTextureID)textureID.raw, Converter::ToImVec2(size), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
		}

	public:
		TextureID textureID;
		FVector2 size;
	};
}