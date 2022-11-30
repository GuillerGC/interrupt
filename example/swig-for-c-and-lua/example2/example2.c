#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "types.h"

// The SWIG external runtime is generated by using.
// swig -lua -external-runtime swig_runtime.h
#include "swig_runtime.h"

extern int luaopen_bindings(lua_State *L);

void callProcessStruct(lua_State *L, my_struct_t *my_struct)
{
    lua_getglobal(L, "processStruct");
    assert(lua_isfunction(L, -1));

    swig_type_info *type = SWIG_TypeQuery(L, "my_struct_t *");
    assert(type != NULL);
    SWIG_NewPointerObj(L, my_struct, type, false);

    // Call function with 1 arguments and no result
    assert(lua_pcall(L, 1, 0, 0) == LUA_OK);
}

int main(void)
{
    lua_State *L = luaL_newstate();
    assert(L != NULL);

    luaL_openlibs(L);
    luaopen_bindings(L); // Load the wrapped module

    int r = luaL_loadfile(L, "example2.lua");
    assert(r == LUA_OK);

    printf("[C] Loading script\r\n");

    // Load the script by calling it
    assert(lua_pcall(L, 0, 0, 0) == LUA_OK);

    my_struct_t my_struct;
    my_struct.isReady = true;
    my_struct.level = LEVEL_MEDIUM;
    strncpy(my_struct.message, "This is example message", sizeof(my_struct.message));
    my_struct.priority = 100;

    printf("[C] Calling processStruct\r\n");
    callProcessStruct(L, &my_struct);
    printf("[C] We're back from Lua\r\n");

    printf("[C] Printing my_struct\r\n");
    printf("\tmy_struct.level = %d\r\n", my_struct.level);
    printf("\tmy_struct.priority = %d\r\n", my_struct.priority);
    printf("\tmy_struct.message = %s\r\n", my_struct.message);
    printf("\tmy_struct.isReady = %s\r\n", my_struct.isReady ? "true" : "false");

    lua_close(L);
    printf("[C] Finished\r\n");
}
