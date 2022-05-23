#pragma once

#include <vector>
#include <string>
#include <stdexcept>



template <typename T>
class MyVector final : public std::vector<T> // Avoid inheriting from classes that not designed for this purpose
{

public:
    MyVector();
    MyVector(const MyVector& other);
    MyVector& operator=(const MyVector& other); // Add assignment operator

    ~MyVector();

    void push_back(const T& obj, const std::string& name);
    void push_back(T&& obj, const std::string& name);


    std::pair<const T&, const std::string&> operator[](int index) const;
    std::pair<T&, std::string&> operator[](int index); // Add non-const operator


    const T& operator[](const std::string& name) const;
    T& operator[](const std::string& name); // Add non-const operator


private:
    void copy_names();


private:
    std::vector<std::string>* m_names;
    size_t* m_ref_ptr;
};


template <typename T>
MyVector<T>::MyVector()
{
    m_ref_ptr = new size_t(1);
    m_names = new std::vector<std::string>();
}

template <typename T>
MyVector<T>::MyVector(const MyVector& other)
    : std::vector<T>(other)
    , m_ref_ptr(other.m_ref_ptr)
    , m_names(other.m_names)
{
    (*m_ref_ptr)++;
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector& other) 
{
    if (this == &other) 
    {
        return *this;
    }

    std::vector<T>::operator=(other);

    m_ref_ptr = other.m_ref_ptr;
    m_names = other.m_names;

    (*m_ref_ptr)++;

    return *this;
}

template <typename T>
MyVector<T>::~MyVector()
{
    if (--(*m_ref_ptr) == 0)
    {
        delete m_ref_ptr;
        delete m_names;
    }
}


template <typename T>
void MyVector<T>::push_back(const T& obj, const std::string& name)
{
    copy_names();

    std::vector<T>::push_back(obj);
    m_names->push_back(name);
}

template <typename T>
void MyVector<T>::push_back(T&& obj, const std::string& name)
{
    copy_names();

    std::vector<T>::push_back(obj);
    m_names->push_back(name);
}


template <typename T>
std::pair<const T&, const std::string&> MyVector<T>::operator[](int index) const
{
    if (index >= std::vector<T>::size())
    {
        throw new std::out_of_range("Index is out of range");
    }

    return std::pair<const T&, const std::string&>(std::vector<T>::operator[](index), (*m_names)[index]); 
}

template <typename T>
std::pair<T&, std::string&> MyVector<T>::operator[](int index)
{
    if (index >= std::vector<T>::size())
    {
        throw new std::out_of_range("Index is out of range");
    }

    return std::pair<T&, std::string&>(std::vector<T>::operator[](index), (*m_names)[index]);
}


template <typename T>
const T& MyVector<T>::operator[](const std::string& name) const
{
    std::vector<std::string>::const_iterator iter = std::find(m_names->begin(), m_names->end(), name);
    if (iter == m_names->end())
    {
        throw new std::invalid_argument(name + " is not found in the MyVector");
    }

    return std::vector<T>::operator[](iter - m_names->begin());
}

template <typename T>
T& MyVector<T>::operator[](const std::string& name)
{
    std::vector<std::string>::const_iterator iter = std::find(m_names->begin(), m_names->end(), name);
    if (iter == m_names->end())
    {
        throw new std::invalid_argument(name + " is not found in the MyVector");
    }

    return std::vector<T>::operator[](iter - m_names->begin());
}


template <typename T>
void MyVector<T>::copy_names()
{
    if (*m_ref_ptr != 1)
    {
        (*m_ref_ptr)--;
        m_ref_ptr = new size_t(1); // Delete extra local variable
        m_names = new std::vector<std::string>(*m_names); // Delete extra local variable
    }
}

