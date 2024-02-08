#include "evaluation.h"
#include <vector>
#include <sstream>
#include <queue>
#include <iostream>
#include <deque>
#include <stack>
#include "Number.h"

static std::vector<Operator> ops(
  {
    Operator("(", 3, PARA_OPEN),
    Operator(")", 3, PARA_CLOSE),
    Operator("+", 0, ADD),
    Operator("-", 0, SUB),
    Operator("*", 1, MUL),
    Operator("/", 1, DIV)
  }
);

Operator::Operator(std::string symbol, int prec, OPERATOR_ENUM _oEnum) :
  op_symbol(symbol), precedence(prec), oEnum(_oEnum)
{
}
int Operator::GetPrecedence() const{
  return precedence;
}
const std::string& Operator::GetSymbol() const{
  return op_symbol;
}
OPERATOR_ENUM Operator::GetEnum() const
{
  return oEnum;
}
  

std::ostream& operator<<(std::ostream& os, const Operator& op)
{
  os << op.GetSymbol();
  return os;
}





std::string tokenise(std::string expr)
{
  
  bool wasNumber = false, isNumber = false;
  for (int i = 0; i < expr.length(); i++)
  {
    if (expr[i] - '0' >= 0 && expr[i] - '0' <= 9)
    {
      isNumber = true;
    }

    if ((!isNumber && wasNumber) || (!wasNumber && isNumber))
    {
      expr.insert(i, " ");
    }    
    wasNumber = isNumber;
    isNumber = false;
  }
  //std::cout << expr << '\n';
  return expr;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::stack<T>& stack) {
    std::stack<T> tempStack = stack; // Make a copy of the input stack

    os << "Stack [top -> bottom]: ";
    while (!tempStack.empty()) {
        os << tempStack.top() << " ";
        tempStack.pop();
    }
    return os;
}
std::ostream& operator<<(std::ostream& os, const std::stack<IValue*>& stack) {
    std::stack<IValue*> tempStack = stack; // Make a copy of the input stack

    os << "Stack [top -> bottom]: ";
    while (!tempStack.empty()) {
        os << *(Number*)tempStack.top() << " ";
        tempStack.pop();
    }
    return os;
}

void eval_step(std::stack<IValue*>& value_queue, std::stack<Operator>& op_queue)
{
  std::cout << "Vals: " << value_queue << '\n' << "Ops: " << op_queue << '\n';
  Operator o = op_queue.top();
  if (o.GetEnum() == PARA_OPEN)
  {
    op_queue.pop();
    return;
  }
  op_queue.pop();

  if (o.GetEnum() == PARA_CLOSE)
  {
    while (op_queue.top().GetEnum() != PARA_OPEN)
    {
      eval_step(value_queue, op_queue);
    }
    op_queue.pop();
    return;
  }
  

  IValue *lhs, *rhs;
  rhs = value_queue.top();
  value_queue.pop();
  lhs = value_queue.top();
  value_queue.pop();

  switch (o.GetEnum()){
    case MUL:
    value_queue.push(lhs->Mul(rhs));
    break;
  case DIV:
    value_queue.push(lhs->Div(rhs));
    break;
  case ADD:
    value_queue.push(lhs->Add(rhs));
    break;
  case SUB:
    value_queue.push(lhs->Sub(rhs));
    break;
  }
  delete lhs;
  delete rhs;
}


IValue* evaluate(std::string expression)
{
  expression = tokenise(expression);
  std::string token;
  std::stack<IValue*> value_queue;
  std::stack<Operator> op_queue;

  std::stringstream stream(expression);
  while (!stream.eof())
  {
    //std::cout << "Values: " << value_queue << '\n';
    stream >> token;
    //std::cout << token << '\n';
    
    std::stringstream token_stream(token);
    
    float next_val;
    token_stream >> std::noskipws >> next_val;

    if (!token_stream.fail())
    {
      value_queue.push(new Number(next_val));
      continue;
    }
    Operator* current_op;
    for (auto itr = ops.begin(); itr != ops.end(); itr++)
    {
      if (token.find(itr->GetSymbol()) != std::string::npos)
      {
        current_op = &(*itr);
        break;
      }
    }
    //std::cout << "found op " << current_op->GetSymbol() << '\n';
    while (!op_queue.empty() && op_queue.top().GetPrecedence() >= current_op->GetPrecedence())
    {
      eval_step(value_queue, op_queue);
    }
    op_queue.push(*current_op);
    if (stream.eof())
    {
      break;
    }
    if (stream.tellg() == -1)
    {
      break;
    }
  }
  while (!op_queue.empty())
  {
    eval_step(value_queue, op_queue);
  }
  //std::cout << "Values : " << value_queue << '\n';
  //std::cout << "Operators : " << op_queue << '\n';
  return value_queue.top();
}



