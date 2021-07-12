workspace "pbr"
    architecture "x86_64"
    configurations {"Debug", "Release"}
    flags {"MultiProcessorCompile"}

project "pbr"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir("bin/")
    objdir("bin/obj/")

    files {"src/**.cc", "src/**.hh"}
    includedirs {"lib/include", "src"}

    libdirs {"lib/bin"}
    links {"spdlog", "pthread", "glfw"}

    defines { "SPDLOG_COMPILED_LIB" }

    filter "configurations:Debug"
        symbols "on"
        optimize "off"
    filter "configurations:Release"
        optimize "on"
        symbols "off"
        runtime "Release"