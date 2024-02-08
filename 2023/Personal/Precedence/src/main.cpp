#include "evaluation.h"
#include <iostream>
#include "Number.h"

int main(void)
{
  std::string s("2 * (3/4) *2-1");

  Number* result = (Number*)evaluate(s);
  
  std::cout << s << " = " << *result << '\n';
  return 0;
}