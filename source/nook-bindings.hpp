#ifndef NOOK_BINDINGS
#define NOOK_BINDINGS

#include "raylib.h"
#include "sol/sol.hpp"

void bindFunctions(sol::state& lua) {
    lua.set_function("DrawText", DrawText);
}

void createBindings(sol::state& lua) {

    lua["BLACK"] = Color(BLACK);

    bindFunctions(lua);
}

#endif