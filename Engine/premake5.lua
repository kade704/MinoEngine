project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    characterset "ASCII"

    files {
        "Source/**.cpp","Source/**.h"
    }

    includedirs {
        "%{IncludeDir.glad}",
        "%{IncludeDir.glfw}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.lua}",
        "%{IncludeDir.sol2}",
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
