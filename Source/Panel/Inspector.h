#pragma once

#include "PanelWindow.h"
#include "../Actor.h"
#include "../Widget/Group.h"
#include "../Widget/ComboBox.h"

namespace Panel
{
	class Inspector : public PanelWindow
	{
	public:
		Inspector(const std::string& p_title);
		~Inspector();

		void FocusActor(Actor& p_target);
		void Unfocus();
		void SoftUnfocus();
		Actor* GetTargetActor() const;
		void Refresh();

		void CreateActorInspector(Actor& p_target);
		void DrawComponent(Component::AComponent& p_component);
		void DrawBehaviour(Component::Behaviour& p_behaviour);

	private:
		Actor* m_targetActor = nullptr;
		Widget::Group* m_inspectorHeader;
		Widget::Group* m_actorInfo;
		Widget::ComboBox* m_componentSelectorWidget;
		uint64_t m_componentAddedListener = 0;
		uint64_t m_componentRemovedListener = 0;
		uint64_t m_destroyedListener = 0;
	};
}
