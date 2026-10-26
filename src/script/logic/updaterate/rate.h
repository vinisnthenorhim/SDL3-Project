#pragma once

struct TimeRate
{
  float acumulate = 0;
  float interval;

  TimeRate(float rate)
  : interval(1 / rate)
  {}

  int tick(float deltaTime)
  {
    acumulate += deltaTime;
    int count = 0;
    if (acumulate >= interval)
    {
      while (acumulate > 0)
      {
        count++;
        acumulate -= deltaTime;
      }
    }
    return count;
  }
};