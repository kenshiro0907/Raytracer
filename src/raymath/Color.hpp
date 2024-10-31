#pragma once

#include <iostream>
#include <cmath>

class  Color
{
private:
  float r = 0;
  float b = 0;
  float g = 0;
public:
  Color();
  Color(float r, float g, float b);
  ~ Color();

  float R() const { return r; };
  float G() const { return g; };
  float B() const { return b; };

  Color operator+(Color const& col) const;
  Color operator*(float col) const;
  Color& operator=(Color const& col);
  friend std::ostream & operator<<(std::ostream & _stream, Color const & col);
};

