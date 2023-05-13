#ifndef LIST_HPP // LIST_HPP
#define LIST_HPP

template <class T>

class List
{
protected:
    int size;
    T *data;

public:
    class Range
    {
    };
    List(int size);
    List(const List &other);
    ~List()
    {
        delete[] data;
    };
    T &operator[](int index);
    int getSize() { return size; };
    void resize(int newSize);
    List &operator=(const List &other);
};

template <class T>
List<T>::List(int size)
{
    this->size = size;
    this->data = new T[size];
    // for (int i = 0; i < size; i++)
    // {
    //     this->data[i] = new T;
    // }
}

template <class T>
List<T>::List(const List &other)
{
    this->size = other.size;
    this->data = new T[size];

    for (int i = 0; i < size; i++)
    {
        this->data[i] = other.data[i];
    }
}

template <class T>
T &List<T>::operator[](int index)
{
    if (index < 0 || index >= size)
    {
        throw Range();
    }
    return data[index];
}

template <class T>
void List<T>::resize(int newSize)
{
    T *newData = new T[newSize];

    for (int i = 0; i < size; i++)
    {
        newData[i] = data[i];
    }

    delete data;
    data = newData;
    size = newSize;
}

template <class T>
List<T> &List<T>::operator=(const List &other)
{
    if (this != &other)
    {
        delete data;
        size = other.size;
        data = new T[size];

        for (int i = 0; i < size; i++)
        {
            data[i] = other.data[i];
        }
    }

    return *this;
}

#endif // LIST_HPP
