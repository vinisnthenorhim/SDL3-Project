#pragma once
#include <iostream>
#include <fstream>

namespace BW
{
  struct File
  {
    FILE* fp = nullptr;
    File(const char* path, const char* mode) 
    : fp(fopen(path, mode)) 
    {
      if (!fp)
      {
        std::cerr << "Failed to open: " << path << '\n';
      }
    }
    ~File() 
    { 
      if(!fp) return;
      fclose(fp); 
      fp = nullptr;
    }
  };
}