local depDir = "dependencies/"

IncludeDir = {}
IncludeDir["assimp"] = depDir .. "assimp/include/"
IncludeDir["libjpeg"] = depDir .. "libjpeg/"
IncludeDir["noise"] = depDir .. "noise/"
IncludeDir["glad"] = depDir .. "glad/include/"
IncludeDir["stb"] = depDir .. "stb/"
IncludeDir["glfw"] = depDir .. "glfw/include/"
IncludeDir["glm"] = depDir .. "glm/"
IncludeDir["imgui"] = depDir .. "imgui/"
IncludeDir["imgui-backends"] = depDir .. "imgui/backends"

LibDir = {}
LibDir["assimp"] = depDir .. "assimp/bin/"
LibDir["glfw"] = depDir .. "glfw/lib/"
LibDir["libjpeg"] = depDir .. "libjpeg/"

Libs = {}
Libs["assimp"] = "assimp"
Libs["glfw"] = "glfw"
Libs["libjpeg"] = "jpeg"

-- When compiling from source here
Sources = {}
Sources["imgui"] = depDir .. "imgui/*.cpp"
Sources["imgui-backends"] = depDir .. "imgui/backends/*.cpp"
Sources["glad"] = depDir .. "glad/src/*.c"

