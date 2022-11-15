#include "Pixels.h"
#include <stdexcept>
#include <string>
#include <algorithm>
#include <cmath>

using namespace sglib;

Pixels::Pixels(size_t width, size_t height)  :
    imageWidth(width), imageHeight(height), colors(nullptr) {
    colors = new Color[width * height];
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            *(colors + i * height + j) = Color();
        }
    }
}

Pixels::Pixels(size_t width, size_t height, const Color& color) :
    imageWidth(width), imageHeight(height), colors(nullptr) {
    colors = new Color[width * height];
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            *(colors + i * height + j) = color;
        }
    }
}

Pixels::~Pixels() {
    delete[] colors;
}

size_t Pixels::height() const {
    return imageHeight;
}

size_t Pixels::width() const {
    return imageWidth;
}

void Pixels::set(int64_t x, int64_t y, const Color &color) {
    if (x < 0 || y < 0 || x >= imageWidth || y >= imageHeight) {
        return;
    }
    *(colors + x * imageHeight + y) = color;
}

Color& Pixels::get(size_t x, size_t y) {
    if (x >= imageWidth || y >= imageHeight) {
        throw std::invalid_argument("indexes were out of range");
    }
    return *(colors + x * imageHeight + y);
}

const Color& Pixels::get(size_t x, size_t y) const {
    if (x >= imageWidth || y >= imageHeight) {
        throw std::invalid_argument("indexes were out of range");
    }
    return *(colors + x * imageHeight + y);
}

Pixels::Pixels(const Pixels &other) :
        imageWidth(other.imageWidth),
        imageHeight(other.imageHeight) {

    colors = new Color[imageHeight * imageWidth];
    std::copy_n(other.colors, imageHeight * imageWidth, colors);
}

Pixels::Pixels(Pixels &&other) noexcept :
               imageWidth(other.imageWidth),
               imageHeight(other.imageHeight) {
    other.imageHeight = 0;
    other.imageWidth = 0;
    colors = other.colors;
    other.colors = nullptr;
}

Pixels& Pixels::operator=(Pixels &&other) noexcept {
    imageHeight = other.imageHeight;
    imageWidth = other.imageWidth;
    other.imageHeight = 0;
    other.imageWidth = 0;
    colors = other.colors;
    other.colors = nullptr;
    return *this;
}

Pixels& Pixels::operator=(const Pixels &other) {
    if (this == &other) {
        return *this;
    }
    imageHeight = other.imageHeight;
    imageWidth = other.imageWidth;
    colors = new Color[imageHeight * imageWidth];
    std::copy_n(other.colors, imageHeight * imageWidth, colors);
    return *this;
}

bool Pixels::empty() const {
    return imageWidth == 0 || imageHeight == 0;
}

void Pixels::setRangeIf(const Point<size_t>& lowerBound,
                        const Point<size_t>& upperBound,
                        const std::function<bool(size_t, size_t)>& function,
                        const Color& color,
                        const Point<size_t>& offset) {
    for (size_t i = lowerBound.x(); i < upperBound.x(); i++) {
        for (size_t j = lowerBound.y(); j < upperBound.y(); j++) {
            if (function(i, j)) {
                set(static_cast<int64_t>(i + offset.x()),
                    static_cast<int64_t>(j + offset.y()), color);
            }
        }
    }
}

void Pixels::setRange(const Point<size_t>& lowerBound,
                      const Point<size_t>& upperBound, const Color& color) {
    for (size_t i = lowerBound.x(); i < std::min(imageWidth, upperBound.x()); i++) {
        for (size_t j = lowerBound.y(); j < std::min(imageHeight, upperBound.y()); j++) {
            *(colors + i * imageHeight + j) = color;
        }
    }
}
