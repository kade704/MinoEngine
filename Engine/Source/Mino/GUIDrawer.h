#pragma once

#include "WidgetContainer.h"
#include "Widget/DragSingleScalar.h"
#include "Math/FVector2.h"
#include "Math/FVector3.h"
#include "Color.h"
#include "Resource/Model.h"

#include <imgui.h>

class GUIDrawer
{
public:
	static const Color TitleColor;
	static const Color ClearButtonColor;

	static const float _MIN_FLOAT;
	static const float _MAX_FLOAT;

	static void CreateTitle(WidgetContainer& p_root, const std::string& p_name);

	template <typename T>
	static void DrawScalar(WidgetContainer& p_root, const std::string& p_name, T& p_data, float p_step = 1.f, T p_min = std::numeric_limits<T>::min(), T p_max = std::numeric_limits<T>::max())
	{
		static_assert(std::is_scalar<T>::value, "T must be a scalar");

		CreateTitle(p_root, p_name);
		auto& widget = p_root.CreateWidget<Widget::DragSingleScalar<T>>(GetDataType<T>(), p_min, p_max, p_data, p_step, "", GetFormat<T>());
		auto& dispatcher = widget.AddPlugin<DataDispatcher<T>>();
		dispatcher.RegisterReference(p_data);
	}

	static void DrawVec2(WidgetContainer& p_root, const std::string& p_name, FVector2& p_data, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
	static void DrawVec3(WidgetContainer& p_root, const std::string& p_name, FVector3& p_data, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
	static void DrawString(WidgetContainer& p_root, const std::string& p_name, std::string& p_data);
	static void DrawColor(WidgetContainer& p_root, const std::string& p_name, Color& p_color, bool p_hasAlpha = false);

	template <typename T>
	static void DrawScalar(WidgetContainer& p_root, const std::string& p_name, std::function<T(void)> p_gatherer, std::function<void(T)> p_provider, float p_step = 1.f, T p_min = std::numeric_limits<T>::min(), T p_max = std::numeric_limits<T>::max())
	{
		static_assert(std::is_scalar<T>::value, "T must be a scalar");

		CreateTitle(p_root, p_name);
		auto& widget = p_root.CreateWidget<Widget::DragSingleScalar<T>>(GetDataType<T>(), p_min, p_max, static_cast<T>(0), p_step, "", GetFormat<T>());
		auto& dispatcher = widget.AddPlugin<DataDispatcher<T>>();
		dispatcher.RegisterGatherer(p_gatherer);
		dispatcher.RegisterProvider(p_provider);
	}

	static void DrawVec2(WidgetContainer& p_root, const std::string& p_name, std::function<FVector2(void)> p_gatherer, std::function<void(FVector2)> p_provider, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
	static void DrawVec3(WidgetContainer& p_root, const std::string& p_name, std::function<FVector3(void)> p_gatherer, std::function<void(FVector3)> p_provider, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
	static void DrawString(WidgetContainer& p_root, const std::string& p_name, std::function<std::string(void)> p_gatherer, std::function<void(std::string)> p_provider);

	static void DrawMesh(WidgetContainer& p_root, const std::string& p_name, Model*& p_data, Event<>* p_updateNotifier);
	static void DrawColor(WidgetContainer& p_root, const std::string& p_name, std::function<Color(void)> p_gatherer, std::function<void(Color)> p_provider, bool p_hasAlpha = false);

	template <typename T>
	static ImGuiDataType_ GetDataType()
	{
		if constexpr (std::is_same<T, float>::value)			return ImGuiDataType_Float;
		else if constexpr (std::is_same<T, double>::value)		return ImGuiDataType_Double;
		else if constexpr (std::is_same<T, uint8_t>::value)		return ImGuiDataType_U8;
		else if constexpr (std::is_same<T, uint16_t>::value)	return ImGuiDataType_U16;
		else if constexpr (std::is_same<T, uint32_t>::value)	return ImGuiDataType_U32;
		else if constexpr (std::is_same<T, uint64_t>::value)	return ImGuiDataType_U64;
		else if constexpr (std::is_same<T, int8_t>::value)		return ImGuiDataType_S8;
		else if constexpr (std::is_same<T, int16_t>::value)		return ImGuiDataType_S16;
		else if constexpr (std::is_same<T, int32_t>::value)		return ImGuiDataType_S32;
		else if constexpr (std::is_same<T, int64_t>::value)		return ImGuiDataType_S64;
	}

	template<typename T>
	static std::string GetFormat()
	{
		if constexpr (std::is_same<T, double>::value) return "%.5f";
		else if constexpr (std::is_same<T, float>::value) return "%.3f";
		else return "%d";
	}
};