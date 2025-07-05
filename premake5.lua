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
        sol = "%{wks.location}/ThirdParty/sol2/include",
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

    group ""
    project "Engine"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        characterset "ASCII"

        files {
            "Source/**.cpp","Source/**.h"
        }

        includedirs {
            "Source",
            "%{IncludeDir.glad}",
            "%{IncludeDir.glfw}",
            "%{IncludeDir.stb}",
            "%{IncludeDir.lua}",
            "%{IncludeDir.sol}",
            "%{IncludeDir.imgui}",
            "%{IncludeDir.assimp}",
        }
        
        links {
            "glad",
            "glfw",
            "lua",
            "imgui",
            "assimp",
        }

        filter  "configurations:Debug"
            runtime "Debug"
            symbols "on"

        filter  "configurations:Release"
            runtime "Release"
            optimize "on"

