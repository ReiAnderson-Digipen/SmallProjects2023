#include <vector>
class IValue {
public:
  
  virtual IValue* Mul(const IValue* other) const = 0;
  virtual IValue* Div(const IValue* other) const = 0;
  virtual IValue* Add(const IValue* other) const = 0;
  virtual IValue* Sub(const IValue* other) const = 0;
  //virtual IValue* Call(const std::vector<IValue*> args);
  
};