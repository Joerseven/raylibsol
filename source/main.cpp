#include <iostream>
#include <sol/sol.hpp>
#include <stdexcept>

#include "nook-bindings.hpp"
#include "raylib.h"

struct ball
{
    float x, y, z;
    float dy, dx;
    float radius;
    float r, g, b;
};

struct ball balls[5];

float calculateDistance(int x, int y, int i) {
    return sqrt(pow(balls[i].x-x,2) +
                pow(balls[i].y-y,2));
}

void initMetaballs(int width, int height)
{
    for (int i = 0; i < 5; i++)
    {
        struct ball b;
        b.x = rand() % width;
        b.y = rand() % height;
        b.dy = 20;
        b.dx = 20;
        b.radius = 40;

        unsigned int color = rand() % 4;
        switch (color)
        {
        case 0:
            b.r = 255;
            b.g = 0;
            b.b = 0;
            break;

        case 1:
            b.r = 0;
            b.g = 255;
            b.b = 0;
            break;

        case 2:
            b.r = 0;
            b.g = 0;
            b.b = 255;
            break;
        default:
            b.r = 255;
            b.g = 0;
            b.b = 255;
            break;
        }
        balls[i] = b;
    }
}

void updateMetballs() {
    for (int i=0; i<5; i++) {
        balls[i].x += balls[i].dx;
        balls[i].y += balls[i].dy;
        if ((balls[i].x > 800)||(balls[i].x < 0))
            balls[i].dx = -balls[i].dx;
        if ((balls[i].y > 450)||(balls[i].y < 0))
            balls[i].dy = -balls[i].dy;
    }
}

void drawMetaballs()
{
    for (int x = 0; x < 800; x++)
        for (int y = 0; y < 450; y++)
        {
            int R = 0;
            int G = 0;
            int B = 0;
            for (int i=0; i<5; i++) {
                R += (balls[i].r / calculateDistance(x,y,i)*10);
                G += (balls[i].g / calculateDistance(x,y,i)*10);
                B += (balls[i].b / calculateDistance(x,y,i)*10);
            }
            if (R>255) R=255;
            if (G>255) G=255;
            if (B>255) B=255;

            DrawPixel(x,y,(Color){(unsigned char)R, (unsigned char)G, (unsigned char)B, 255});
        }
}

int main()
{
    sol::state lua;

    lua.open_libraries(
        sol::lib::base,
        sol::lib::package,
        sol::lib::string,
        sol::lib::math,
        sol::lib::table);

    nook::createBindings(lua);

    auto result = lua.safe_script_file("main.lua", sol::script_pass_on_error);
    if (!result.valid())
    {
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

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    initMetaballs(screenWidth, screenHeight);

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        float deltaTime = GetFrameTime();

        // Update
        //----------------------------------------------------------------------------------
        updateMetballs();
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawFPS(20,20);
        drawMetaballs();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}