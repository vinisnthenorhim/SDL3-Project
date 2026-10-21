#pragma once

#include <iostream>
#include <sol/sol.hpp>
#include <memory>


namespace BW
{
  inline void stateLoader(  sol::state& luaState, const char* path)
  {

    luaState.open_libraries(
      sol::lib::base,
      sol::lib::math,
      sol::lib::os
    );

    sol::protected_function_result result = luaState.safe_script_file(path);

    if (!result.valid())
    {
        sol::error error = result;
        std::cerr << "Lua error: " << error.what() << '\n';
    }
  }
}