#include <iostream>
#include <sol/sol.hpp>
#include <stdexcept>

#include "nook-bindings.hpp"
#include "raylib.h"

int main() {
    sol::state lua;

    lua.open_libraries(
        sol::lib::base,
        sol::lib::package,
        sol::lib::string,
        sol::lib::math,
        sol::lib::table);

    nook::createBindings(lua);

    auto result = lua.safe_script_file("main.lua", sol::script_pass_on_error);
    if (!result.valid()) {
        sol::error err = result;
        std::cerr << "The script was unable to run." << std::endl
                  << err.what() << std::endl;
        return 1;
    }

    sol::function luainit = lua["nook"]["Init"];
    sol::protected_function luadraw = lua["nook"]["Draw"];
    sol::function luaupdate = lua["nook"]["Update"];

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Game go brr!");

    SetTargetFPS(60);  // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    luainit();
    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
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
        DrawFPS(20,20);
        auto valid = luadraw();
        if (!valid.valid()) {
            sol::error err = valid;
		    std::string what = err.what();
		    std::cout << what << std::endl;
        }
        

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}