#ifndef ABSTRACT_ELEM_H
#define ABSTRACT_ELEM_H

namespace DigiPen {
  class AbstractElement {
  public:
    virtual ~AbstractElement(){};
    virtual void Print() const = 0;
    virtual int Get() const = 0;
    virtual int GetID() const = 0;

  private:
  };

}


#endif