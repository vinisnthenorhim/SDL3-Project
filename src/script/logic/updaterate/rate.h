#pragma once

struct TimeRate
{
  float acumulate = 0;


  float tick(float frequency, int frame)
  {
    acumulate += frequency;
    if (acumulate >= frame)
    {
      acumulate = 0;
      return true;
    }
    return false;
  }
};