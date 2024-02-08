#ifndef ELEMENT2_H
#define ELEMENT2_H
#include <iostream>
#include "abstract-element.h"

namespace DigiPen {
  class Element2 : public AbstractElement{
    public:
      Element2(int _val);
      Element2(const Element2& rhs);
      Element2& operator=(const Element2& rhs);
      int Get() const;
      void Set(int new_val);
      void Print() const;
      static int GetAlive();
      static int GetTotal();
      int GetID() const;
      ~Element2();
  private:
    static int _total;
    static int _alive;
    int* p_val;
  };
}
#endif
