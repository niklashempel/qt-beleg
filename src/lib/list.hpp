#ifndef LIST_HPP
#define LIST_HPP

template <typename T> struct Node {
  Node *next;
  Node *previous;
  T *item;
};

template <typename T> class List {
protected:
  Node<T> *head;
  Node<T> *tail;
  int size;

public:
  List() {
    head = nullptr;
    tail = nullptr;
    size = 0;
  };
  List(const List &other) {
    head = nullptr;
    tail = nullptr;
    size = 0;
    Node<T> *current = other.head;
    while (current != nullptr) {
      add(current->item);
      current = current->next;
    }
  }
  List &operator=(const List &other) {
    if (this != &other) {
      Node<T> *current = head;
      while (current != nullptr) {
        Node<T> *next = current->next;
        delete current->item;
        delete current;
        current = next;
      }
      head = nullptr;
      tail = nullptr;
      size = 0;
      current = other.head;
      while (current != nullptr) {
        add(current->item);
        current = current->next;
      }
    }
    return *this;
  }
  ~List() {
    Node<T> *current = head;
    while (current != nullptr) {
      Node<T> *next = current->next;
      delete current->item;
      delete current;
      current = next;
    }
  };
  void add(T *item) {
    Node<T> *node = new Node<T>;
    node->item = item;
    node->next = nullptr;
    node->previous = nullptr;
    if (head == nullptr) {
      head = node;
      tail = node;
    } else {
      node->previous = tail;
      tail->next = node;
      tail = node;
    }
    size++;
  };
  void remove(const T *item) {
    Node<T> *current = head;
    while (current != nullptr) {
      if (current->item == item) {
        if (current->previous != nullptr) {
          current->previous->next = current->next;
        } else {
          head = current->next;
        }
        if (current->next != nullptr) {
          current->next->previous = current->previous;
        } else {
          tail = current->previous;
        }
        delete current->item;
        delete current;
        size--;
        return;
      }
      current = current->next;
    }
  };
  int getSize() const noexcept { return size; };
  bool isEmpty() const { return size == 0; }
  Node<T> *getHead() { return head; };
  Node<T> *getTail() { return tail; };

  struct Iterator {
    Node<T> *current;
    Iterator(Node<T> *current) : current(current){};
    Iterator &operator++() {
      current = current->next;
      return *this;
    }
    Iterator &operator--() {
      current = current->previous;
      return *this;
    }
    bool operator==(const Iterator &other) const {
      return current == other.current;
    }
    bool operator!=(const Iterator &other) const {
      return current != other.current;
    }
    T &operator*() const { return *(current->item); }
  };

  Iterator begin() { return Iterator(head); }

  Iterator end() { return Iterator(nullptr); }
};

#endif // LIST_HPP
