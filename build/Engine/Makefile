# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

SHELLTYPE := posix
ifeq ($(shell echo "test"), "test")
	SHELLTYPE := msdos
endif

# Configurations
# #############################################

ifeq ($(origin CC), default)
  CC = clang
endif
ifeq ($(origin CXX), default)
  CXX = clang++
endif
ifeq ($(origin AR), default)
  AR = ar
endif
RESCOMP = windres
INCLUDES += -I../../dependencies/assimp/include -I../../dependencies/libjpeg -I../../dependencies/noise -I../../dependencies/glad/include -I../../dependencies/stb -I../../dependencies/glfw/include -I../../dependencies/glm -I../../dependencies/imgui -I../../dependencies/imgui/backends
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LIBS += -lassimp -lglfw -ljpeg
LDDEPS +=
ALL_LDFLAGS += $(LDFLAGS) -L../../dependencies/assimp/bin -L../../dependencies/glfw/lib -L../../dependencies/libjpeg -framework IOKit -framework CoreFoundation -rpath @executable_path/../../dependencies/assimp/bin/ -rpath @executable_path/../../dependencies/libjpeg/
LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),debug)
TARGETDIR = ../../bin/Debug
TARGET = $(TARGETDIR)/Engine
OBJDIR = obj/Debug
DEFINES += -DDEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -g -Wall -Wextra
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -g -Wall -Wextra -std=c++17

else ifeq ($(config),release)
TARGETDIR = ../../bin/Release
TARGET = $(TARGETDIR)/Engine
OBJDIR = obj/Release
DEFINES += -DNDEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -O2 -Wall -Wextra
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -O2 -Wall -Wextra -std=c++17

endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/app.o
GENERATED += $(OBJDIR)/camera.o
GENERATED += $(OBJDIR)/circle.o
GENERATED += $(OBJDIR)/color.o
GENERATED += $(OBJDIR)/debug.o
GENERATED += $(OBJDIR)/draw_command.o
GENERATED += $(OBJDIR)/engine.o
GENERATED += $(OBJDIR)/framebuffer.o
GENERATED += $(OBJDIR)/fs.o
GENERATED += $(OBJDIR)/game_object.o
GENERATED += $(OBJDIR)/glad.o
GENERATED += $(OBJDIR)/grid.o
GENERATED += $(OBJDIR)/imgui.o
GENERATED += $(OBJDIR)/imgui_demo.o
GENERATED += $(OBJDIR)/imgui_draw.o
GENERATED += $(OBJDIR)/imgui_impl_glfw.o
GENERATED += $(OBJDIR)/imgui_impl_opengl3.o
GENERATED += $(OBJDIR)/imgui_tables.o
GENERATED += $(OBJDIR)/imgui_widgets.o
GENERATED += $(OBJDIR)/input.o
GENERATED += $(OBJDIR)/light.o
GENERATED += $(OBJDIR)/line.o
GENERATED += $(OBJDIR)/main.o
GENERATED += $(OBJDIR)/mesh.o
GENERATED += $(OBJDIR)/model.o
GENERATED += $(OBJDIR)/point.o
GENERATED += $(OBJDIR)/renderer.o
GENERATED += $(OBJDIR)/scene.o
GENERATED += $(OBJDIR)/shader.o
GENERATED += $(OBJDIR)/skybox.o
GENERATED += $(OBJDIR)/terrain.o
GENERATED += $(OBJDIR)/test.o
GENERATED += $(OBJDIR)/texture2d.o
GENERATED += $(OBJDIR)/transform.o
GENERATED += $(OBJDIR)/utils.o
GENERATED += $(OBJDIR)/vertex.o
GENERATED += $(OBJDIR)/window.o
OBJECTS += $(OBJDIR)/app.o
OBJECTS += $(OBJDIR)/camera.o
OBJECTS += $(OBJDIR)/circle.o
OBJECTS += $(OBJDIR)/color.o
OBJECTS += $(OBJDIR)/debug.o
OBJECTS += $(OBJDIR)/draw_command.o
OBJECTS += $(OBJDIR)/engine.o
OBJECTS += $(OBJDIR)/framebuffer.o
OBJECTS += $(OBJDIR)/fs.o
OBJECTS += $(OBJDIR)/game_object.o
OBJECTS += $(OBJDIR)/glad.o
OBJECTS += $(OBJDIR)/grid.o
OBJECTS += $(OBJDIR)/imgui.o
OBJECTS += $(OBJDIR)/imgui_demo.o
OBJECTS += $(OBJDIR)/imgui_draw.o
OBJECTS += $(OBJDIR)/imgui_impl_glfw.o
OBJECTS += $(OBJDIR)/imgui_impl_opengl3.o
OBJECTS += $(OBJDIR)/imgui_tables.o
OBJECTS += $(OBJDIR)/imgui_widgets.o
OBJECTS += $(OBJDIR)/input.o
OBJECTS += $(OBJDIR)/light.o
OBJECTS += $(OBJDIR)/line.o
OBJECTS += $(OBJDIR)/main.o
OBJECTS += $(OBJDIR)/mesh.o
OBJECTS += $(OBJDIR)/model.o
OBJECTS += $(OBJDIR)/point.o
OBJECTS += $(OBJDIR)/renderer.o
OBJECTS += $(OBJDIR)/scene.o
OBJECTS += $(OBJDIR)/shader.o
OBJECTS += $(OBJDIR)/skybox.o
OBJECTS += $(OBJDIR)/terrain.o
OBJECTS += $(OBJDIR)/test.o
OBJECTS += $(OBJDIR)/texture2d.o
OBJECTS += $(OBJDIR)/transform.o
OBJECTS += $(OBJDIR)/utils.o
OBJECTS += $(OBJDIR)/vertex.o
OBJECTS += $(OBJDIR)/window.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking Engine
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning Engine
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(GENERATED)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(GENERATED)) del /s /q $(subst /,\\,$(GENERATED))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild: | $(OBJDIR)
	$(PREBUILDCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) | $(PCH_PLACEHOLDER)
$(GCH): $(PCH) | prebuild
	@echo $(notdir $<)
	$(SILENT) $(CXX) -x c++-header $(ALL_CXXFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
$(PCH_PLACEHOLDER): $(GCH) | $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) touch "$@"
else
	$(SILENT) echo $null >> "$@"
endif
else
$(OBJECTS): | prebuild
endif


# File Rules
# #############################################

$(OBJDIR)/glad.o: ../../dependencies/glad/src/glad.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imgui_impl_glfw.o: ../../dependencies/imgui/backends/imgui_impl_glfw.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imgui_impl_opengl3.o: ../../dependencies/imgui/backends/imgui_impl_opengl3.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imgui.o: ../../dependencies/imgui/imgui.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imgui_demo.o: ../../dependencies/imgui/imgui_demo.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imgui_draw.o: ../../dependencies/imgui/imgui_draw.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imgui_tables.o: ../../dependencies/imgui/imgui_tables.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imgui_widgets.o: ../../dependencies/imgui/imgui_widgets.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/test.o: ../../dependencies/imgui/test.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/app.o: ../../src/app.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/camera.o: ../../src/camera.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/circle.o: ../../src/circle.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/color.o: ../../src/color.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/debug.o: ../../src/debug.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/draw_command.o: ../../src/draw_command.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/engine.o: ../../src/engine.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/framebuffer.o: ../../src/framebuffer.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/fs.o: ../../src/fs.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/game_object.o: ../../src/game_object.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/grid.o: ../../src/grid.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/input.o: ../../src/input.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/light.o: ../../src/light.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/line.o: ../../src/line.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/main.o: ../../src/main.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/mesh.o: ../../src/mesh.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/model.o: ../../src/model.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/point.o: ../../src/point.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/renderer.o: ../../src/renderer.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/scene.o: ../../src/scene.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/shader.o: ../../src/shader.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/skybox.o: ../../src/skybox.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/terrain.o: ../../src/terrain.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/texture2d.o: ../../src/texture2d.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/transform.o: ../../src/transform.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/utils.o: ../../src/utils.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/vertex.o: ../../src/vertex.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/window.o: ../../src/window.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif