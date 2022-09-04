#ifndef NOOK_BINDINGS
#define NOOK_BINDINGS

#include "raylib.h"
#include "sol/sol.hpp"

namespace nook {

struct Vector2 {
    float x;
    float y;

    Vector2() : x(0), y(0) {}

    Vector2(float x, float y): x(x), y(y) {}

    Vector2(::Vector2 vec): x(vec.x), y(vec.y) {}

    Vector2 operator+(const Vector2& right) const {
        return Vector2(x + right.x, y + right.y);
    }

    Vector2 operator-(const Vector2& right) const {
        return Vector2(x - right.x, y - right.y);
    }

};

struct Color {
    ::Color color;

    Color(int r, int g, int b, int a): color(::Color{(unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a}) {}

};

void bindUserTypes(sol::state &lua) {
    sol::usertype<Color> color_type = lua.new_usertype<Color>("Color",
    sol::constructors<Color(int, int, int ,int)>());

    color_type["color"] = &Color::color;
}

void bindFunctions(sol::state &lua) {
    lua.set_function("DrawText", DrawText);
    lua.set_function("DrawPixel", DrawPixel);
    lua.set_function("GetScreenWidth", GetScreenWidth);
    lua.set_function("GetScreenHeight", GetScreenHeight);
}

void createBindings(sol::state &lua) {
    lua["nook"] = sol::new_table();

    lua["BLACK"] = Color(BLACK);

    bindUserTypes(lua);

    bindFunctions(lua);
}

}  // namespace nook

#endif