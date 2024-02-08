#include "rangelist.h"
#include <cstdlib>    /* new/delete */
#include <cstddef>    /* NULL */
#include <cstdio>     /* printf */
#include <algorithm>  /* std::swap */
#include <iostream>


#define xDEBUG

int IsBetweenNodes(const struct RangeList::RangeNode* node1, const struct RangeList::RangeNode* node2, int pos)
{
  if(node1 && node2)
    return pos >= node1->end && pos < node2->begin;
  return 0;
}

int PositionInNode(const struct RangeList::RangeNode* phead, int position)
{
  return position >= phead->begin-1 && position <= phead->end;
}

void ShiftDown(struct RangeList::RangeNode* phead)
{
  while (phead)
  {
    if (phead->next && phead->end == phead->next->begin)
    {
      phead->begin += 1;
      phead->end += 1;
      phead = phead->next;
    }
    else
    {
      break;
    }
  }
}

unsigned GetLength(RangeList::RangeNode* phead)
{
  unsigned len = phead->Last()->end;
  
  return len;
}

void Merge(struct RangeList::RangeNode** start)
{
  struct RangeList::RangeNode* current = *start;
  struct RangeList::RangeNode* last = *start;
  
  if ((*start)->value == 0)
  {
    current = current->next;
    delete *start;
    *start = current;
    
  }
  
  while (current)
  {
    if (current->value == 0)
    {
      last->next = current->next;
      delete current;
      current = last->next;
      if (current == NULL)
        return;
    }
    if (current->begin == current->end)
    {
      if (last != current)
      {
        last->next = current->next;
        delete current;
        current = last->next;
        if (current == NULL)
          return;
      }
      
      
    }
    if (current->value == 0)
    {
      if (last != current)
      {
        last->next = current->next;
        delete current;
        current = last->next;
        if(current == NULL){
          return;
        }
      }
    }
    if (current->next)
    {
      struct RangeList::RangeNode* next = current->next;
      
      if (next->value == current->value)
      {
        if (current->end == next->begin)
        {
          current->end = next->end;
          current->next = next->next;
          delete next;
        }
        else if (current->begin == next->begin && current->end == next->end)
        {
          current->next = next->next;
          delete next;
        }
        else
        {
          last = current;
          current = current->next;
        }
      }
      else
      {
        last = current;
        current = current->next;
      }
    }
    else
    {
      last = current;
      current = current->next;
    }
  }

  

}


void RangeList::RangeNode::dump_node() const
{
  printf( "+--------------------------\n" );
  printf( "| %p | %i [ %i, %i ) --> %p\n", reinterpret_cast<void const*>(this), value, begin, end, reinterpret_cast<void const*>(next) );
  printf( "+--------------------------\n" );
}

RangeList::RangeNode::RangeNode(int b, int e, int v) : begin(b), end(e), value(v), next(NULL)
{
}

RangeList::RangeNode::RangeNode(const RangeNode& node) : begin(node.begin), end(node.end), value(node.value), next(NULL)
{
  if (node.next)
  {
    next = new RangeNode(*node.next);
  }
}

RangeList::RangeNode* RangeList::RangeNode::Last()
{
  RangeNode* current = this;
  while (current->next)
  {
    current = current->next;
  }
  return current;
}

RangeList::RangeList() : pHead(NULL)
{
}

RangeList::RangeList(const RangeList& rhs) : pHead(NULL)
{
  if (rhs.pHead)
  {
    pHead = new RangeNode(*rhs.pHead);
  }
}

RangeList::~RangeList()
{
  while (pHead)
  {
    RangeNode* old = pHead;
    pHead = pHead->next;
    delete old;
  }
  
}

int RangeList::get(int position) const
{
  const struct RangeNode* current = this->pHead;
  const struct RangeNode* last = current;
  while (current)
  {
    
    if (position >= current->begin && position < current->end)
    {
      return current->value;
    }
    else if (IsBetweenNodes(last, current, position))
    {
      return 0;
    }
    last = current;
    current = current->next;
  }
  return 0;
}

int RangeList::insert_value(int position, int value)
{
  #define RangeNode RangeList::RangeNode
  struct RangeNode* current = pHead;
  struct RangeNode* last = NULL;
  if (pHead == NULL)
  {
    struct RangeNode* nFront = new RangeNode(position, position+1, value);
    nFront->next = pHead;
    pHead = nFront;
    return 0;
  }
  if (position < current->begin)
  {
    struct RangeNode* nFront = new RangeNode(position, position+1, value);;
    nFront->next = pHead;
    pHead = nFront;
    Merge(&pHead);
    return 0;
  }
  if (position == 0)
  {
    current = pHead;
    if (pHead->value == value)
    {
      pHead->end++;
      ShiftDown(pHead->next);
      Merge(&pHead);
    }
    else
    {
      if (current->begin == 0)
      {
        if (current->end - current->begin != 1)
        {
          struct RangeNode* nFront = new RangeNode(0, 1, value);
          nFront->next = current;
          current->begin += 1;
          pHead = nFront;          
        }
        else
        {
          current->value = value;
        }
      }
      else
      {
        struct RangeNode* nFront = new RangeNode(0, 1, value);
        nFront->next = pHead;
        pHead = nFront;
      }
      
      Merge(&pHead);
    }
    return 0;
  }

  
  while (current)
  {
    
    if (PositionInNode(current, position) && current->end != position)
    {
      break;
    }
    last = current;
    current = current->next;
    if (current == NULL)
    {
      struct RangeNode* end = new RangeNode(position, position + 1, value);
      last->next = end;
      Merge(&pHead);
      return 0;
    }
    if (IsBetweenNodes(last, current, position))
    {
      struct RangeNode* between = new RangeNode(position, position + 1, value);
      between->next = current;
      last->next = between;
      if (value != 0)
      {        
        ShiftDown(current);        
      }
      Merge(&pHead);

      return 0;
    }
    if (position >= current->end && current->next == NULL)
    {
      struct RangeNode* after = new RangeNode(position, position + 1, value);
      current->next = after;
      Merge(&pHead);
      return 0;
    }
  }

  if (current->value == value)
  {
    if (position + 1 == current->begin)
    {
      current->begin--;
      last->end--;
      
    }
    else if (position == current->end)
    {
      current->end++;
      ShiftDown(current->next);
    }
  }

  else
  {
    struct RangeNode* insert = new RangeNode(position, position + 1, value);
    if (position + 1 == current->begin)
    {
      last->next = insert;
      insert->next = current;
    }
    else if (position == current->end)
    {
      insert->next = current->next;
      current->next = insert;
    }
    else
    {
      if (current->end - current->begin != 1)
      {
        if (current->begin == position)
        {
          if (last)
          {
            last->next = insert;
          }
          else
          {
            pHead = insert;
          }
          insert->next = current;
          current->begin++;
        }
        else if (current->end == position)
        {
          current->next = insert;
        }
        else
        {
          struct RangeNode* split = new RangeNode(position + 1, current->end, current->value);

          current->end = position;
          split->next = current->next;
          current->next = insert;
          insert->next = split;
        }
        
      }
      else
      {
        current->value = value;
        delete insert;
      }

    }
  }

  Merge(&pHead);
  return 0;
  #undef RangeNode
}

int RangeList::insert_nonoverlapping_range( int begin, int end, int value )
{
  RangeNode ** ppHead = &pHead;

  /* main logic */
  if ( !*ppHead || end < (*ppHead)->begin ) { /* empty or in-front (short circuit) */
    // front next to old head same value
    if ( *ppHead && (*ppHead)->value == value && end == (*ppHead)->begin ) {
      (*ppHead)->begin = begin;
    } else {
      RangeNode * old_head = *ppHead;
      *ppHead = new RangeNode( begin, end, value );
      (*ppHead)->next = old_head;
    }
    return 0;  
  } else { /* insert in the middle or tail */
    RangeNode * p = *ppHead;
    while ( p->next && p->next->end <= begin ) { /* find the right place */
      p = p->next;
    }

    if ( !p->next ) { /* insert in the end */
      if ( p->value == value && p->end == begin ) { // if inserting same value next to exising node
        p->end = end;                               // just change the end
      } else {
        p->next = new RangeNode( begin, end, value ); /* make tail */
      }
      return 0; 
    } else { /* insert between p and p->next */
      RangeNode * old_p_next = p->next; 
      p->next = new RangeNode( begin, end, value );
      p->next->next = old_p_next;
    }
  }
  return 0;
}

void RangeList::printf_elements( char const * format, int dimension, bool do_sanity_check, bool indices ) const
{
  RangeNode const * p = pHead;
  int i, last_pos = 0;
  bool zeroes     = 0;
  bool non_merged = 0;
  bool overlapping_ranges = 0;
  /* indices */
  if ( indices ) {
    for ( i = 0; i < dimension; ++i ) { printf( format, i ); }
    printf( "\n" );
  }
  /* data */
  while ( p ) {
    for ( i = last_pos; i < p->begin; ++i ) { printf( format, 0 ); }          /* print leading zeros */
    if ( p->value == 0 ) { zeroes = 1; } /* sanity check */
    for ( i = p->begin; i < p->end;   ++i ) { printf( format, p->value ); }   /* print value */
    last_pos = p->end;
    if ( p->next && p->end >  p->next->begin ) { overlapping_ranges = 1; }    /* sanity check */
    if ( p->next && p->end == p->next->begin && p->value == p->next->value) { non_merged = 1; } /* sanity check */
    p = p->next;
  }
  for ( i = last_pos ; i < dimension; ++i ) { printf( format, 0 ); }

  /* sanity reporting */
  if ( do_sanity_check && zeroes ) {
    printf( "zero values should not be stored\n" );
  }
  if ( do_sanity_check && overlapping_ranges ) {
    printf( "overlapping ranges\n" );
  }
  if ( do_sanity_check && non_merged ) {
    printf( "ranges with the same value and next to each other - should be merged\n" );
  }
}

void RangeList::print_raw( ) const
{
  RangeNode const * p = pHead;
  while ( p ) {
    printf( "%i [ %i, %i ) --> ", p->value, p->begin, p->end );
    p = p->next;
  }
  printf( "NULL" );
}

NodeProxy RangeList::operator[](int index)
{
  return NodeProxy(index, get(index), *this);
}

int RangeList::operator*(const RangeList& rhs) const
{
  int ret = 0;
  int index = 0;
  
  RangeNode *lcurrent = pHead, *rcurrent = rhs.pHead;
  unsigned len = GetLength(rcurrent);
  while (index < static_cast<int>(len) && lcurrent && rcurrent)
  {
    int lval = get(index);
    int rval = rhs.get(index);
    ret += lval * rval;
    if (lcurrent->next)
    {
      if (index >= lcurrent->end && index >= lcurrent->next->begin)
      {
        lcurrent = lcurrent->next;
      }
    }
    if (rcurrent->next)
    {
      if (index >= rcurrent->end && index >= rcurrent->next->begin)
      {
        rcurrent = rcurrent->next;
      }
    }
    
    index++;
  }
  return ret;
}

RangeList RangeList::operator+(const RangeList& rhs) const
{
  RangeList ret(*this);
  unsigned len = GetLength(rhs.pHead);
  unsigned index = 0;

  while (index < len)
  {
    int rval = rhs.get(index);
    int lval = ret.get(index);
    ret[index] = lval + rval;
    index++;
  }

  return ret;
}

RangeList& RangeList::operator=(const RangeList& rhs)
{
  delete this->pHead;
  if (rhs.pHead)
    this->pHead = new RangeNode(*rhs.pHead);
  else
    this->pHead = NULL;
  return *this;
}

NodeProxy::NodeProxy(int _pos, int _val, RangeList& _list) :list(_list), pos(_pos), val(_val)
{
}

NodeProxy& NodeProxy::operator=(int val)
{
  list.insert_value(pos, val);
  return *this;
}

NodeProxy& NodeProxy::operator+=(int _val)
{
  list.insert_value(pos, val + _val);
  return *this;
}

NodeProxy& NodeProxy::operator*=(int _val)
{
  list.insert_value(pos, val * _val);
  return *this;
}

NodeProxy::operator int() const
{
  return val;
}

