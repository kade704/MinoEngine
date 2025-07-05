#pragma once

#include <map>

#include "DataWidget.h"
#include "../Event.h"
 
namespace Widget
{
	class ComboBox : public DataWidget<int>
	{
	public:
		ComboBox(int p_currentChoice = 0) :
			DataWidget<int>(currentChoice),
			currentChoice(p_currentChoice)
		{

		}

	protected:
		void _Draw_Impl() override
		{
			if (choices.find(currentChoice) == choices.end())
				currentChoice = choices.begin()->first;

			if (ImGui::BeginCombo(m_widgetID.c_str(), choices[currentChoice].c_str()))
			{
				for (const auto& [key, value] : choices)
				{
					bool selected = key == currentChoice;

					if (ImGui::Selectable(value.c_str(), selected))
					{
						if (!selected)
						{
							ImGui::SetItemDefaultFocus();
							currentChoice = key;
							ValueChangedEvent.Invoke(currentChoice);
							this->NotifyChange();
						}
					}
				}

				ImGui::EndCombo();
			}
		}

	public:
		std::map<int, std::string> choices;
		int currentChoice;

	public:
		Event<int> ValueChangedEvent;
	};
}