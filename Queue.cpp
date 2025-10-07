/*
3_3. Реализовать очередь с помощью двух стеков.
Требования: Очередь должна быть реализована в виде класса. Стек тоже должен быть реализован в виде класса (на основе динамического массива).
in       out
3        YES
3 44
3 50
2 44

2        YES
2 -1
3 10

2        NO
3 44
2 66
*/


#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>

class Stack {
    private:
        int top;
        int capacity;
        int* stack;

        void Resize() {
            int newCapacity = capacity * 2;
            int* newStack = new int[newCapacity];
            for (int i = 0; i <= top; ++i) {
                newStack[i] = stack[i];
            }
            capacity = newCapacity;
            delete[] stack;
            stack = newStack;
        }

    public:
        Stack(int capacity = 5) : capacity(capacity), top(-1) { stack = new int[capacity]; }

        ~Stack() { delete[] stack; }

        Stack& operator=(const Stack& other) {
            if (this != &other) {
                delete[] stack;
                capacity = other.capacity;
                top = other.top;
                stack = new int[capacity];
                std::copy(other.stack, other.stack + capacity, stack);
            }
            return *this;
        }

        Stack(const Stack& other) : capacity(other.capacity), top(other.top) {
            stack = new int[capacity];
            std::copy(other.stack, other.stack + capacity, stack);
        }

        int GetSize() const { return top + 1; }

        bool IsFull() const { return top + 1 == capacity; }

        bool IsEmpty() const { return top == -1; }

        void Push(int element) {
            if (IsFull()) {
                Resize();
            }
            stack[++top] = element;
        }

        int Pop() {
            if (IsEmpty()) {
                throw std::runtime_error("Stack is empty");
            }
            return stack[top--];
        }

        int Peek() const {
            if (IsEmpty()) {
                throw std::runtime_error("Stack is empty");
            }
            return stack[top];
        }
};

class Queue {
    private:
        Stack inputStack;
        Stack outputStack;

        void TransferElements() {
            while (!inputStack.IsEmpty()) {
                outputStack.Push(inputStack.Pop());
            }
        }

    public:
        Queue(int capacity = 5) : inputStack(capacity), outputStack(capacity) {}

        void Enqueue(int element) { inputStack.Push(element); }

        int Dequeue() {
            if (IsEmpty()) {
                return -1;
            }

            if (outputStack.IsEmpty()) {
                TransferElements();
            }

            return outputStack.Pop();
        }

        bool IsEmpty() const { return inputStack.IsEmpty() && outputStack.IsEmpty(); }

        int GetSize() const { return inputStack.GetSize() + outputStack.GetSize(); }
};

void TestQueue() {
    Queue queue;

    // Тест 1: Добавление и извлечение элементов
    std::cout << "Test 1: Basic operations" << std::endl;
    queue.Enqueue(1);
    queue.Enqueue(2);
    queue.Enqueue(3);

    assert(queue.Dequeue() == 1);
    assert(queue.Dequeue() == 2);
    assert(queue.Dequeue() == 3);
    assert(queue.IsEmpty());

    // Тест 2: Чередование операций
    std::cout << "Test 2: Mixed operations" << std::endl;
    queue.Enqueue(10);
    queue.Enqueue(20);
    assert(queue.Dequeue() == 10);
    queue.Enqueue(30);
    queue.Enqueue(40);
    assert(queue.Dequeue() == 20);
    assert(queue.Dequeue() == 30);
    assert(queue.Dequeue() == 40);
    assert(queue.IsEmpty());

    // Тест 3: Размер очереди
    std::cout << "Test 3: Size operations" << std::endl;
    queue.Enqueue(100);
    queue.Enqueue(200);
    assert(queue.GetSize() == 2);
    queue.Dequeue();
    assert(queue.GetSize() == 1);
    queue.Dequeue();
    assert(queue.GetSize() == 0);
}

int Run() {
    Queue queue;

    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        int operation = 0;
        int element = 0;
        std::cin >> operation >> element;

        switch (operation) {
            case 3: {
                queue.Enqueue(element);
                break;
            }

            case 2: {
                int temp = queue.Dequeue();
                if (temp != element) {
                    std::cout << "NO" << std::endl;
                    return 0;
                }
            }
        }
    }
    std::cout << "YES" << std::endl;
    return 0;
}

int main() {
    // TestQueue();
    int ok = Run();
    return 0;
}
