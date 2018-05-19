#pragma once
template <class T>
class unique_ptr
{
 public:

  unique_ptr() : payload(nullptr) {}

  unique_ptr(T * t): payload(t) {}

  T * get() { return payload; }

  void reset(T * new_ptr = nullptr)
  {
    T * old_ptr = payload;
    payload = new_ptr;
    if(old_ptr != nullptr) delete old_ptr;
  }

  T * release(T * t = nullptr)
  {
    T* tmp = payload;
    payload = t;
    return t;
  }

  bool isNull() { return payload == nullptr;}

  T* operator->() { return get();}

  ~unique_ptr(){ delete payload; }

 private:
  T * payload;
};

template <class T>
class unique_array_ptr
{
 public:

  unique_array_ptr() : payload(nullptr) {}

  unique_array_ptr(T * t): payload(t) {}

  T * get() { return payload; }

  void reset(T * new_ptr = nullptr)
  {
    T * old_ptr = payload;
    payload = new_ptr;
    if(old_ptr != nullptr) delete[] old_ptr;
  }

  T * release(T * t = nullptr)
  {
    T* tmp = payload;
    payload = t;
    return t;
  }

  bool isNull() { return payload == nullptr;}

  ~unique_array_ptr(){ delete[] payload; }

 private:
  T * payload;
};
