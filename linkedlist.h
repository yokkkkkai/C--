/*
List<T> - Шаблонный класс односвязного списка

Конструкторы:
- List() - пустой список
- List(initializer_list<T>) - из списка инициализации
- List(size_t, T*) - из массива заданной длины
- List(const List&) - конструктор копирования
- List(const vector<T>&) - из вектора
- List(List&&) - конструктор перемещения

Операторы:
- operator= - присваивание
- operator+ - конкатенация списков
- operator[] - доступ по индексу

Методы доступа:
- get(index) - получение элемента по индексу
- headNode() - получение первого узла
- tailNode() - получение последнего узла

Методы модификации:
- append(value) - добавление в конец
- append(value, index) - вставка по индексу
- update(value, index) - обновление значения по индексу
- remove(value) - удаление первого найденного значения
- remove(index) - удаление по индексу
- swap(index1, index2) - обмен элементов по индексам
- clear() - очистка списка
- toVector() - преобразование в std::vector(возвращает его)
- toArray() - преобразование в указатель на массив(возвращает его)
- reverse() - переворачивает список

Методы запросов:
- find(value) - поиск узла по значению
- count(value) - подсчет количества вхождений значения
- len() - получение длины списка

Вывод:
- print() - вывод всего списка
- print(start, end) - вывод части списка

Обработка ошибок:
- Выбрасывает out_of_range при неверных индексах
- Проверяет корректность всех операций с памятью
*/
#define COMP
#ifdef COMP
#include <iostream>
#include <stdexcept>
#include <cstddef>
#include <vector>
#include <memory>
#endif

template <typename T>
class Node {
public:
    T data;
    Node* next;
    Node(const T& d) : data(d), next(nullptr) {}
};

template <typename T>
class List {
private:
    Node<T>* head;
    size_t size;

public:
    // Constructors & Destructor
    List() : head(nullptr), size(0) {}

    List(std::initializer_list<T> init) : head(nullptr), size(0) {
        for (const auto& value : init) {
            append(value);
        }
    }

    List(size_t count, T* data) : head(nullptr), size(0) {
        for (size_t i = 0; i < count; ++i) {
            append(data[i]);
        }
    }

    List(const List& other) : head(nullptr), size(0) {
        Node<T>* current = other.head;
        while (current != nullptr) {
            append(current->data);
            current = current->next;
        }
    }

    List(const std::vector<T>& other) : head(nullptr), size(0) {
        if (other.empty()) return;

        head = new Node<T>(other[0]);
        Node<T>* current = head;

        for (size_t i = 1; i < other.size(); ++i) {
            current->next = new Node<T>(other[i]);
            current = current->next;
        }
        size = other.size();
    }

    List(List&& other) noexcept : head(other.head), size(other.size) {
        other.head = nullptr;
        other.size = 0;
    }

    ~List() {
        clear();
    }

    // Operators
    List& operator=(const List& other) {
        if (this != &other) {
            clear();
            Node<T>* current = other.head;
            while (current != nullptr) {
                append(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    List<T> operator+(const List<T>& other) const {
        List<T> result(*this);
        Node<T>* current = other.head;
        while (current != nullptr) {
            result.append(current->data);
            current = current->next;
        }
        return result;
    }

    T& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        Node<T>* current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }

    // Element Access
    T get(size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Invalid index");
        }
        Node<T>* current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }

    Node<T>* headNode() const {
        return head;
    }

    Node<T>* tailNode() const {
        if (head == nullptr) return nullptr;
        Node<T>* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        return current;
    }

    // Modification Methods
    void append(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (head == nullptr) {
            head = newNode;
        }
        else {
            Node<T>* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        ++size;
    }

    void append(const T& value, int index) {
        if (index < -1 || index > static_cast<int>(size)) {
            throw std::out_of_range("Invalid index");
        }

        Node<T>* newNode = new Node<T>(value);

        if (index == 0) {
            newNode->next = head;
            head = newNode;
        }
        else {
            Node<T>* current = head;
            for (int i = 0; i < index - 1; ++i) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
        ++size;
    }

    void update(const T& value, size_t index) {
        if (index >= size) {
            throw std::out_of_range("Invalid index");
        }
        Node<T>* current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        current->data = value;
    }

    void remove(const T& value) {
        if (head == nullptr) return;

        if (head->data == value) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            --size;
            return;
        }

        Node<T>* current = head;
        while (current->next != nullptr) {
            if (current->next->data == value) {
                Node<T>* temp = current->next;
                current->next = current->next->next;
                delete temp;
                --size;
                return;
            }
            current = current->next;
        }
    }

    void remove(size_t index) {
        if (index >= size) {
            throw std::out_of_range("Invalid index");
        }

        if (index == 0) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        else {
            Node<T>* current = head;
            for (size_t i = 0; i < index - 1; ++i) {
                current = current->next;
            }
            Node<T>* temp = current->next;
            current->next = temp->next;
            delete temp;
        }
        --size;
    }
    
    void swap(size_t j1, size_t j2) {
        if (j1 >= size || j2 >= size) {
            throw std::out_of_range("Invalid indices");
        }
        
        if (j1 == j2) {
            return;
        }
        
        Node<T>* node1 = head;
        for (size_t i = 0; i < j1; ++i) {
            node1 = node1->next;
        }
        
        Node<T>* node2 = head;
        for (size_t i = 0; i < j2; ++i) {
            node2 = node2->next;
        }
        
        T temp = node1->data;
        node1->data = node2->data;
        node2->data = temp;
    }        

    void clear() {
        while (head != nullptr) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        size = 0;
    }
    
    std::vector<T> toVector() const {
        std::vector<T> arr;

        if(head == nullptr) return arr;

        Node<T>* current = head;

        while(current != nullptr) {
            arr.push_back(current->data);
            current = current->next;
        }

        return arr;
    }

   T* toArray() const {
       if(head == nullptr) return nullptr;

        T* newArr = new T[size];
        Node<T>* current = head;
        int index = 0;

        while(current != nullptr) {
            newArr[index++] = current->data;
            current = current->next;
        }

        return newArr;
    }

    void reverse() {
        Node<T>* prev = nullptr;
        Node<T>* current = head;
        Node<T>* next = nullptr;

        while (current != nullptr) {
            next = current->next; 
            current->next = prev;
            prev = current;
            current = next; 
        }
        head = prev; 
    }

    // Query Methods
    Node<T>* find(const T& value) const {
        Node<T>* current = head;
        while (current != nullptr) {
            if (current->data == value) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    long long count(const T& value) const {
        long long cnt = 0;
        Node<T>* current = head;
        while (current != nullptr) {
            if (current->data == value) {
                ++cnt;
            }
            current = current->next;
        }
        return cnt;
    }

    size_t len() const {
        return size;
    }

    bool isEmpty() const {
        if(size == 0 || head == nullptr) return true;
        return false;
    }

    // Output Methods
    void print() const {
        Node<T>* current = head;
        while (current != nullptr) {
            std::cout << current->data << ' ';
            current = current->next;
        }
        std::cout << std::endl;
    }

    void print(size_t start, size_t end) const {
        if (start >= size || end > size || start > end) {
            throw std::out_of_range("Invalid range");
        }

        Node<T>* current = head;
        for (size_t i = 0; i < start; ++i) {
            current = current->next;
        }

        for (size_t i = start; i < end; ++i) {
            std::cout << current->data << ' ';
            current = current->next;
        }
        std::cout << std::endl;
    }
};