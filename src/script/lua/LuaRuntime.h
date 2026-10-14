
#pragma once

#include <sol/sol.hpp>
#include <string>

class LuaRuntime
{
public:
    LuaRuntime();

    void loadScript(const std::string& path);

    sol::state& lua() {return luaState;}
    sol::protected_function lua_function(const std::string& functionName);
    template<typename Func>
    void registerFunction(const std::string& name, Func&& function)
    {
        luaState.set_function(name, std::forward<Func>(function));
    }
private:
    sol::state luaState;
};
