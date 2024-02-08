#ifndef ELEMENT1_H
#define ELEMENT1_H
#include <iostream>
#include "abstract-element.h"

namespace DigiPen {
  class Element1 : public AbstractElement{
    public:
    
      Element1(int _val);
      ~Element1();
      int Get() const;
      void Set(int new_val);
      void Print() const;
      static int GetAlive();
      static int GetTotal();
      int GetID() const;
  private:
    static int _total;
    static int _alive;
    int val;
  };
}
#endif
