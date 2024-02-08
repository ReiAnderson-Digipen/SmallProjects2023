
#include "IValue.h"
#include <ostream>

class Number : public IValue {
public:
  Number(float in);
  Number* Mul(const IValue* other) const override;
  Number* Div(const IValue* other) const override;
  Number* Add(const IValue* other) const override;
  Number* Sub(const IValue* other) const override;
  friend std::ostream& operator<<(std::ostream& os, const Number& num);
private:
  float val;
};
