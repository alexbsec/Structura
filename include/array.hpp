#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <stdexcept>

namespace structura {
template<typename T>
class Array {
  private:
    T* data_;       // Pointer to the data in the array
    int size_;      // Current size of the array
    int capacity_;  // Capacity of the array

    /* Private method to resize the array when size
     * becomes the same of capacity
     * Takes an integer value to represent new capacity
     * */
    void Resize(int capacity) {
      // Create new data pointer with the new capacity
      T* new_data = new T[capacity];
      // Copy the values of olde array into this new data
      for (int i = 0; i < size_; i++) {
        new_data[i] = data_[i];
      }
      // Deletes old data and assign necessary variables
      delete[] data_;
      data_ = new_data;
      capacity_ = capacity;
    }

  public:
    /* Default constructor of the class
     * Starts the class with size of 0 and capacity of 10 elements
     * */
    Array() : size_(0), capacity_(10) {
      data_ = new T[capacity_];
    }

    /* Constructor of the class specifying the capacity size
     * as direct int input
     * */
    Array(int capacity) : size_(0), capacity_(capacity) {
      data_ = new T[capacity_];
    }

    /* Constructor that assigns an existant array as input
     * for the newly constructed array
     * */
    Array(const Array& other) : size_(other.size_), capacity_(other.capacity_) {
      data_ = new T[capacity_];
      // Copies the value of the array into the new array
      for (int i = 0; i < size_; i++) {
        data_[i] = other.data_[i];
      }
    }

    /* Constructor that makes a deep copy of an existant array
     * carefully deleting old array in the process
     * */
    Array(const Array&& other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
      other.data_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    }

    /* Simple destructor
     * */
    ~Array() {
      delete[] data_;
    }

    /* Member function to append elements at the end of
     * the array by their type
     * */
    void Push(const T& value) {
      // Checks if size will exceed capacity in this operation
      if (size_ == capacity_) {
        // Resize to make the new element fit into the array
        Resize(capacity_ * 2);
      }

      // Inserts the new value into the last element of the array while
      // incrementing the size
      data_[size_++] = value;
    }

    /* Member function to remove element at a given index
     * of the array
     * */
    void Remove(int index) {
      // Checks whether passed index is within array bounds
      if (index >= size_ || index < 0) {
        // Throw error if not
        throw std::out_of_range("Index out of bounds");
      }

      // Shifts all elements on the right of index to the left
      for (int i = index; i < size_ - 1; i++) {
        data_[i] = data_[i + 1];
      }

      // Decrement size as we removed one element
      size_--;
    }

    /* Member function to insert an element at a given
     * index of the array
     * */
    void Insert(int index, const T& value) {
      if (index > size_ || index < 0) {
        throw std::out_of_range("Index out of bounds");
      }

      // Capacity check
      if (size_ == capacity_) Resize(capacity_ * 2);

      // Shifts all elements after index to the right by one
      for (int i = size_; i > index; i--) {
        data_[i] = data_[i - 1];
      }

      // Assign the new element at index and increment size
      data_[index] = value;
      size_++;
    }

    /* Member function to insert an array into another array
     * at a given index
     * */
    template<typename InputIt>
    void Insert(int index, InputIt first, InputIt last) {
      if (index > size_ || index < 0) {
        throw std::out_of_range("Index out of bounds");
      }

      // Calculates the size of the array to be inserted
      int range_size = std::distance(first, last);

      // Capacity check
      if (size_ + range_size > capacity_) Resize(capacity_ + range_size + 1);

      // Shift elements to the right
      for (int i = size_ - 1; i >= index; i--) {
        data_[i + range_size] = data_[i];
      }

      // Insert new elements
      for (int i = 0; i < range_size; i++) {
        data_[index + i] = *first++;
      }

      // Increase original array size
      size_ += range_size;
    }

    /* Member function to erase elements within
     * a range of indices
     * */
    void Erase(int first, int last) {
      if (first < 0 || last > size_ || first >= last) {
        throw std::out_of_range("Invalid range");
      }

      // Calculates the range size
      int range_size = last - first;

      // Shift elements to the left
      for (int i = last; i < size_; i++) {
        data_[i - range_size] = data_[i];
      }

      // Decrease size of the array
      size_ -= range_size;
    }

    /* Member function to invert the elements
     * of the array
     * */
    void Invert() {
      // Two pointers approach
      if (size_ == 0) return;
      int start = 0, end = size_ - 1;
      while (start < end) {
        // Make a copy of starting value
        T temp = data_[start];
        data_[start] = data_[end];
        data_[end] = temp;
        // Increment and decrement pointers
        start++;
        end--;
      }
    }

    /* Member function to shrink the capacity to
     * current size of the array
     * */
    void Shrink() {
      if (size_ == 0)
        Resize(1);
      else
        Resize(size_);
    }

    /* Member function to find an element in the array
     * the function returns the index if it finds the element
     * or -1 if it does not find
     * */
    int Find(const T& value) const {
      for (int i = 0; i < size_; i++) {
        if (data_[i] == value) return i;
      }

      return -1;
    }

    /* Member function to check if element exists
     * in the array
     * */
    bool Contains(const T& value) {
      int index = Find(value);
      return index != -1;
    }

    /* Member function to clear the array
     * */
    void Clear() {
      size_ = 0;
    }

    /* Member function to remove last element of
     * the array
     * */
    void Pop() {
      if (size_ > 0) {
        size_--;
      } else {
        throw std::out_of_range("Array is empty");
      }
    }

    /* Assignment operator = to make an array equals another
     * */
    Array& operator=(const Array& other) {
      if (this != &other) {
        delete[] data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = new T[capacity_];
        for (int i = 0; i < size_; i++) {
          data_[i] = other.data_[i];
        }
      }

      return *this;
    }

    /* Assignment operator = to make the array equals another
     * while deleting the other reference
     * */
    Array& operator=(const Array&& other) {
      if (this != &other) {
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
      }

      return *this;
    }

    /* Comparison operator == to check whether an array
     * equals another array
     * */
    bool operator==(const Array& other) const {
      if (size_ != other.size_) return false;
      for (int i = 0; i < size_; i++) {
        if (data_[i] != other.data_[i]) return false;
      }

      return true;
    }

    /* Comparison != operator to check whether an array
     * is not equal another array
     * */
    bool operator!=(const Array& other) const {
      return !(*this == other);
    }

    /* Operator to access element of the array using brackets
     * like arr[element]
     * */
    T& operator[](int index) {
      if (index >= size_ || index < 0) {
        throw std::out_of_range("Index out of bounds");
      }

      return data_[index];
    }

    /* Const version of brackets operator
     * */
    const T& operator[](int index) const {
      if (index >= size_ || index < 0) {
        throw std::out_of_range("Index out of bounds");
      }

      return data_[index];
    }

    /* Begin iterator
     * */
    T* Begin() {
      return data_;
    }

    /* End iterator
     * */
    T* End() {
      return data_ + size_;
    }

    const T* Begin() const {
      return data_;
    }

    const T* End() const {
      return data_ + size_;
    }

    /* Member function to get the current size of the array
     * */
    int Size() const {
      return size_;
    }

    /* Member function to get the capacity of the array
     * */
    int Capacity() const {
      return capacity_;
    }

    /* Member function to check whether array is empty
     * or not
     * */
    bool Empty() const {
      return size_ == 0;
    }
};
}

#endif  // ARRAY_HPP
