#pragma once
#include "Color.h"
#include "Point.h"
#include <functional>

namespace sglib {
    class Pixels {
    public:
        Pixels() : imageWidth(0), imageHeight(0), colors(nullptr) { }
        Pixels(size_t width, size_t height);
        Pixels(size_t width, size_t height, const Color& color);
        Pixels(const Pixels& other);
        Pixels(Pixels&& other) noexcept;
        Pixels& operator=(const Pixels& other);
        Pixels& operator=(Pixels&& other) noexcept;
        ~Pixels();

        size_t height() const;
        size_t width() const;
        void set(int64_t x, int64_t y, const Color& color);
        void setRange(const Point<size_t>& lowerBound, const Point<size_t>& upperBound, const Color& color);
        void setRangeIf(const Point<size_t>& lowerBound,
            const Point<size_t>& upperBound,
            const std::function<bool(size_t, size_t)>& function,
            const Color& color,
            const Point<size_t>& offset = {0, 0});
        Color& get(size_t x, size_t y);
        const Color& get(size_t x, size_t y) const;
        bool empty() const;

    private:
        size_t imageWidth;
        size_t imageHeight;
        Color* colors;
    };
}