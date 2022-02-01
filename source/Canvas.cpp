#include "Canvas.h"
#include <stdexcept>
#include "Shape.h"

using namespace sglib;

void Canvas::draw(const std::string& out, Bitmap::Type type) {
    Bitmap* bitmap = nullptr;
    if (type == Bitmap::Type::bit24) {
        bitmap = new Bitmap24(out);
    } else if(type == Bitmap::Type::bit32) {
        bitmap = new Bitmap32(out);
    } else {
        throw std::runtime_error("unsupported bitmap type");
    }

    bitmap->write(pixels);

    delete bitmap;
}

Pixels& Canvas::get() {
    return pixels;
}

const Pixels& Canvas::get() const {
    return pixels;
}

size_t Canvas::width() const {
    return pixels.width();
}

size_t Canvas::height() const {
    return pixels.height();
}

Canvas& Canvas::addLine(Point<float> start, Point<float> finish, const Color& color) {
    Line line(*this, color, start, finish);
    return line.draw();
}

Canvas& Canvas::addEllipse(Point<float> lowerBound, Point<float> upperBound, const Color &color) {
    Ellipse ellipse(*this, color, lowerBound, upperBound);
    return ellipse.draw();
}

Canvas& Canvas::fill(const Color& color) {
    pixels.setRange({0, 0}, {pixels.width(), pixels.height()}, color);
    return *this;
}

Canvas& Canvas::fill(const LinearGradient& gradient, LinearGradient::Type type) {
    gradient.apply(pixels, {0, 0},
                   {static_cast<float>(pixels.width()),
                    static_cast<float>(pixels.height())},
                   [](size_t i, size_t j){return true;}, type);
    return *this;
}


Canvas& Canvas::addFilledEllipse(Point<float> lowerBound, Point<float> upperBound, const Color& color) {
    Ellipse ellipse(*this, color, lowerBound, upperBound);
    return ellipse.fill();
}

Canvas& Canvas::addRectangle(Point<float> lowerBound, Point<float> upperBound, const Color& color) {
    Rectangle rectangle(*this, color, lowerBound, upperBound);
    return rectangle.draw();
}

Canvas& Canvas::addFilledRectangle(Point<float> lowerBound, Point<float> upperBound, const Color& color) {
    Rectangle rectangle(*this, color, lowerBound, upperBound);
    return rectangle.fill();
}

Canvas& Canvas::addFilledRectangle(Point<float> lowerBound, Point<float> upperBound,
                                   const LinearGradient& gradient, LinearGradient::Type type) {
    Rectangle rectangle(*this, Color(Color::Rgb(255, 255, 255)), lowerBound, upperBound);
    return rectangle.fill(gradient, type);
}

Canvas& Canvas::addFilledEllipse(Point<float> lowerBound, Point<float> upperBound, const LinearGradient& gradient,
                                 LinearGradient::Type type) {
    Ellipse ellipse(*this, Color(Color::Rgb(255, 255, 255)), lowerBound, upperBound);
    return ellipse.fill(gradient, type);
}
