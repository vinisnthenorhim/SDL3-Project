#pragma once
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <stb/stb_image.h>
#include <iostream>

namespace BW
{
  struct Image
  {
    unsigned char* image = nullptr;
    int w = 0;
    int h = 0;
    int ch = 0;
    ~Image()
    {
      stbi_image_free(image);
      image = nullptr;
    }

  };
  Image imageLoader(const char* path, uint32_t channel)
  {
    int width, height, ch;

    unsigned char* imgLoad = stbi_load(path, &width, &height, &ch, channel);
    if (!imgLoad)
    {
      std::cerr << path << " doesn't exist\n";
    }
    bool hasAlpha = false;
    for (int i = 0; i < width * height; i++) {
        if (imgLoad[i * 4 + 3] < 255) { hasAlpha = true; break; }
    }
    if (ch < 4 || !hasAlpha)
    {
      std::cout << '[' << path << ']' << '\n';
      std::cout << "file channels = " << ch << '\n'; 
      std::cout << "has transparent pixels: " << hasAlpha << '\n';
    }
    return {imgLoad, width, height, ch};
  }

}
