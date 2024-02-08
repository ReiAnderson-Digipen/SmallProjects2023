#ifndef RANGELIST_H
#define RANGELIST_H

// forward declaration
class NodeProxy; 

class RangeList {
  public:
    
    RangeList();
    RangeList(const RangeList& rhs);
    ~RangeList();

    int get(int position) const;
    int insert_value( int position, int value );
    int insert_nonoverlapping_range( int begin, int end, int value );
    void printf_elements( char const * format, int dimension, bool do_sanity_check, bool indices = true ) const; 
    void print_raw( ) const;

    NodeProxy operator[](int index);
    int operator*(const RangeList& rhs) const;
    RangeList operator+(const RangeList& rhs) const;
    RangeList& operator=(const RangeList& rhs);

    friend class NodeProxy;
private:
    struct RangeNode {
      int begin;
      int end;
      int value;
      struct RangeNode * next;
      void dump_node ( ) const;
      RangeNode(int b, int e, int v);
      RangeNode(const RangeNode& node);
      RangeNode* Last();
    };
    RangeNode * pHead;
private: // helpers
  friend int IsBetweenNodes(const struct RangeList::RangeNode* node1, const struct RangeList::RangeNode* node2, int pos);
  friend int PositionInNode(const struct RangeList::RangeNode* phead, int position);
  friend void Merge(struct RangeList::RangeNode** start);
  friend void ShiftDown(struct RangeList::RangeNode* phead);
  friend unsigned GetLength(struct RangeList::RangeNode* phead);


};

class NodeProxy {
public:
  NodeProxy(int pos, int val, RangeList& list);
  NodeProxy& operator=(int val);
  NodeProxy& operator+=(int val);
  NodeProxy& operator*=(int val);
  operator int() const;

private:
  RangeList& list;
  int pos, val;

};
#endif
