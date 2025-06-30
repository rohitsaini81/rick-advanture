#ifndef SCRIPT_H
#define SCRIPT_H

extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
  }
#include "raylib.h"


#pragma once

void run_lua_script(const char* filename);

void load_config(const char* filename);
time_t getFileLastModifiedTime(const std::string& path);

#endif
