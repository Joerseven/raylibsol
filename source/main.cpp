#include <iostream>
#include <sol/sol.hpp>

#include "raylib.h"

#include "nook-bindings.hpp"

int main()
{
    sol::state lua;

    lua.open_libraries(
        sol::lib::base,
        sol::lib::package,
        sol::lib::string,
        sol::lib::math,
        sol::lib::table);

    createBindings(lua);

    auto result = lua.safe_script_file("main.lua", sol::script_pass_on_error);
    if (!result.valid()) {
        sol::error err = result;
        std::cerr << "The script was unable to run." << std::endl << err.what() << std::endl;
        return 1;
    }

    sol::function luadraw = lua["Draw"];
    sol::function luaupdate = lua["Update"];

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        float deltaTime = GetFrameTime();

        // Update
        //----------------------------------------------------------------------------------
        luaupdate(deltaTime);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawText("C Window", 190, 200, 20, BLACK);
            luadraw();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}