#include <iostream>
#include <iomanip>

#if 1
template <typename T, int Size>
std::ostream& operator<<( std::ostream &os, Lariat<T, Size> const & list )
{
    typename Lariat<T, Size>::LNode * current  = list.head_;
    int index = 0;
    while ( current ) {
        os << "Node starting (count " << current->count << ")\n";
        for ( int local_index = 0; local_index < current->count; ++local_index ) {
            os << index << " -> " << current->values[local_index] << std::endl;
            ++index;
        }
        os << "-----------\n";
        current = current->next;
    }
    return os;
}

template <typename T, int Size>
Lariat<T, Size>::Lariat() : asize_(Size), head_(tail_(nullptr)), size_(0), nodecount_(0)
{
    
}

template <typename T, int Size>
Lariat<T, Size>::Lariat(const Lariat& rhs) : size_(rhs.size_), asize_(Size), head_(tail_(nullptr))
{
    auto current = rhs.head_;
    while (current)
    {
        for (int i = 0; i < current->count; i++)
        {
            push_back(current->values[i]);
        }
        current = current->next;
    }

}

template<typename T, int Size>
template <typename R, int rsize>
Lariat<T, Size>::Lariat(const Lariat<R, rsize>& rhs) : size_(rhs.size_), asize_(Size), head_(tail_(nullptr))
{
    auto current = rhs.head_;
    while (current)
    {
        for (int i = 0; i < current->count; i++)
        {
            push_back(static_cast<T>(current->values[i]));
        }
        current = current->next;
    }
}

template <typename T, int Size>
Lariat<T, Size>::~Lariat()
{
    auto current = head_
    while(current)
    {
        auto old = current;
        current = current->next;
        delete old;
    }

}

template <typename T, int Size>
Lariat<T, Size>& Lariat<T, Size>::operator=(const Lariat& rhs)
{
    erase();
    for (auto current = rhs.head_; current != nullptr; current = current->next)
    {
        for (auto i = 0; i < current->count; i++)
        {
            push_back(current->values[i]);
        }
    }
}

template <typename T, int Size>
template<typename R, int rsize>
Lariat<T, Size>& Lariat<T, Size>::operator=(const Lariat<R, rsize>& rhs)
{
    erase();
    for (auto current = rhs.head_; current != nullptr; current = current->next)
    {
        for (auto i = 0; i < current->count; i++)
        {
            push_back(static_cast<T>(current->values[i]));
        }
    }
}

template <typename T, int Size>
void Lariat<T, Size>::insert(int index, const T& val)
{
    if (index > size)
    {
        throw LariatException(LariatException::E_BAD_INDEX, "Subscript is out of range");
    }
    else if (index == size)
    {
        push_back(val);
    }
    else if (index == 0)
    {
        push_front(val);
    }
    else
    {
        int node = index / asize_;
        int aindex = index % asize_;
        auto current = head_;
        for (int i = 0; i < node; i++)
        {
            current = current->next;
        }
        

        if (aindex > current->count)
        {
            current->values[aindex] = val;
            current->count++;
            
        }
        else if (aindex <= current->count)
        {
            shift_down(aindex, current);
            current.values[aindex] = val;

        }
        size++;

    }

}

template <typename T, int Size>
void Lariat<T, Size>::shift_down(int& index, LNode*& current)
{
    int shift_count = asize_ - index;
    int next_count = asize_ - (current->count + shift_count)
    // split me!
    if ( next_count > 0)
    {
        LNode* sp = new LNode;
        sp->count = 0;
        sp->next = current->next;
        sp->prev = current;
        current->next = sp;

        for (int i = 0; i < next_count; i++)
        {
            sp->values[i] = current->values[asize - 1 - next_count + i];
        }
        

    }
    for (int i = 0; i < shift_count - next_count; i++)
    {
        current->values[asize_ - 1 - i] = current->values[asize_ - 2 - i];
    }
}

#else // fancier 
#endif
