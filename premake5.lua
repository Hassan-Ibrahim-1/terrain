---@diagnostic disable: undefined-global

include "deps.lua"

local projectName = "Engine"
local config = "debug"

local function build()
    if not os.execute("premake5 gmake2") then
        os.exit(1)
    end
    if not os.execute("make config=" .. config .. " -C build -f Makefile") then
        os.exit(1)
    end
end

local function run()
    local targetDir = "bin/" .. config .. "/" .. projectName
    os.execute("./" .. targetDir)
end

workspace(projectName)
    configurations {"Debug", "Release"}
    location "build"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter {}

project(projectName)
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir "bin/%{cfg.buildcfg}"
    location ("build/" .. projectName)
    warnings "Extra"

    -- rpath {
    --     "@executable_path/dependencies/assimp", "@loader_path/dependencies/assimp",
    -- }

    files {
        "src/**.cpp",
        "src/**.hpp",
        Sources["imgui"],
        Sources["imgui-backends"],
        Sources["glad"],
    }

    linkoptions {
        "-framework IOKit",
        "-framework CoreFoundation",
        "-rpath @executable_path/../../dependencies/assimp/bin/",
        "-rpath @executable_path/../../dependencies/libjpeg/",
    }

    includedirs {
        IncludeDir["assimp"],
        IncludeDir["libjpeg"],
        IncludeDir["noise"],
        IncludeDir["glad"],
        IncludeDir["stb"],
        IncludeDir["glfw"],
        IncludeDir["glm"],
        IncludeDir["imgui"],
        IncludeDir["imgui-backends"],
    }

    libdirs {
        LibDir["assimp"],
        LibDir["glfw"],
        LibDir["libjpeg"],
    }

    links {
        Libs["assimp"],
        Libs["glfw"],
        Libs["libjpeg"],
    }

    newaction {
        trigger = "build",
        description = "build using the config variable set in premake5.lua",
        execute = build
    }

    newaction {
        trigger = "run",
        description = "Run the executable after having built it",
        execute = function ()
            build()
            run()
        end
    }

    newaction {
        trigger = "clean",
        description = "removes build directory",
        execute = function ()
            os.execute("rm -rf build")
            os.execute("rm -rf bin")
        end
    }

