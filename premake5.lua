workspace "MinoEngine"
    configurations { "Debug", "Release" }
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	targetdir ("bin/%{outputdir}/")
	objdir ("bin-int/%{outputdir}/obj/")
    startproject "Runtime"  
    architecture "x64"
    systemversion "latest"
    buildoptions { "/bigobj" }

    IncludeDir = {
        glad = "%{wks.location}/ThirdParty/glad/include",
        glfw = "%{wks.location}/ThirdParty/glfw/include",
        stb = "%{wks.location}/ThirdParty/stb",
        lua = "%{wks.location}/ThirdParty/lua",
        sol2 = "%{wks.location}/ThirdParty/sol2/include",
        imgui = "%{wks.location}/ThirdParty/imgui",
        assimp = "%{wks.location}/ThirdParty/assimp/include",
    }

    filter "configurations:Debug"
        defines { "DEBUG", "_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    group "ThirdParty"
        include "ThirdParty/glad"
        include "ThirdParty/glfw"
        include "ThirdParty/lua"
        include "ThirdParty/imgui"
        include "ThirdParty/assimp"
        include "ThirdParty/sol2"
        include "ThirdParty/stb"

    group "Main"
        include "Engine"
        include "Editor"
        include "Runtime"
    