cmake_minimum_required(VERSION 3.16)
project(lua LANGUAGES C)

file(GLOB LUA_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
file(GLOB LUA_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

# Define executables
set(LUA_EXECUTABLE lua)
set(LUAC_EXECUTABLE luac)

# Separate main files
set(LUA_MAIN src/lua.c)
set(LUAC_MAIN src/luac.c)
list(REMOVE_ITEM LUA_SOURCES ${LUA_MAIN} ${LUAC_MAIN})

# Static and shared libraries
add_library(lua_static STATIC ${LUA_SOURCES})
target_compile_features(lua_static PRIVATE c_std_99)

# Optionally: set include path
target_include_directories(lua_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

