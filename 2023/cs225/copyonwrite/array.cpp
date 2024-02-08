#include "array.h"
// if Factory is used cirrectly, there will be no more includes

namespace DigiPen {
	Array::Array(int* array,
		unsigned int _size,
		const ElementFactory* _pElementFactory
	)
		: data(new AbstractElement* [_size]),
		ref_count(new unsigned int(1)),
		size(_size),
		pElementFactory(_pElementFactory)
	{
		for (unsigned int i = 0; i < size; ++i)
		{
			//create Element with id = 1 (that is Element1 )
			data[i] = pElementFactory->MakeElement(1, array[i]);
		}
	}

	Array::Array(const Array& other): data(other.data), ref_count(other.ref_count), size(other.size), pElementFactory(other.pElementFactory)
	{
		(*ref_count)++;
	}

	Array& Array::operator=(const Array& other)
	{
		(*ref_count)--;
		if (*ref_count <= 0)
		{
			DeleteData();
			delete ref_count;
		}


		data = other.data;
		ref_count = other.ref_count;
		(*ref_count)++;
		size = other.size;
		pElementFactory = other.pElementFactory;



		return *this;
	}

	Array::~Array()
	{
		(*ref_count)--;
		if (*ref_count <= 0)
		{
			DeleteData();
			delete ref_count;
		}

	}

	int Array::Get(unsigned int pos) const
	{
		return data[pos]->Get();
	}

	void Array::Set(int id, int pos, int value)
	{
		DeepCopy(); // modifying the data - need our own copy
		delete data[pos];
		data[pos] = pElementFactory->MakeElement(id, value); // make sure is used 
	}

	void Array::Print() const
	{
		for (unsigned int i = 0;i < size;++i) data[i]->Print();
		std::cout << std::endl;
	}

	void Array::DeleteData()
	{
		for (unsigned i = 0; i < size; i++)
		{
			delete data[i];
		}
		delete[] data;
	}

	void Array::DeepCopy()
	{
		if (*ref_count > 1)
		{
			AbstractElement** copy = new AbstractElement * [size];

			for (unsigned i = 0; i < size; i++)
			{
				AbstractElement* elem = data[i];
				copy[i] = pElementFactory->MakeElement(elem->GetID(), elem->Get());
			}
			(*ref_count)--;
			data = copy;
			ref_count = new unsigned int(1);
		}

	}
}
