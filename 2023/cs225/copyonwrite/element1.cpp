#include "element1.h"

int DigiPen::Element1::_alive = 0;

int DigiPen::Element1::_total = 0;

DigiPen::Element1::Element1(int _val) : val(_val)
{
  _total++;
  _alive++;
}

DigiPen::Element1::~Element1()
{
  _alive--;
}

int DigiPen::Element1::Get() const { return val; }

void DigiPen::Element1::Set(int new_val) { val=new_val; }

void DigiPen::Element1::Print() const { 
  std::cout << "[" << Get() << "] "; 
}

int DigiPen::Element1::GetAlive()
{
  return _alive;
}

int DigiPen::Element1::GetTotal()
{
  return _total;
}

int DigiPen::Element1::GetID() const
{
  return 1;
}

