#include "LinearGradient.h"
#include "stdexcept"
#include <cmath>

using namespace sglib;

void LinearGradient::set(size_t index, const Color& color) {
    colors_[index] = color;
}

Color& LinearGradient::get(size_t index) {
    return colors_[index];
}

const Color& LinearGradient::get(size_t index) const {
    return colors_[index];
}

void LinearGradient::apply(Pixels& pixels,
                           const Point<float>& lowerBound,
                           const Point<float>& upperBound,
                           const std::function<bool(size_t, size_t)>& function,
                           Type type) const {

    float length = upperBound.x() - lowerBound.x();

    const Point<size_t> lowerBoundSizeT = {static_cast<size_t>(lowerBound.x()),
                                  static_cast<size_t>(lowerBound.y())};
    const Point<size_t> upperBoundSizeT = {static_cast<size_t>(upperBound.x()),
                                           static_cast<size_t>(upperBound.y())};

    if (type == Type::UpToBottom || type == Type::BottomToUp) {
        length = upperBound.y() - lowerBound.y();
    } else if(type != Type::LeftToRight && type != Type::RightToLeft) {
        throw std::invalid_argument("unsupported gradient type");
    }
    auto lengthSizeT = static_cast<size_t>(length);
    const float difference = length / static_cast<float>(colors_.size() - 1);
    const auto repeats = static_cast<size_t>(difference);

    for (size_t i = 0; i + 1 < colors_.size(); i++) {
        Color start = colors_[i];
        Color finish = colors_[i + 1];
        const float rStep = static_cast<float>(std::abs(finish.r() -
                static_cast<int>(start.r()))) / difference;
        const float gStep = static_cast<float>(std::abs(finish.g() -
                static_cast<int>(start.g()))) / difference;
        const float bStep = static_cast<float>(std::abs(finish.b() -
                static_cast<int>(start.b()))) / difference;
        float r = start.r();
        float g = start.g();
        float b = start.b();
        for(size_t j = 0; j < repeats; j++) {
            Color next(Color::Rgb{static_cast<uint8_t>(roundf(r)),
                                  static_cast<uint8_t>(roundf(g)),
                                  static_cast<uint8_t>(roundf(b))});

            if (type == Type::LeftToRight) {
                pixels.setRangeIf({i * repeats + j, 0},
                                  {i * repeats + j + 1, upperBoundSizeT.y()},
                                  function, next, lowerBoundSizeT);
            } else if (type == Type::RightToLeft) {
                pixels.setRangeIf({lengthSizeT - i * repeats - j - 1, 0},
                                {lengthSizeT - i * repeats - j, upperBoundSizeT.y()},
                                function, next, lowerBoundSizeT);
            } else if (type == Type::UpToBottom) {
                pixels.setRangeIf({0, lengthSizeT - i * repeats - j - 1},
                                {upperBoundSizeT.x(), lengthSizeT - i * repeats - j},
                                function, next, lowerBoundSizeT);
            } else {
                pixels.setRangeIf({0, i * repeats + j},
                                {upperBoundSizeT.x(), i * repeats + j + 1},
                                function, next, lowerBoundSizeT);
            }

            if (start.r() < finish.r()) {
                r += rStep;
            } else {
                r -= rStep;
            }

            if (start.g() < finish.g()) {
                g += gStep;
            } else {
                g -= gStep;
            }

            if (start.b() < finish.b()) {
                b += bStep;
            } else {
                b -= bStep;
            }
        }
    }
}

LinearGradient::LinearGradient(std::initializer_list<Color> colors) : colors_(colors.size()) {
    size_t index = 0;
    for(const auto& i : colors) {
        colors_[index] = i;
        index++;
    }
}