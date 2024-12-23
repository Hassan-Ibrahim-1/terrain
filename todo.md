# Terrain gen

-- Get rid of bounds and use terrain.gobj.transform
-- figure out why scale moves the terrain - offset it?

Water:
separate water shader
color ground blue in the first pass. have a second
framebuffer that runs the water shader that takes in the water boundary
draw water then. reflections would be nice. speculars from directional light
planar reflections?


rivers (meh water), trees
try bringing over grass code to this
shadows to make grass look better
First person traversal, very basic collision

better skybox (just blue sky is fine)

mountains, snow, clouds
# Winter Break stuff

 - SAT collision
 - Basic ECS demo
 - Restructure and organize engine

 -- DONE -- Move state variables related to rendering in Engine to Renderer
 -- DONE -- Implement depth_view_enabled checking in every render pass
 -- DONE -- Add a get_projection_matrix function to Camera
 -- DONE -- Add near and far values to Camera
 -- DONE -- Pass in camera to renderer to replace set_matrices and to provide near and far values

 -- DONE -- Flesh out Application
    ** DONE ** Add a renderer instance instead of having a Renderer in Engine
    ** DONE ** Add a camera instance instead of using global::camera
    ** DONE ** Add a window instance
    ** DONE ** delta time
    ** DONE ** create info, width, height, name

 -- DONE -- Get rid of ErrorHandler

 - Custom key callback function which input::add_key_callback uses - no glfw stuff
    * use custom stuff like KeyCode instead of ints and GLFW specific stuff
 - Move private members in namespaces to the source file

 -- DONE -- key_down, key_pressed, key_released in input_handler



# NEXT UP:

- In material instead of having different vectors for specular and diffuse
    textures just have one vector of textures and use TextureType instead
    to differentiate between them

 - What are those weird artificats in wireframe mode with lighting?
    When in wireframe - don't take lighting into account

  WORK ON THIS
 - Work on genproj
    * templates are just directory names in .genproj
    * base template with just a basic Makefile and src directory

 - A send texture data function that takes in a material and shader
    Make this function public. Make send_light_data public as well

 - LIGHTING
    ** DONE ** Lighting shaders - lighting_mesh & lighting_texture_mesh
    ** DONE ** Scene::lights. Renderer just uses that
    ** DONE ** Point, directional, and spotlights
    * visualizations for all lights - not just cubes
        DONE point lights - sphere.
        DONE spot light - pyramid
    * arrow for directional lights to show the direction
    ** DONE ** utils functions for lights
    ** DONE ** Scene::has_lights. Renderer uses this to determine whether to use basic_mesh or light_mesh

 -- DONE -- Lighting with textures

 - Fix model rendering
    ** DONE ** The backpack model isn't rendering because it has several meshes
    ** DONE ** Need to be able to load and render several meshes associated with a game object
        Maybe make GameObject::mesh a vector? that should fix it.
    * Figure out how to copy over mesh data without having to copy vertices and indices
        load_model function for GameObjects
    * Same problem with material. There can be several textures in a model. Need to figure out 
        how to deal with this as well.
    ** DONE ** Copy material stuff over as well (textures)
    * Add a Cube::get_mesh function that just returns meshes.back()

 -- DONE -- Spot lights
    ** DONE ** make pyramid rotate in the direction that the spot light is facing
    ** DONE ** figure out why the specular high light thing isn't moving when the camera moves
        flashlight type stuff
        only depends on the position vector?

 -- DONE -- Fix model
 - Add textures back to model

 -- DONE -- UNCAPPED FRAMERATE

 - RENDERER REFACTOR
    ** DONE ** Store pointers to objects instead of copies - call scene.add_primitive() which adds to the scene's
        list of pointers to primitives. Renderer then uses these pointers instead of keeping track of
        its own data (allows for hidden. etc)
    ** DONE ** Get rid of a bunch of shaders and replace them with generic stuff
    ** DONE ** DONT Figure out how to make primtives inherit from GameObject and still work the same way - minimal vaos
    * look at older code and fix it -> const refs
    ** DONE ** maybe don't clear arrays every frame?
    ** DONE ** get rid of textured_rects_vao and combine to just one vao with the texture coords
    ** DONE ** Fix Line rendering
 - get rid of texture type

 -- DONE -- make primitives inherit from gobj
    ** DONE ** get rid of stuff like render_cubes and just replace it with render_game_objects
    ** DONE ** move all primitives to the GameObject pipeline. Also figure out a way to access vaos from Scene
    ** DONE ** don't call create buffers on the mesh - instead just set a custom vao for that primitive
    ** DONE ** instead of having to use GL_TRIANGLES create your own enum
    ** DONE ** add a DrawCommand struct to mesh which renderer will use

 - Scene stuff
    * Get game object by name


 - Instead of having a bunch of shaders, just have a lighting.frag shader and a textured_lighting.frag shader
    * this would get rid of a lot of useless code and make lighting easier
    * If no shader is provided in the material just run the regular lighting shader
    * if there is a shader in material just use that
    * have more generic shaders already compiled in Renderer such as basic_color, basic_texture - stuff with no lighting
        maybe even stuff with transparency

 - Better way of creating meshes.
    - Be able to load a mesh using a model file - only load one mesh
    - Figure out primitives - keep them the same because of performance?
        but also make it so that user created GameObjects can be created using primitive templates
        or the other way around - primitives inherit from GameObject -- probably this

 -- DONE -- Make every primitive use / inherit from GameObject

 - Rendering the contents of a framebuffer using Renderer
    * just a rect with the screen_shader shader and the framebuffer texture

 -- DONE -- Material struct should have a Shader property
    ** DONE ** Update renderer to account for optional shader
 -- DONE -- GameObject class with a mesh, transform and update function
    ** DONE ** Update renderer to take in GameObjects
 -- DONE -- Have to separate passes in engine for drawing meshes and for calling user update code
     ** DONE ** Don't have the user call renderer.draw() each frame. Instead just add a away to just create a GameObject
     and the engine deals with drawing it. Same with primitives.

 -- DONE -- A Scene class which contains a bunch of GameObjects. Instead of calling Renderer::add_game_object
    just call Scene::create_game_object & Scene::add_game_object

 - Add a default configure window created by the engine
    ** DONE ** includes variables for global things like renderer and camera
    * reset state button which resets user and engine state
        (give Application a user defined reset state function)

-- DONE -- Add a main_camera in engine

- WARN macro in debug.hpp

 -- DONE -- Figure out a better way of doing delta time. Currently the user has to add a member
    variable themselves to an Application child


 -- DONE -- Use meshes in Renderer for primitives. should organize stuff more
    ** DONE ** primitives inherit from GameObject
    ** DONE ** figure out a better way of organizing mesh vaos and vbos

 ?- Fix rotation - looks super buggy

 - reset state button which resets user and engine state
    (give Application a user defined reset state function)

# TODO
 -- DONE - Create a logger that can be disabled
 -- DONE engine.init before creating BaseProgram instance
 -- DONE Make mock not allocate shaders on the heap
 -- DONE Refactor mock
 -- DONE Maybe have a program class that can be inherited from - then
    pass that class to engine instead of passing function pointers

 -- DONE Way too many heap allocations
 -- DONE - Engine class. engine.run() Engine(init, update, cleanup)
 -- DONE - function pointers to init, update, and cleanup (these functions would only be related to the program and nothing to do with the engine
    engine. ex: engine code would surround update in the main loop)
 -- DONE -  cleaner main function
 -- DONE - Figure out a better alternative for Settings

 - State struct instead of engine member variables 
    App & Config struct - look at filament sample code

 -- DONE -- Better error handling
 - better coordinate system

 -- WORK ON THIS -- 
 -- DONE -- Add an add_shader function to Renderer so that user shaders can be reloaded
 -- DONE --  Use structs in vbos in renderer instead of pushing indiviually to vao - 
 -- DONE -- Update 2d objects to work in 3d as well
 -- DONE -- Fix rect rendering
 -- DONE -- Cube rendering
 -- DONE -- render_cube function for lights that draws the lights as a cube - put this in utils instead
 -- DONE -- Being able to set basic stuff like window background color, cursor enabled, wireframe
 -- DONE -- Sphere rendering - just use blender

 -- DONE -- Fix specular maps on both models - highlights appearing in the back
 -- DONE -- Noise generation functions - being able to create a texture from noise and seeing that texture
 -- DONE --  Applying textures to a rect - basically visualizing any texture
 -- DONE -- Material struct
 - Texture / user assets directory
 - Applying textures to 2d and 3d objects (just port over Material)
    ** DONE ** Cubes
    ** DONE ** Spheres
    * Circles
 -- DONE -- Move delta time to a time class or namespace

 - optimize Renderer::update_vbos. create a bool that signals if a set of data has been updated
    if so update the buffer. reset the bool each frame.
 - Add consts where needed
 - Reserve some vector space if needed in Renderer?

 - functions in utils that add primitive rendering subjects to a window - what?
 - implement support for rendering bezier curves. use this to better the gravity simulator estimator
 - Camera orientation bug - doesn't really matter but try fixing it - what?
 -- DONE -- Support rotations in transform
 
 -- DONE -- Figure out how to do sphere rendering with lighting.
 - Figure out lighting for rendering primitives

 - 3D object base class. both cube and sphere are basically the same

 - Proper pathing - Filesystem that gets path to shaders, models, textures, without having to give a directory (provide these directories on init)


 -- DONE - Color class
 - Better organized buffers
 - add glfw and glm to dependencies
 - Make program code not have to interface with opengl or glfw at all (in case i wanna switch to something else later)
 - Base GameObject class (being able to add physics to some game objects and not to others)
 - Being able to edit object data during runtime and saving that to a file
 - Multiple Camera classes (orthographic and perspective)
 - Multiple cameras in runtime and being able to switch between them
 - Ability to close all imgui windows
 - GameObjects should have a bool to make them hidden (not render)
 - Main engine imgui window
 - Move away from a Globals namespace and prefer smaller more descriptive namespaces camera should be somewhere else
 - Modal runtime - Edit mode changes get saved - no physics / user update function
     Run mode changes don't get saved - physics & user update function gets called

 - aabb collision
 - Fixed update at 60fps for physics
 - Refactoring (figure this out)
 - Advanced lighting (move on to blin phong eventually)
 - post processing effects support (frame buffers)
 - Ability to add skyboxes
 - Transparency
 - Text rendering using a user specified font - Text class
 - ECS
 - Option to disable lighting and just use non lighting related shaders

