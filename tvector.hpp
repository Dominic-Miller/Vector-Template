
using namespace std;

//Beginning of TVector Class

//Default constructor, create empty vector
template <typename T>
TVector<T>::TVector() {
    size = 0;
    capacity = size + SPARECAPACITY;
    array = new T[capacity]; //new empty array
}

//Paramaterized constructor, create vector with num copies of val
template <typename T>
TVector<T>::TVector(T val, int num) {
    size = num;
    capacity = size + SPARECAPACITY;
    array = new T[capacity]; //new empty array
    for (int i = 0; i < num; i++) {
        array[i] = val; //val into every element of array
    }
}

//Destructor (1/5)
template <typename T>
TVector<T>::~TVector() {
    delete [] array; //clean up
}

//Copy constructor (2/5)
template <typename T>
TVector<T>::TVector(const TVector<T>& v) {
    size = v.size;
    capacity = v.capacity;
    array = new T[capacity]; //new empty array
    for (int i = 0; i < size; i++) {
        array[i] = v.array[i]; //copy all the values
    }
}

//Copy assignment operator (3/5)
template <typename T>
TVector<T>& TVector<T>::operator=(const TVector<T>& v) {
    if (this != &v) {
        delete [] array;
        size = v.size;
        capacity = v.capacity;
        array = new T[capacity]; //new empty array
        for (int i = 0; i < size; i++) {
            array[i] = v.array[i]; //copy all the values
        }
    }
    //Could also create a copy of the rhs and use std::swap

    return *this;
}

//Move constructor (4/5)
template <typename T>
TVector<T>::TVector(TVector<T>&& v) {
    size = v.size;
    capacity = v.capacity;
    array = v.array; //Calling copy assignment operator

    v.array = nullptr; //Protects the data of the calling object
    v.size = 0;
    v.capacity = 0;
}

//Move assignment operator (5/5)
template <typename T>
TVector<T>& TVector<T>::operator=(TVector<T>&& v) {
    std::swap(size, v.size); //swap everything from v into vector
    std::swap(capacity, v.capacity);
    std::swap(array, v.array);
    return *this; //return reference to vector
}

//Checks to see whether container is empty
template <typename T>
bool TVector<T>::IsEmpty() const {
    return size == 0; //Checks if empty
}

//Clear out Vector, reset to empty
template <typename T>
void TVector<T>::Clear() {
    delete [] array;
    size = 0;
    capacity = size + SPARECAPACITY;
    array = new T[capacity]; //new empty array
}

//Return the size of the Vector
template <typename T>
int TVector<T>::GetSize() const {
    return size;
}

//Insert data d as last element
template <typename T>
void TVector<T>::InsertBack(const T& d) {
    if (size == capacity) {
        capacity += SPARECAPACITY; //grow array
        T* tmp = new T[capacity]; //new tmp array
        for (int i = 0; i < size; i++) {
            tmp[i] = array[i]; //copy everything into tmp
        }
        std::swap(array, tmp); //swap the arrays
        delete [] tmp; //manage memory
    }
    array[size] = d;
    size += 1;
}

//Remove last element of Vector
template <typename T>
void TVector<T>::RemoveBack() {
    size -= 1; //Does not reallocate the memory, but uses size to keep track of end of array
}

//Return first element of Vector
template <typename T>
T& TVector<T>::GetFirst() const {
    return array[0];
}

//Return last element of Vector
template <typename T>
T& TVector<T>::GetLast() const {
    return array[size - 1];
}

//Return iterator to first item
template <typename T>
TVectorIterator<T> TVector<T>::GetIterator() const {
    TVectorIterator<T> it;
    if (size > 0) {
        it.ptr = &array[0]; //&array[0] is the address of the first data member
        it.index = 0;
        it.vsize = size;
    }
    return it;
}

//Return iterator to last item
template <typename T>
TVectorIterator<T> TVector<T>::GetIteratorEnd() const {
    TVectorIterator<T> it;
    if (size > 0) {
        it.ptr = &array[size - 1]; //&array[size - 1] is the address of the last data member
        it.index = size - 1;
        it.vsize = size;
    }
    return it;
}

//Reset the capacity of the Vector to c.  Adjust size if c is smaller
template <typename T>
void TVector<T>::SetCapacity(unsigned int c) {
    if (c < size) {
        size = c; //anything after size won't be accessible
    }
    else {
        capacity = c;
        T* tmp = new T[capacity]; //grow array
        for (int i = 0; i < size; i++) {
            tmp[i] = array[i]; //copy array
        }
        std::swap(array, tmp); //swap arrays back
        delete [] tmp; //manage memory
    }
}

//Insert data element d just before item at pos position, return iterator to the new data item
template <typename T>
TVectorIterator<T> TVector<T>::Insert(TVectorIterator<T> pos, const T& d) {
    if (size == capacity) {
        SetCapacity(size + SPARECAPACITY); //grow array
    }
    T object = array[pos.index]; //save object at pos
    array[pos.index] = d; //insert d at pos
    for (int i = size; i > pos.index; i--) { //move every element after pos over one
        array[i] = array[i - 1];
    }
    array[pos.index + 1] = object; //put object back in after d

    size++; //increase size
    TVectorIterator<T> ret_pos = pos;
    ret_pos.vsize = size;
    return ret_pos;

}

//Remove data item at position pos. Return iterator to the item that comes after the one being deleted
template <typename T>
TVectorIterator<T> TVector<T>::Remove(TVectorIterator<T> pos) {
    //if invalid, return default iterator
    if (size == 0 || pos.index >= size || pos.index < 0) {
        TVectorIterator<T> ret_pos;
        return ret_pos;
    }

    for (int i = pos.index; i < size - 1; i++) { //move everything after pos over to the left one
        array[i] = array[i + 1];
    }

    size--; //decrease size
    TVectorIterator<T> ret_pos = pos; //pos is now just pointing to new data member at same position
    ret_pos.vsize = size;
    return ret_pos;
}

//Remove data item in range [pos1, pos2), return iterator to the item that comes after the deleted items
template <typename T>
TVectorIterator<T> TVector<T>::Remove(TVectorIterator<T> pos1, TVectorIterator<T> pos2) {
    //if invalid, do not remove anything and return default iterator
    if (size == 0 || pos1.index >= size || pos1.index < 0 || pos2.index >= size || pos2.index < 0 || pos1.index == pos2.index) {
        TVectorIterator<T> ret_pos;
        return ret_pos;
    }

    int position = pos2.index; //save the index of the item after last position deleted
    int otherposition = pos1.index; //save the index of the first item deleted
    for (int i = pos2.index; i < size; i++) { //move everything over to the left that comes after position 2 index - 1
        array[otherposition] = array[position];
        otherposition++;
        position++;
    }
    TVectorIterator<T> ret_pos = pos2; //position 2 was not deleted
    ret_pos.index = pos1.index; //index moves to first element not deleted
    size = size - (pos2.index - pos1.index); //decrease size
    ret_pos.vsize = size; //track size
    return ret_pos;
}

//Print Vector contents in order, separated by given delimiter
template <typename T>
void TVector<T>::Print(std::ostream& os, char delim) const {
    if (size == 0) {
        os << "Nothing to print. Size is 0" << endl; //if vector is empty
    }

    else {
        for (int i = 0; i < size; i++) {
            os << array[i] << delim; //if vector is not empty
        }
    }
}

//End of TVector Class




//Stand-alone function for concatenating two TVector objects
template <typename T>
TVector<T> operator+(const TVector<T>& t1, const TVector<T>& t2) {
    int s1 = t1.GetSize();
    int s2 = t2.GetSize();
    TVector<T> v = t1; //create new vector which is a copy of t1
    v.SetCapacity(s1 + s2); //capacity is sum of sizes
    if (s2 > 0) {
        TVectorIterator<T> it = t2.GetIterator(); //get iterator of t2
        v.InsertBack(it.GetData()); //insert into v
        while (it.HasNext()) { //repeat while t2 has next
            v.InsertBack(it.Next().GetData());
        }
    }
    return v; //return new vector
}



//Beginning TVectorIterator Class

//Default constructor
template <typename T>
TVectorIterator<T>::TVectorIterator() {
    index = 0;
    vsize = 0;
    ptr = nullptr; //initialize the iterator to be nullptr
}

//Next item available?
template <typename T>
bool TVectorIterator<T>::HasNext() const {
    if (index + 1 < vsize) { //if 1 + the index of the itr is less than the size of vector
        return true;
    }
    return false;
}

//Previous item available?
template <typename T>
bool TVectorIterator<T>::HasPrevious() const {
    if (index > 0) { //there will be itr at index 0
        return true;
    }
    return false;
}

//Advance to next item
template <typename T>
TVectorIterator<T> TVectorIterator<T>::Next() {
    if (ptr != nullptr && HasNext()) {
        index++;
        ptr++; //move the index & pointer to the next position
        return *this;
    }
    return *this;
}

//Move to previous item
template <typename T>
TVectorIterator<T> TVectorIterator<T>::Previous() {
    if (ptr != nullptr && HasPrevious()) {
        index--;
        ptr--; //move the index & pointer to the previous position
        return *this;
    }
    return *this;
}

//Return data element of current node
template <typename T>
T& TVectorIterator<T>::GetData() const {
    if (ptr == nullptr) {
        return TVector<T>::dummy; //return the static dummy
    }
    return *ptr;
}

//End of TVectorIterator Class
