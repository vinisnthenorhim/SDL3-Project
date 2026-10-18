
#include "LuaRuntime.h"
#include <filesystem>
#include <iostream>

LuaRuntime::LuaRuntime()
{
    luaState.open_libraries(
      sol::lib::base,
      sol::lib::math,
      sol::lib::os
    );
}

#include "LuaRuntime.h"


void LuaRuntime::loadScript(const std::string& path)
{
    if (!std::filesystem::exists(path))
    {
        std::cerr << "Lua script does not exist: "
                  << path << '\n';
        return;
    }

    sol::protected_function_result result =
        luaState.safe_script_file(path);

    if (!result.valid())
    {
        sol::error error = result;
        std::cerr << "Lua error: " << error.what() << '\n';
    }
}


sol::protected_function LuaRuntime::lua_function(const std::string& functionName)
{
    sol::protected_function function = luaState[functionName];

    if (!function.valid())
    {
        std::cerr << "Lua function not found: "
                  << functionName << '\n';
        return sol::protected_function{};
    }

    return function;
}