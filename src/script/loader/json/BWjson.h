#pragma once

#include <fstream>

namespace BW
{
  struct File
  {
    FILE* fp = nullptr;
    File(const char* path, const char* mode) 
    : fp(fopen(path, mode)) 
    {}
    ~File() 
    { 
      if(!fp) return;
      fclose(fp); 
      fp = nullptr;
    }
  };
}