project "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    characterset "ASCII"
    debugdir "%{wks.location}"

    files {
        "Source/**.cpp","Source/**.h"
    }

    postbuildcommands {
        "xcopy \"%{wks.location}\\bin\\%{outputdir}\\Editor.exe\" \"%{wks.location}\" /y /i /c",
    }

    includedirs {
        "%{wks.location}/Engine/Source",

        "%{IncludeDir.glad}",
        "%{IncludeDir.lua}",
        "%{IncludeDir.sol2}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.tinyxml2}",
    }
    
    links {
        "Engine",
    }

