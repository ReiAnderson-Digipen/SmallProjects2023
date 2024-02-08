#include <string>
#include <ostream>

class IValue;

enum OPERATOR_ENUM { PARA_OPEN, PARA_CLOSE, MUL, DIV, ADD, SUB };

class Operator {
public:
  Operator(std::string symbol, int precedence, OPERATOR_ENUM oEnum);

  const std::string& GetSymbol() const;
  int GetPrecedence() const;
  OPERATOR_ENUM GetEnum() const;
  friend std::ostream& operator<<(std::ostream& os, const Operator& op);

private:
  std::string op_symbol;
  int precedence;
  OPERATOR_ENUM oEnum;
};

IValue* evaluate(std::string expression);