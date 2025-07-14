#pragma once

#include "AButton.h"
#include "../Math/FVector2.h"
#include "../Color.h"
#include "../TextureID.h"
#include "../Converter.h"

#include <imgui.h>

namespace Widget
{
	class ButtonImage : public AButton
	{
	public:
		ButtonImage(const std::string& p_id, uint32_t p_textureID, const FVector2& p_size) :
			id(p_id),
			textureID{ p_textureID },
			size(p_size)
		{
		}

	protected:
		void _Draw_Impl() override
		{
			ImVec4 bg = Converter::ToImVec4(background);
			ImVec4 tn = Converter::ToImVec4(tint);

			if (ImGui::ImageButton(("##img_btn_" + id).c_str(), (ImTextureID)textureID.raw, Converter::ToImVec2(size), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f), bg, tn))
				ClickedEvent.Invoke();
		}

	public:
		Color background = { 0, 0, 0, 0 };
		Color tint = { 1, 1, 1, 1 };

		std::string id;
		TextureID textureID;
		FVector2 size;
	};
}