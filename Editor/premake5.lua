project "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    characterset "ASCII"
    debugdir "%{wks.location}"

    files {
        "Source/**.cpp","Source/**.h"
    }

    includedirs {
        "%{wks.location}/Engine/Source",

        "%{IncludeDir.glad}",
        "%{IncludeDir.lua}",
        "%{IncludeDir.sol2}",
        "%{IncludeDir.imgui}",
    }
    
    links {
        "Engine",
    }

