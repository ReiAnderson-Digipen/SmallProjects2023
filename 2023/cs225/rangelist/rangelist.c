#include "rangelist.h"
#include <stdlib.h> /* NULL/malloc/free */
#include <stdio.h>  /* printf */

#define xDEBUG

/* helper function, prototyped in the implementation file, so NOT available in the main */
void dump_node(struct RangeNode* p);
static int IsBetweenNodes(const struct RangeNode* node1, const struct RangeNode* node2, int pos);


/* your implementations: insert_value, get, delete */

void delete(struct RangeNode* p)
{
  struct RangeNode* current = p;
  while (current)
  {
    struct RangeNode* next = current->next;
    /*printf("%i\n", i++);*/
    free(current);
    current = next;
  }
}

int get(struct RangeNode const* p, int position)
{
  const struct RangeNode* current = p;
  const struct RangeNode* last = p;
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


static void ShiftDown(struct RangeNode* phead)
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

static int PositionInNode(const struct RangeNode* phead, int position)
{
  return position >= phead->begin-1 && position <= phead->end;
}

static int IsBetweenNodes(const struct RangeNode* node1, const struct RangeNode* node2, int pos)
{
  return pos >= node1->end && pos < node2->begin;
}

static struct RangeNode* CreateNode(int begin, int end, int value)
{
  struct RangeNode* ret = (struct RangeNode*)calloc(1, sizeof(struct RangeNode));
  ret->begin = begin;
  ret->end = end;
  ret->value = value;
  ret->next = NULL;
  return ret;
}

static void Merge(struct RangeNode** start)
{
  struct RangeNode* current = *start;
  struct RangeNode* last = *start;
  
  if ((*start)->value == 0)
  {
    current = current->next;
    free(*start);
    *start = current;
    
  }
  
  while (current)
  {
    if (current->value == 0)
    {
      last->next = current->next;
      free(current);
      current = last->next;
      if (current == NULL)
        return;
    }
    if (current->begin == current->end)
    {
      if (last != current)
      {
        last->next = current->next;
        free(current);
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
        free(current);
        current = last->next;
        if(current == NULL){
          return;
        }
      }
    }
    if (current->next)
    {
      struct RangeNode* next = current->next;
      
      if (next->value == current->value)
      {
        if (current->end == next->begin)
        {
          current->end = next->end;
          current->next = next->next;
          free(next);
        }
        else if (current->begin == next->begin && current->end == next->end)
        {
          current->next = next->next;
          free(next);
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

int insert_value(struct RangeNode** ppHead, int position, int value)
{
  struct RangeNode* current = *ppHead;
  struct RangeNode* last = NULL;
  if (*ppHead == NULL)
  {
    struct RangeNode* nFront = CreateNode(position, position+1, value);
    nFront->next = *ppHead;
    *ppHead = nFront;
    return 0;
  }
  if (position < current->begin)
  {
    struct RangeNode* nFront = CreateNode(position, position+1, value);
    nFront->next = *ppHead;
    *ppHead = nFront;
    Merge(ppHead);
    return 0;
  }
  if (position == 0)
  {
    current = *ppHead;
    if ((*ppHead)->value == value)
    {
      (*ppHead)->end++;
      ShiftDown((*ppHead)->next);
      Merge(ppHead);
    }
    else
    {
      if (current->begin == 0)
      {
        if (current->end - current->begin != 1)
        {
          struct RangeNode* nFront = CreateNode(0, 1, value);
          nFront->next = current;
          current->begin += 1;
          *ppHead = nFront;          
        }
        else
        {
          current->value = value;
        }
      }
      else
      {
        struct RangeNode* nFront = CreateNode(0, 1, value);
        nFront->next = *ppHead;
        *ppHead = nFront;
      }
      
      Merge(ppHead);
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
      struct RangeNode* end = CreateNode(position, position + 1, value);
      last->next = end;
      Merge(ppHead);
      return 0;
    }
    if (IsBetweenNodes(last, current, position))
    {
      struct RangeNode* between = CreateNode(position, position + 1, value);
      between->next = current;
      last->next = between;
      ShiftDown(current);
      Merge(ppHead);

      return 0;
    }
    if (position >= current->end && current->next == NULL)
    {
      struct RangeNode* after = CreateNode(position, position + 1, value);
      current->next = after;
      Merge(ppHead);
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
    struct RangeNode* insert = CreateNode(position, position + 1, value);
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
            *ppHead = insert;
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
          struct RangeNode* split = CreateNode(position + 1, current->end, current->value);

          current->end = position;
          split->next = current->next;
          current->next = insert;
          insert->next = split;
        }
        
      }
      else
      {
        current->value = value;
        free(insert);
      }

    }
  }

  Merge(ppHead);
  return 0;
}




/* implementations - DO NOT MODIFY*/
void dump_node ( struct RangeNode * p )
{
  printf( "+--------------------------\n" );
  printf( "| %p | %i [ %i, %i ) --> %p\n", (void*)p, p->value, p->begin, p->end, (void*)p->next );
  printf( "+--------------------------\n" );
}

void printf_elements( struct RangeNode const * p, char * format, int dimension, int do_sanity_check )
{
  int i, last_pos = 0;
  typedef int boolean;
  boolean zeroes     = 0;
  boolean non_merged = 0;
  boolean overlapping_ranges = 0;
  /* indices */
  for ( i = 0; i < dimension; ++i ) { printf( format, i ); }
  printf( "\n" );
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

void print_raw( struct RangeNode const * p)
{
  while ( p ) {
    printf( "%i [ %i, %i ) --> ", p->value, p->begin, p->end );
    p = p->next;
  }
  printf( "NULL" );
}

int insert_nonoverlapping_range( struct RangeNode ** ppHead, int begin, int end, int value )
{
  struct RangeNode * new_node = (struct RangeNode*) malloc( sizeof( struct RangeNode ) );
  if ( new_node == NULL ) { 
    return 1;
  }

  new_node->begin = begin;
  new_node->end   = end;
  new_node->value = value;

  /* main logic */
  if ( !*ppHead || end < (*ppHead)->begin ) { /* empty or in-front (short circuit) */
    new_node->next = *ppHead;
    *ppHead = new_node;
    return 0;  
  } else { /* insert in the middle or tail */
    struct RangeNode * p = *ppHead;
    while ( p->next && p->next->end <= begin ) { /* find the right place */
      p = p->next;
    }

    if ( !p->next ) { /* insert in the end */
      new_node->next = NULL;
      p->next = new_node; /* make tail */
      return 0; 
    } else { /* insert between p and p->next */
      new_node->next = p->next; 
      p->next = new_node;
    }
  }
  return 0;
}

