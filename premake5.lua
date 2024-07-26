workspace "Convolutor"
    architecture "x64"
    startproject "main"

    configurations {
        "Debug",
        "Release",
        "ReleaseSIMD",
        "ReleaseCSIMD"
    }

    filter "configurations:ReleaseCSIMD"
        defines { "NDEBUG", "CSIMD" }
        buildoptions { "-fopenmp", "-mavx2", "-O3" }
        linkoptions { "-fopenmp" }

    filter "configurations:ReleaseSIMD"
        defines { "NDEBUG", "SIMD" }
        buildoptions { "-fopenmp", "-mavx2", "-O3" }
        linkoptions { "-fopenmp" }

    filter "configurations:Release"
        defines { "NDEBUG", "NOSIMD" }
        buildoptions { "-fopenmp"}
        linkoptions { "-fopenmp" }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    project "conv_lib"
        location "conv_lib"
        kind "SharedLib"
        language "C++"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("obj/" .. outputdir .. "/%{prj.name}")

        files {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
        }

        includedirs {
            "%{prj.name}/src"
        }

        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        prebuildcommands {
            ("{MKDIR} ../bin/" .. outputdir .. "/main")
        }

        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/main")
        }

        -- Apply conditional compiler definitions
        filter "configurations:Debug"
            defines { "DEBUG", "LOGGING" }
            symbols "On"

        filter "configurations:Release"
            defines { "NDEBUG", NOSIMD }
            buildoptions { "-fopenmp", "-O2" }
            linkoptions { "-fopenmp" }

        filter "configurations:ReleaseSIMD"
            defines { "NDEBUG", "SIMD" }
            buildoptions { "-fopenmp", "-mavx2", "-O3" }
            linkoptions { "-fopenmp" }
        filter "configurations:ReleaseCSIMD"
            defines { "NDEBUG", "CSIMD" }
            buildoptions { "-fopenmp", "-mavx2" }
            linkoptions { "-fopenmp" }

    project "main"
        location "main"
        kind "ConsoleApp"
        language "C++"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("obj/" .. outputdir .. "/%{prj.name}")

        files {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
        }

        includedirs {
            "conv_lib/src",
            "%{prj.name}/src"
        }

        links {
            "conv_lib"
        }

        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        -- Apply conditional compiler definitions
        filter "configurations:Debug"
            defines { "DEBUG", "LOGGING" }
            symbols "On"

        filter "configurations:Release"
            defines { "NDEBUG", "NOSIMD" }
            buildoptions { "-fopenmp" }
            linkoptions { "-fopenmp" }

        filter "configurations:ReleaseSIMD"
            defines { "NDEBUG", "SIMD" }
            buildoptions { "-fopenmp", "-mavx2", "-O3" }
            linkoptions { "-fopenmp" }

        filter "configurations:ReleaseCSIMD"
            defines { "NDEBUG", "CSIMD" }
            buildoptions { "-fopenmp", "-mavx2" }
            linkoptions { "-fopenmp" }
