#include <iostream>
#include <utility>

using namespace std;

template <typename T>
TVector<T> ::TVector()//Default constructor
{
    capacity = SPARECAPACITY;
    size = 0;
    array = new T[capacity];
}

template <typename T>
TVector<T> ::TVector(T val, int num)//Parameterized constructor
{
    capacity = num + SPARECAPACITY;
    size = num;
    array = new T[capacity];

    for (int i = 0; i < num; ++i)
    {
        array[i] = val;
    }

}

template <typename T>
void TVector<T>::Clear()
{
    size = 0;
    delete []array;
    array = nullptr;
    capacity = SPARECAPACITY;
}

template <typename T>
TVector<T> :: ~TVector()//Destructor
{
    if (array != 0)	// if not null pointer
        delete [] array;	// clean up the data and recollect the memory
}

template <typename T>
TVector<T> :: TVector(const TVector<T>& v)//copy constructor
{

    // member by member deep copy
    capacity = v.capacity;
    size = v.size;
    array = new T[capacity];
    for (int i = 0; i < size; i++)
        array[i] = v.array[i];
}

template <typename T>
TVector<T>& TVector<T> ::operator=(const TVector<T>& v)
{

    if (this != &v)		// if not self-assignment
    {

        delete[] array;		// clean up old array

        // member by member assignment
        capacity = v.capacity;
        size = v.size;
        array = new T[capacity];
        for (int i = 0; i < size; i++)
            array[i] = v.array[i];

    }

    return *this;		// return calling obj, for cascading
}

template <typename T>
TVector<T> ::TVector(TVector<T> && v)//move constructor
{

    capacity = v.capacity;
    size = v.size;
    array = v.array; // take the pointer, steal the data in v

    v.array = nullptr;		// null out parameter's pointer s.t. v's destructor will not delete *arr
    v.capacity = v.size = 0;		// null out parameter's trackers
}

template <typename T>
TVector<T>& TVector<T> :: operator=(TVector<T> && v)//move assignment operator
{
    std::swap(capacity, v.capacity);	// swap trackers between calling obj and param
    std::swap(size, v.size);
    std::swap(array, v.array);	// trade pointers between calling obj and param

    return *this;
}

template <typename T>
bool TVector<T>::IsEmpty() const//returns true if array is empty
{
    if(size == 0)
        return true;
    else return false;
}

template <typename T>
int TVector<T>::GetSize() const
{
    return size;
}

template <typename T>
T& TVector<T>::GetFirst() const
{
    if (IsEmpty())// if array is empty return dummy
    {
        TVector<T>::dummy;
    }
    else
        return array[0];
}

template <typename T>
T& TVector<T>::GetLast() const
{
    if (IsEmpty())// if array is empty return dummy
    {
        TVector<T>::dummy;
    }
    else
        return array[size - 1];
}

template <typename T>
void TVector<T>::InsertBack(const T& d)
{
    if(size == capacity)
    {
        SetCapacity(2 * capacity);
    }
    array[size++] = d;

}

template <typename T>
void TVector<T>::RemoveBack() //check if its empty then if not decrease size by 1
{
    if (size > 0)
    {
        size--;
        if (size < capacity/2)
        {
            SetCapacity(capacity/2);
        }
    }
    else {
        cout << "Empty";
    }
}

template <typename T>
TVectorIterator<T> TVector<T>::GetIterator() const //initializes iterator private members to point to first array object
{
    TVectorIterator<T> itr;
    if (size > 0)
    {
        itr.index = 0;
        itr.ptr = array;
        itr.vsize = size;
    }
    return itr;
}

template <typename T>
TVectorIterator<T> TVector<T>::GetIteratorEnd() const //initializes iterator private members to point to last array object
{
    TVectorIterator<T> itr;
    if (size > 0)
    {
        itr.index = size - 1;
        itr.ptr = array + size - 1;
        itr.vsize = size;
    }
    return itr;
}

template <typename T>
void TVector<T>::SetCapacity(unsigned int c) //check
{
    if (c > 0 && c != capacity)//Check if the new capacity is greater than 0 and not equal to the current capacity
    {
        T* NewArray = new T[c]; // Create a new array with the new capacity
        int newSize;

        if (c < size) { // If the new capacity is less than the current size, set the new size to the new capacity
            newSize = c;
        } else {
            newSize = size;
        }
        for (int i = 0; i < newSize; ++i)
        {
            NewArray[i] = array[i];
        }

        delete[] array;
        array = NewArray;
        capacity = c;
        size = newSize;
    }
}


template <typename T>
TVectorIterator<T> TVector<T>::Insert(TVectorIterator<T> pos, const T& d)
{
    int p = pos.index; // Get the index of the position passed in as an argument
    TVectorIterator<T> ret_pos = pos; // Create a new iterator and set it to the position passed in as an argument
    if (size == capacity)//increase the capacity if size is equal to capacity and update iterator's pointer to the correct position
    {
        SetCapacity(2 * capacity + 1);
        ret_pos.ptr = array + pos.index;
    }

    if(size == 0)  // If the vector is empty, insert the element at the first position and update the iterator's index and pointer
    {
        array[0] = d;
        ret_pos.index = 0;
        ret_pos.ptr = array;

    }
    else if( p >= 0 && p <= size - 1 ) // if the position is within the range of the vector, shift all elements after the position to the right
    {
        for(int i = size - 1; i >= p; i--)
        {
            array[i + 1] = array[i];
        }
        array[p] = d; //Insert the element at the specified position
    }
    else { // If the position is out of range
        array[size] = d; // Insert the element at the end of the vector
        ret_pos.index = size; // Update the iterator's index and pointer
        ret_pos.ptr = array + size;
    }

    size++;
    ret_pos.ptr = array + size; // Update the iterator's pointer to the end of the vector
    return ret_pos;
}

template <typename T>
TVectorIterator<T> TVector<T>::Remove(TVectorIterator<T> pos)
{
    if(IsEmpty() || pos.index >= size || pos.index < 0) //do proper checks to return an empty iterator
    {
        TVectorIterator<T> ret_pos;

        return ret_pos;
    }

    TVectorIterator<T> ret_pos = pos; //Create a new iterator and set it to the position passed in as an argument
    for (int i = pos.index; i < size - 1; i++) // Shift all elements after the position to the left
    {
        array[i] = array[i + 1];
    }
    size--;//decrease the size by 1
    ret_pos.vsize = size;
    return ret_pos;
}

template <typename T>
TVectorIterator<T> TVector<T>::Remove(TVectorIterator<T> pos1, TVectorIterator<T> pos2) // chnage the whole thing
{
    TVectorIterator<T> t;

    // Check if the position1 and position2 are valid and position1 is before position2
    if(pos1.index >= 0 && pos1.index < size && pos2.index >= 0 && pos2.index <= size && pos1.index < pos2.index)
    {
        t.index = pos2.index; // Set the iterator t to position2
        t.ptr = array + t.index;
        t.vsize = size - (pos2.index - pos1.index); // Set the vsize variable of the iterator t

        for (int i = pos1.index; i < size - (pos2.index - pos1.index); ++i)
        {
            array[i] = array[i + (pos2.index - pos1.index)]; // Shift all elements between position1 and position2 to the right
        }
        size -= (pos2.index - pos1.index); // Decrement the size variable
    }

    return t;
}

template <typename T>
void TVector<T>::Print(std::ostream& os, char delim) const
{
    if (size == 0)
    {
        os << "Array is empty" << endl;//if array is empty return that the array is empty
    }

    for (int i = 0; i < size; i++)//go through the array and print out each value
    {
        os << delim << array[i];
    }

}

template <typename T>
TVector<T> operator+(const TVector<T>& t1, const TVector<T>& t2)
{
    TVector<T> itr = t1; //create a new vector and set it equal to t1
    itr.SetCapacity(t1.GetSize() + t2.GetSize()); // Set the capacity of the new vector to the sum of the sizes of t1 and t2


    TVectorIterator<T> it2 = t2.GetIterator(); // Get an iterator to the beginning of t2
    while (it2.HasNext())//while there is a next variable in it2, insert the next element of t2 at the end of the new vector
    {
        itr.InsertBack(it2.GetData());
        it2.Next();
    }

    return itr; //return the new vector
}

template <typename T>
TVectorIterator<T> ::TVectorIterator()//initialize the values
{
    index = 0;
    ptr = new T[index];
    vsize = 0;
}

template <typename T>
bool TVectorIterator<T> ::HasNext() const // return true if the index is less than the size
{
    return index < vsize;
}

template <typename T>
bool TVectorIterator<T>::HasPrevious() const //return true if the index is not the first array value
{
    if(index != 0)
        return true;
    else
        return false;

}

template <typename T>
TVectorIterator<T> TVectorIterator<T>::Next() //does proper checks then increases the index and pointer
{
    if (HasNext() && ptr != nullptr)
    {
        index++;
        ptr++;
    }
    return *this;
}

template <typename T>
TVectorIterator<T> TVectorIterator<T>::Previous() // does proper checks and then decreases the index and pointer
{
    if(HasPrevious() && ptr != nullptr)
    {
        index--;
        ptr--;
    }
    return *this;
}

template <typename T>
T& TVectorIterator<T>::GetData() const //returns a dummy if the pointer is pointing to nothing and if not returns the ptr
{
    if(ptr == nullptr)
    {
        TVector<T>::dummy;
    }
    return *ptr;
}
