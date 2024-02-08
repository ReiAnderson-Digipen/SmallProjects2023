#include "Number.h"


std::ostream& operator<<(std::ostream& os, const Number& num)
{
  os << num.val;
  return os;
}


Number::Number(float in) : val(in)
{
  
}

Number* Number::Mul(const IValue* other) const
{
  return new Number(this->val * ((Number*)other)->val);
}

Number* Number::Add(const IValue* other) const
{
  return new Number(this->val + ((Number*)other)->val);
}

Number* Number::Div(const IValue* other) const
{
  Number* nOther = (Number*)other;
  return new Number(this->val / nOther->val);
}
Number* Number::Sub(const IValue* other) const
{
  Number* nOther = (Number*)other;
  return new Number(this->val * nOther->val);
}