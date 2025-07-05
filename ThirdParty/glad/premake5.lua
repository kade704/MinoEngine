project("glad")
	kind "StaticLib"
	language "C"
	staticruntime "Off"

	files {
		"inlcude/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c",
	} 
	
	includedirs {
		"include"
	}