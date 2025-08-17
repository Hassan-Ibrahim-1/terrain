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

    const imgui_dep = b.dependency("cimgui", .{
        .target = target,
        .optimize = optimize,
    });

    const imgui_config = cimgui.getConfig();
    const imgui_lib = imgui_dep.artifact(imgui_config.clib_name);
    exe.linkLibrary(imgui_lib);

    exe.addIncludePath(imgui_dep.path(imgui_config.include_dir));
    exe.addIncludePath(imgui_dep.path(imgui_config.backend_include_dir));
    exe.addIncludePath(b.path("dependencies/stb"));
    exe.addIncludePath(b.path("dependencies/noise"));
    exe.addIncludePath(b.path("dependencies/glad/include"));

    exe.linkSystemLibrary("assimp");
    exe.linkSystemLibrary("jpeg");
    exe.linkSystemLibrary("glfw");

    const cfiles: []const []const u8 = &.{
        "src/app.cpp",
        "src/camera.cpp",
        "src/circle.cpp",
        "src/color.cpp",
        "src/debug.cpp",
        "src/draw_command.cpp",
        "src/engine.cpp",
        "src/framebuffer.cpp",
        "src/fs.cpp",
        "src/game_object.cpp",
        "src/grid.cpp",
        "src/input.cpp",
        "src/light.cpp",
        "src/line.cpp",
        "src/main.cpp",
        "src/terrain.cpp",
        "src/mesh.cpp",
        "src/model.cpp",
        "src/point.cpp",
        "src/renderer.cpp",
        "src/scene.cpp",
        "src/shader.cpp",
        "src/skybox.cpp",
        "src/texture2d.cpp",
        "src/transform.cpp",
        "src/utils.cpp",
        "src/vertex.cpp",
        "src/window.cpp",

        "dependencies/glad/src/glad.c",
    };

    const flags: []const []const u8 = switch (optimize) {
        .Debug => &.{"-g"},
        .ReleaseSafe, .ReleaseFast, .ReleaseSmall => &.{
            "-O3",
        },
    };

    exe.addCSourceFiles(.{ .files = cfiles, .flags = flags });

    exe.addCSourceFile(.{ .file = imgui_dep.path(cimgui.glfwBackendSource()) });
    exe.addCSourceFile(.{ .file = imgui_dep.path(cimgui.openGlBackendSource()) });

    exe.linkLibC();
    exe.linkLibCpp();

    b.installArtifact(exe);

    const run_app = b.addRunArtifact(exe);
    const run_app_step = b.step("run", "Run the app module");
    run_app_step.dependOn(&run_app.step);
}
