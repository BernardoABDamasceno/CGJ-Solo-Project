#ifndef MGL_IDRAWABLE_HPP
#define MGL_IDRAWABLE_HPP

namespace mgl{

class IDrawable;


class IDrawable {
 public:
  virtual void draw(void) = 0;
};

}
#endif