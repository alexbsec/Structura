#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <stdexcept>

namespace structura {

template <typename T>
class INode {
  protected:
    INode() {}

  public:
    virtual ~INode() {}
    virtual T Data() const = 0;
    virtual void Set(const T& data) = 0;

    virtual INode<T>* Next() const = 0;
    virtual void SetNext(INode<T>* next) = 0;
};

template <typename T>
class SinglyNode : public INode<T> {
  private:
    T data_;
    INode<T>* next_;

  public:
    SinglyNode(const T& data) : data_(data), next_(nullptr) {}

    T Data() const override {
      return data_;
    }

    void Set(const T& data) override {
      data_ = data;
    }

    INode<T>* Next() const override {
      return next_;
    }

    void SetNext(INode<T>* next) override {
      next_ = next;
    }
};

template <typename T>
class DoublyNode : public INode<T> {
  private:
    T data_;
    INode<T>* next_;
    INode<T>* prev_;

  public:
    DoublyNode(const T& data) : data_(data), next_(nullptr), prev_(nullptr) {}

    T Data() const override {
      return data_;
    }

    void Set(const T& data) override {
      data_ = data;
    }

    INode<T>* Next() const override {
      return next_;
    }

    INode<T>* Prev() const {
      return prev_;
    }

    void SetNext(INode<T>* next) override {
      next_ = next;
    }

    void SetPrev(INode<T>* prev) {
      prev_ = prev;
    }
};

template <typename T>
class SinglyList {
  private:
    SinglyNode<T>* head_;
    SinglyNode<T>* tail_;
    int size_;

    bool IsOutOfBounds(int index) {
      if (index < 0 || index > size_ - 1) return true;
      return false;
    }

  public:
    SinglyList() : head_(nullptr), tail_(nullptr), size_(0) {}
    ~SinglyList();

    void InsertFront(const T& data) {
      SinglyNode<T>* node = new SinglyNode<T>(data);
      if (head_ == nullptr) {
        head_ = node;
        tail_ = node;
        size_++;
        return;
      }

      node->SetNext(head_);
      head_ = node;
      size_++;
    }

    void InsertBack(const T& data) {
      SinglyNode<T>* node = new SinglyNode<T>(data);
      if (tail_ == nullptr) {
        head_ = node;
        tail_ = node;
        size_++;
        return;
      }

      tail_->SetNext(node);
      tail_ = node;
      size_++;
    }

    void InsertAt(int index, const T& data) {
      if (index < 0 || index > size_) {
        throw std::out_of_range("Index out of bounds");
      }

      if (index == 0) {
        InsertFront(data);
        return;
      }

      if (index == size_) {
        InsertBack(data);
        return;
      }

      SinglyNode<T>* temp = head_;
      SinglyNode<T>* prev = nullptr;
      for (int counter = 0; counter < index; counter++) {
        prev = temp;
        temp = temp->Next();
      }

      SinglyNode<T>* node = new SinglyNode<T>(data);
      node->SetNext(temp);
      prev->SetNext(node);
      size_++;
    }


    void Invert() {
      if (Empty() || head_ == tail_) return;

      SinglyNode<T>* prev = nullptr;
      SinglyNode<T>* curr = head_;
      SinglyNode<T>* next = nullptr;
      tail_ = curr;

      while (curr != nullptr) {
        next = curr->Next();
        curr->SetNext(prev);
        prev = curr;
        curr = next;
      }

      head_ = prev;
    }

    void PopFront() {
      if (Empty()) {
        throw std::out_of_range("Cannot remove element from empty list");
      }

      if (head_ == tail_) {
        delete head_;
        head_ = tail_ = nullptr;
        size_--;
        return;
      }

      SinglyNode<T>* to_delete = head_;
      head_ = head_->Next();
      delete to_delete;
      size_--;
    }

    void PopBack() {
      if (Empty()) {
        throw std::out_of_range("Cannot remove element from empty list");
      }

      if (head_ == tail_) {
        delete head_;
        head_ = tail_ = nullptr;
        size_--;
        return;
      }

      SinglyNode<T>* to_delete = tail_;
      SinglyNode<T>* new_tail = head_;

      while (new_tail->Next() != tail_) {
        new_tail = new_tail->Next();
      }

      tail_ = new_tail;
      tail_->SetNext(nullptr);
      delete to_delete;
      size_--;
    }

    void PopAt(int index) {
      if (IsOutOfBounds(index)) throw std::out_of_range("Index out of bounds");

      if (index == 0) {
        PopFront();
        return;
      }

      if (index == size_ - 1) {
        PopBack();
        return;
      }

      SinglyNode<T>* prev = nullptr;
      SinglyNode<T>* curr = head_;
      for (int counter = 0; counter < index; counter++) {
        prev = curr;
        curr = curr->Next();
      }

      prev->SetNext(curr->Next());
      delete curr;
      size_--;
    }

    void PopMiddle() {
      if (Empty()) {
        throw std::out_of_range("Cannot remove element from empty list");
      }

      if (size_ == 1) {
        PopFront();
        return;
      }

      SinglyNode<T>* fast = head_;
      SinglyNode<T>* slow = head_;
      SinglyNode<T>* prev = nullptr;
      
      while (fast && fast->Next()) {
        fast = fast->Next()->Next();
        prev = slow;
        slow = slow->Next();
      }

      prev->SetNext(slow->Next());
      delete slow;
      size_--;
    }

    int Size() const {
      return size_;
    }

    T At(int index) const {
      if (IsOutOfBounds(index)) throw std::out_of_range("Index out of bounds");
      SinglyNode<T>* temp = head_;
      for (int counter = 0; counter < index; counter++) {
        temp = temp->Next();
      }

      return temp->Data();
    }

    bool Empty() const {
      return size_ == 0;
    }

    void Clear() {
      SinglyNode<T>* temp = head_;
      while (temp) {
        SinglyNode<T>* next = temp->Next();
        delete temp;
        temp = next;
      }

      head_ = nullptr;
      tail_ = nullptr;
      size_ = 0;
    }

    const T& Head() const {
      if (Empty()) throw std::runtime_error("List is empty.");
      return head_->Data();
    }

    const T& Tail() const {
      if (Empty()) throw std::runtime_error("List is empty.");
      return tail_->Data();
    }

};

template <typename T>
class DoublyList {};

}

#endif
