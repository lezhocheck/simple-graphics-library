#pragma once
#include <cstdint>
#include <algorithm>
#include <cstring>
#include <stdexcept>

namespace sglib {
    template<typename T>
    class Array {
    public:
        Array() : arraySize(0) { }
        explicit Array(size_t size) : arraySize(size) {
            values = new T[size];
            memset(values, 0, sizeof(T) * arraySize);
        }

        Array(size_t size, const T& value) : arraySize(size) {
            values = new T[size];
            for (size_t i = 0; i < size; i++) {
                values[i] = value;
            }
        }

        Array(std::initializer_list<T> list) : arraySize(list.size()) {
            values = new T[list.size()];
            size_t index = 0;
            for(const auto& i : list) {
                values[index] = i;
                index++;
            }
        }

        Array(const Array& other) : arraySize(other.arraySize) {
            values = new T[other.arraySize];
            std::copy_n(other.values, other.arraySize, values);
        }

        Array(Array&& other) noexcept : arraySize(other.arraySize) {
            values = other.values;
            other.values = nullptr;
            other.arraySize = 0;
        }

        Array& operator=(const Array& other) {
            if (this == &other) {
                return *this;
            }
            values = new T[other.arraySize];
            arraySize = other.arraySize;
            std::copy_n(other.values, other.arraySize, values);
            return *this;
        }

        Array& operator=(Array&& other) noexcept {
            arraySize = other.arraySize;
            values = other.values;
            other.values = nullptr;
            other.arraySize = 0;
            return *this;
        }

        ~Array() {
            delete[] values;
        }

        const T& operator[](size_t index) const {
            if (index >= arraySize) {
                throw std::out_of_range("index was out of range");
            }
            return values[index];
        }

        T& operator[](size_t index) {
            if (index >= arraySize) {
                throw std::out_of_range("index was out of range");
            }
            return values[index];
        }

        void sort() {
            std::sort(values, values + arraySize);
        }

        size_t size() const {
            return arraySize;
        }

    private:
        T* values;
        size_t arraySize;
    };
}