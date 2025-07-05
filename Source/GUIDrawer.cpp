#include "GUIDrawer.h"
#include "Widget/TextColored.h"
#include "Widget/InputText.h"
#include "Widget/DragMultipleScalars.h"
#include "Widget/Text.h"
#include "Widget/Group.h"
#include "DDTarget.h"
#include "PathParser.h"
#include "ServiceLocator.h"
#include "Manager/ModelManager.h"
#include "Widget/ColorEdit.h"

const Color GUIDrawer::TitleColor = { 0.85f, 0.65f, 0.0f };
const Color GUIDrawer::ClearButtonColor = { 0.5f, 0.0f, 0.0f };
const float GUIDrawer::_MIN_FLOAT = -999999999.f;
const float GUIDrawer::_MAX_FLOAT = +999999999.f;

void GUIDrawer::CreateTitle(WidgetContainer& p_root, const std::string& p_name)
{
	p_root.CreateWidget<Widget::TextColored>(p_name, TitleColor);
}

void GUIDrawer::DrawVec2(WidgetContainer& p_root, const std::string& p_name, FVector2& p_data, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<Widget::DragMultipleScalars<float, 2>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<DataDispatcher<std::array<float, 2>>>();
	dispatcher.RegisterReference(reinterpret_cast<std::array<float, 2>&>(p_data));
}

void GUIDrawer::DrawVec3(WidgetContainer& p_root, const std::string& p_name, FVector3& p_data, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<Widget::DragMultipleScalars<float, 3>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<DataDispatcher<std::array<float, 3>>>();
	dispatcher.RegisterReference(reinterpret_cast<std::array<float, 3>&>(p_data));
}

void GUIDrawer::DrawString(WidgetContainer& p_root, const std::string& p_name, std::string& p_data)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<Widget::InputText>("");
	auto& dispatcher = widget.AddPlugin<DataDispatcher<std::string>>();
	dispatcher.RegisterReference(p_data);
}

void GUIDrawer::DrawVec2(WidgetContainer& p_root, const std::string& p_name, std::function<FVector2(void)> p_gatherer, std::function<void(FVector2)> p_provider, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<Widget::DragMultipleScalars<float, 2>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<DataDispatcher<std::array<float, 2>>>();

	dispatcher.RegisterGatherer([p_gatherer]()
		{
			FVector2 value = p_gatherer();
	return reinterpret_cast<const std::array<float, 2>&>(value);
		});

	dispatcher.RegisterProvider([p_provider](std::array<float, 2> p_value)
		{
			p_provider(reinterpret_cast<FVector2&>(p_value));
		});
}

void GUIDrawer::DrawVec3(WidgetContainer& p_root, const std::string& p_name, std::function<FVector3(void)> p_gatherer, std::function<void(FVector3)> p_provider, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<Widget::DragMultipleScalars<float, 3>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<DataDispatcher<std::array<float, 3>>>();

	dispatcher.RegisterGatherer([p_gatherer]()
		{
			FVector3 value = p_gatherer();
	return reinterpret_cast<const std::array<float, 3>&>(value);
		});

	dispatcher.RegisterProvider([p_provider](std::array<float, 3> p_value)
		{
			p_provider(reinterpret_cast<FVector3&>(p_value));
		});
}

void GUIDrawer::DrawString(WidgetContainer& p_root, const std::string& p_name, std::function<std::string(void)> p_gatherer, std::function<void(std::string)> p_provider)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<Widget::InputText>("");
	auto& dispatcher = widget.AddPlugin<DataDispatcher<std::string>>();
	dispatcher.RegisterGatherer(p_gatherer);
	dispatcher.RegisterProvider(p_provider);
}

void GUIDrawer::DrawMesh(WidgetContainer& p_root, const std::string& p_name, Model*& p_data, Event<>* p_updateNotifier)
{
	CreateTitle(p_root, p_name);
	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto& rightSide = p_root.CreateWidget<Widget::Group>();
	auto& widget = rightSide.CreateWidget<Widget::Text>(displayedText);
	widget.AddPlugin<DDTarget<std::pair<std::string, Widget::Group*>>>("File").DataReceivedEvent += [&widget, &p_data, p_updateNotifier](auto p_receivedData)
	{
		if (PathParser::GetFileType(p_receivedData.first) == PathParser::EFileType::MODEL)
		{
			if (auto resource = MINO_SERVICE(ModelManager).GetResource(p_receivedData.first); resource)
			{
				p_data = resource;
				widget.content = p_receivedData.first;
				if (p_updateNotifier)
					p_updateNotifier->Invoke();
			}
		}
	};
}

void GUIDrawer::DrawColor(WidgetContainer& p_root, const std::string& p_name, Color& p_color, bool p_hasAlpha)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<Widget::ColorEdit>(p_hasAlpha);
	auto& dispatcher = widget.AddPlugin<DataDispatcher<Color>>();
	dispatcher.RegisterReference(p_color);
}
