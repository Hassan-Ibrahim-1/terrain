const std = @import("std");
const cimgui = @import("cimgui");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "terrain-gen",
        .optimize = optimize,
        .target = target,
    });

    const cfiles: []const []const u8 = &.{
        "src/app.cpp",
        "src/app.hpp",
        "src/camera.cpp",
        "src/camera.hpp",
        "src/circle.cpp",
        "src/circle.hpp",
        "src/color.cpp",
        "src/color.hpp",
        "src/common.hpp",
        "src/cube.hpp",
        "src/debug.cpp",
        "src/debug.hpp",
        "src/draw_command.cpp",
        "src/draw_command.hpp",
        "src/engine.cpp",
        "src/engine.hpp",
        "src/framebuffer.cpp",
        "src/framebuffer.hpp",
        "src/fs.cpp",
        "src/fs.hpp",
        "src/game_object.cpp",
        "src/game_object.hpp",
        "src/grid.cpp",
        "src/grid.hpp",
        "src/input.cpp",
        "src/input.hpp",
        "src/keycodes.hpp",
        "src/light.cpp",
        "src/light.hpp",
        "src/line.cpp",
        "src/line.hpp",
        "src/main.cpp",
        "src/material.hpp",
        "src/mesh.cpp",
        "src/mesh.hpp",
        "src/model.cpp",
        "src/model.hpp",
        "src/point.cpp",
        "src/point.hpp",
        "src/rect.hpp",
        "src/renderer.cpp",
        "src/renderer.hpp",
        "src/scene.cpp",
        "src/scene.hpp",
        "src/shader.cpp",
        "src/shader.hpp",
        "src/skybox.cpp",
        "src/skybox.hpp",
        "src/sphere.hpp",
        "src/terrain.cpp",
        "src/terrain.hpp",
        "src/texture2d.cpp",
        "src/texture2d.hpp",
        "src/transform.cpp",
        "src/transform.hpp",
        "src/utils.cpp",
        "src/utils.hpp",
        "src/vertex.cpp",
        "src/vertex.hpp",
        "src/window.cpp",
        "src/window.hpp",

        "dependencies/glad/src/glad.c",
    };

    const flags: []const []const u8 = switch (optimize) {
        .Debug => &.{"-g"},
        .ReleaseSafe, .ReleaseFast, .ReleaseSmall => &.{
            "-O3",
        },
    };

    exe.addCSourceFiles(.{ .files = cfiles, .flags = flags });

    exe.linkLibC();
    exe.linkLibCpp();

    exe.linkSystemLibrary("glfw");

    const imgui_dep = b.dependency("cimgui", .{
        .target = target,
        .optimize = optimize,
    });

    const imgui_config = cimgui.getConfig();
    const imgui_lib = imgui_dep.artifact(imgui_config.clib_name);
    exe.linkLibrary(imgui_lib);
    exe.addIncludePath(imgui_dep.path(imgui_config.include_dir));

    exe.addIncludePath(b.path("dependencies/stb"));
    exe.addIncludePath(b.path("dependencies/noise"));
    exe.addIncludePath(b.path("dependencies/glad/include"));

    b.installArtifact(exe);

    const run_app = b.addSystemCommand(&.{ "zig", "build", "run" });
    const run_app_step = b.step("run", "Run the app module");
    run_app_step.dependOn(&run_app.step);
}

