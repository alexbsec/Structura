#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include "array.hpp"
#include "linked-list.hpp"
#include <functional>

namespace structura {

template <typename T, typename P>
class HashNode : public INode<P> {
  public:
    T key;
    P value;
    INode<P>* next;

    HashNode(T key, P val) : key(key), value(val), next(nullptr) {}
};

template <typename T, typename P, typename HashFn = std::hash<T>>
class HashTable {
  HashFn hash_fn_;

  private:
    int current_size_;
    int total_entries_;
    HashNode<T, P>** data_map;

    int Hash(T key, int size) {
      return hash_fn_(key) % size;
    }

    void Resize() {
      int new_size = current_size_ * 2;
      HashNode<T, P>** new_data_map = new HashNode<T, P>*[new_size];

      for (int i = 0; i < new_size; i++) {
        new_data_map[i] = nullptr;
      }

      for (int i = 0; i < current_size_; i++) {
        HashNode<T, P>* temp = data_map[i];
        while (temp) {
          int new_index = Hash(temp->key, new_size);
          HashNode<T, P>* next_node = temp->next;

          // Rehash the nodes
          temp->next = new_data_map[new_index];
          new_data_map[new_index] = temp;

          temp = next_node;
        }
      }

      delete[] data_map;
      data_map = new_data_map;
      current_size_ = new_size;
    }

  public:
    HashTable() : current_size_(7), total_entries_(0) {
      data_map = new HashNode<T, P>*[current_size_];
      for (int i = 0; i < current_size_; i++) {
        data_map[i] = nullptr;
      }
    }

    ~HashTable() {
      for (int i = 0; i < current_size_; i++) {
        HashNode<T, P>* temp = data_map[i];
        while (temp) {
          HashNode<T, P>* to_delete = temp;
          temp = temp->next;
          delete to_delete;
        }
      }

      delete[] data_map;
    }

    void Set(T key, P value) {
      int index = Hash(key, current_size_);
      HashNode<T, P>* temp = data_map[index];

      while (temp) {
        if (temp->key == key) {
          temp->value = value;
          return;
        }

        temp = temp->next;
      }

      HashNode<T, P>* node = new HashNode<T, P>(key, value);
      node->next = data_map[index];
      data_map[index] = node;
      total_entries_++;

      if ((float)total_entries_ / current_size_ > 0.75) {
        Resize();
      }
    }

    bool Find(T key) {
      int index = Hash(key, current_size_);
      HashNode<T, P>* temp = data_map[index];

      while (temp) {
        if (temp->key == key) return true;
        temp = temp->next;
      }

      return false;
    }

    bool Remove(T key) {
      int index = Hash(key, current_size_);
      HashNode<T, P>* temp = data_map[index];
      HashNode<T, P>* prev = nullptr;

      while (temp) {
        if (temp->key == key) {
          if (prev) {
            prev->next = temp->next;
          } else {
            data_map[index] = temp->next;
          }

          delete temp;
          total_entries_--;
          return true;
        }
        prev = temp;
        temp = temp->next;
      }

      return false;
    }

    P& Get(T key) {
      int index = Hash(key, current_size_);
      HashNode<T, P>* temp = data_map[index];
      while (temp) {
        if (temp->key == key) return temp->value;
        temp = temp->next;
      }

      throw std::runtime_error("Key not found");
    }

    const P& Get(T key) const {
      int index = Hash(key, current_size_);
      HashNode<T, P>* temp = data_map[index];

      while (temp) {
        if (temp->key == key) return temp->value;
        temp = temp->next;
      }

      throw std::runtime_error("Key not found");
    }

    Array<T> Keys() {
      Array<T> all_keys;
      for (int i = 0; i < current_size_; i++) {
        HashNode<T, P>* temp = data_map[i];
        while (temp) {
          all_keys.Push(temp->key);
          temp = temp->next;
        }
      }

      return all_keys;
    }

    P& operator[](T key) {
      try {
        return Get(key);
      } catch (const std::runtime_error& e) {
        // Key does not exist, so insert new node
        int index = Hash(key, current_size_);
        HashNode<T, P>* node = new HashNode<T, P>(key, P());
        node->next = data_map[index];
        data_map[index] = node;
        total_entries_++;

        if ((float)total_entries_ / current_size_ > 0.75) {
          Resize();
        }

        return node->value;
      }
    }
};

}

#endif
