#include <Canvas.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

void Canvas::AddPanel(Panel::APanel& p_panel)
{
	m_panels.push_back(std::ref(p_panel));
}

void Canvas::RemovePanel(Panel::APanel& p_panel)
{
	m_panels.erase(std::remove_if(m_panels.begin(), m_panels.end(), [&p_panel](std::reference_wrapper<Panel::APanel>& p_item)
		{
			return &p_panel == &p_item.get();
		}));
}

void Canvas::Draw()
{
	if (!m_panels.empty())
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        // ImGuiViewport* viewport = ImGui::GetMainViewport();
        // ImGui::SetNextWindowPos(viewport->Pos);
        // ImGui::SetNextWindowSize(viewport->Size);
        // ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::SetWindowPos({ 0.f, 0.f });
        ImVec2 displaySize = ImGui::GetIO().DisplaySize;
        ImGui::SetWindowSize({ (float)displaySize.x, (float)displaySize.y });

        ImGui::PopStyleVar(3);

        for (auto& panel : m_panels)
            panel.get().Draw();

        ImGui::Render();
	}
}
