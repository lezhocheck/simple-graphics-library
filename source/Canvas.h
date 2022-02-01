#pragma once
#include "Pixels.h"
#include "Bitmap.h"
#include "Point.h"
#include "LinearGradient.h"

namespace sglib {
    class Canvas {
    public:
        Canvas(size_t x, size_t y, const Color& fill = Color::white) : pixels(x, y, fill) { }
        Canvas(const Canvas& other) = delete;
        Canvas& operator=(const Canvas& other) = delete;
        void draw(const std::string& out, Bitmap::Type type = Bitmap::Type::bit24);
        Canvas& fill(const Color& color = Color::white);
        Canvas& fill(const LinearGradient& gradient, LinearGradient::Type type =
                LinearGradient::Type::LeftToRight);
        Pixels& get();
        const Pixels& get() const;
        size_t width() const;
        size_t height() const;

        Canvas& addLine(Point<float> start, Point<float> finish, const Color& color);
        Canvas& addEllipse(Point<float> lowerBound, Point<float> upperBound, const Color& color);
        Canvas& addRectangle(Point<float> lowerBound, Point<float> upperBound, const Color& color);

        Canvas& addFilledEllipse(Point<float> lowerBound, Point<float> upperBound, const Color& color);
        Canvas& addFilledEllipse(Point<float> lowerBound, Point<float> upperBound,
                                 const LinearGradient& gradient,
                                 LinearGradient::Type type = LinearGradient::Type::LeftToRight);
        Canvas& addFilledRectangle(Point<float> lowerBound, Point<float> upperBound, const Color& color);
        Canvas& addFilledRectangle(Point<float> lowerBound, Point<float> upperBound,
                                   const LinearGradient& gradient,
                                   LinearGradient::Type type = LinearGradient::Type::LeftToRight);
    private:
        Pixels pixels;
    };
}