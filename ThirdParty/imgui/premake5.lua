project "imgui"
    location "imgui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "Off"

	files
	{
		"imconfig.h",
		"imgui.h",
		"imgui_internal.h",
		"imstb_rectpack.h",
		"imstb_textedit.h",
		"imstb_truetype.h",
		"backends/imgui_impl_glfw.h",
		"backends/imgui_impl_opengl3.h",
		"backends/imgui_impl_opengl3_loader.h",

		"imgui.cpp",
		"imgui_draw.cpp",
		"imgui_widgets.cpp",
		"imgui_tables.cpp",
		"imgui_demo.cpp",
        "backends/imgui_impl_glfw.cpp",
		"backends/imgui_impl_opengl3.cpp",
	}

	includedirs
	{
		"./",
		"backends",
        "%{IncludeDir.glfw}",
	}

	links
	{
		"glfw",
	}