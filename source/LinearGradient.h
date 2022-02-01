#pragma once
#include "Array.h"
#include "Color.h"
#include "Pixels.h"
#include "Point.h"
#include "functional"

namespace sglib {
    class LinearGradient {
    public:
        enum class Type {
            LeftToRight,
            RightToLeft,
            UpToBottom,
            BottomToUp
        };
        explicit LinearGradient(const Array<Color>& colors) : colors_(colors) { }
        LinearGradient(std::initializer_list<Color> colors);

        void apply(Pixels& pixels, const Point<float>& lowerBound,
                   const Point<float>& upperBound,
                   const std::function<bool(size_t, size_t)>& function, Type type) const;

        void set(size_t index, const Color& color);
        Color& get(size_t index);
        const Color& get(size_t index) const;

    private:
        Array<Color> colors_;
    };
}