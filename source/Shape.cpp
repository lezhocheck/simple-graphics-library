#include "Shape.h"
#include <stdexcept>
#include <cmath>
#include <random>

using namespace sglib;

Canvas& Line::draw() {
    drawLine(start_, finish_);
    return canvas_;
}

Line Line::rotate(Point<float> origin, int angle) {
    Point<float> startCorrected = start_;
    Point<float> finishCorrected = finish_;
    finishCorrected.rotate(origin, angle);
    startCorrected.rotate(origin, angle);
    return Line(canvas_, color_, startCorrected, finishCorrected);
}

Line Line::mirror(const Line& other) {
    Point<float> startCorrected = start_;
    Point<float> finishCorrected = finish_;
    startCorrected.mirror(other.start_, other.finish_);
    finishCorrected.mirror(other.start_, other.finish_);
    return Line(canvas_, color_, startCorrected, finishCorrected);
}

void Shape::drawLine(Point<float> start, Point<float> end) {
    Pixels& pixels = canvas_.get();
    float k, b;
    if (start.x() != end.x()) {
        k = (end.y() - start.y()) / (end.x() - start.x());
    }
    if (start.x() != end.x() && std::abs(k) <= 1.0f) {
        if (start.x() > end.x()) {
            std::swap(start.x(), end.x());
            std::swap(start.y(), end.y());
        }
        b = start.y() - k * start.x();
        for (auto i = static_cast<int64_t>(start.x()); i <= static_cast<int64_t>(end.x()); i++) {
            pixels.set(i,static_cast<int64_t>(roundf(k * static_cast<float>(i) + b)), color_);
        }

    } else {
        if (start.y() > end.y()) {
            std::swap(start.x(), end.x());
            std::swap(start.y(), end.y());
        }
        k = (end.x() - start.x()) / (end.y() - start.y());
        b = start.x() - k * start.y();
        for (auto i = static_cast<int64_t>(start.y()); i <= static_cast<int64_t>(end.y()); i++) {
            pixels.set(static_cast<int64_t>(roundf(k * static_cast<float>(i) + b)), i, color_);
        }
    }
}

Canvas& Ellipse::draw() {
    lowerBound_.swap(upperBound_);

    const float width = std::abs(upperBound_.x() - lowerBound_.x());
    const float height = std::abs(upperBound_.y() - lowerBound_.y());
    if (width == 0.0f || height == 0.0f) {
        return canvas_;
    }
    const Point<float> start(width / 2.0f + lowerBound_.x(), height + lowerBound_.y());
    const Array<Point<float>> upperRight = generatePointsOnEllipseSegment();
    const Array<Point<float>> upperLeft = mirrorEllipseSegment(upperRight, start, {start.x(), start.y() + 100});
    const Array<Point<float>> lowerRight = mirrorEllipseSegment(upperRight, start, {start.x() + 100, start.y()});
    const Array<Point<float>> lowerLeft = mirrorEllipseSegment(lowerRight, start, {start.x(), start.y() + 100});
    for (size_t i = 0; i + 1 < upperRight.size(); i++) {
        drawLine(upperRight[i], upperRight[i + 1]);
        drawLine(upperLeft[i], upperLeft[i + 1]);
        drawLine({lowerRight[i].x(), lowerRight[i].y() - height},
                 {lowerRight[i + 1].x(), lowerRight[i + 1].y() - height});
        drawLine({lowerLeft[i].x(), lowerLeft[i].y() - height},
                 {lowerLeft[i + 1].x(), lowerLeft[i + 1].y() - height});
    }

    return canvas_;
}

Canvas& Ellipse::fill() {
    Pixels& pixels = canvas_.get();
    lowerBound_.swap(upperBound_);

    const auto width = static_cast<size_t>(std::abs(upperBound_.x() - lowerBound_.x()));
    const auto height = static_cast<size_t>(std::abs(upperBound_.y() - lowerBound_.y()));
    const auto widthF = static_cast<float>(width);
    const auto heightF = static_cast<float>(height);
    for (size_t x = 0; x <= width / 2; x++) {
        for (size_t  y = 0; y <= height / 2; y++) {
            const auto xF = static_cast<float>(x);
            const auto yF = static_cast<float>(y);
            const float a = (xF + 0.5f) / (widthF / 2.0f) - 1.0f;
            const float b = (yF + 0.5f) / (heightF / 2.0f) - 1.0f;
            if (a * a + b * b <= 1.0f) {
                pixels.set(static_cast<int64_t>(xF + lowerBound_.x()),
                           static_cast<int64_t>(yF + lowerBound_.y()), color_);
                pixels.set(static_cast<int64_t>(widthF - xF + lowerBound_.x()),
                           static_cast<int64_t>(heightF - yF + lowerBound_.y()), color_);
                pixels.set(static_cast<int64_t>(xF + lowerBound_.x()),
                           static_cast<int64_t>(heightF - yF + lowerBound_.y()), color_);
                pixels.set(static_cast<int64_t>(widthF - xF + lowerBound_.x()),
                           static_cast<int64_t>(yF + lowerBound_.y()), color_);
            }
        }
    }
    return canvas_;
}

Array<Point<float>> Ellipse::generatePointsOnEllipseSegment(size_t precision) {
    const float width = std::abs(upperBound_.x() - lowerBound_.x());
    const float height = std::abs(upperBound_.y() - lowerBound_.y());
    const  float a = width / 2.0f;
    const float b = height / 2.0f;
    const float ratio = b / a;

    Array<Point<float>> result(precision + 2);
    const size_t precisionD = precision / 2;

    result[0] = {a + lowerBound_.x(), height + lowerBound_.y()};
    result[1] = {width + lowerBound_.x(), b + lowerBound_.y()};

    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist1(0, a * 0.8f);
    std::uniform_real_distribution<> dist2(a * 0.8f, a);

    for (size_t i = 0; i < precisionD; i++) {
        const auto start = static_cast<float>(dist1(e2));
        const float yf = ratio * sqrtf(a * a - start * start);
        const float y = roundf(yf) + b + lowerBound_.y();
        result[i + 2] = {start + a + lowerBound_.x(), y};
    }

    for (size_t i = 0; i < precisionD; i++) {
        const auto start = static_cast<float>(dist2(e2));
        const float yf = ratio * sqrtf(a * a - start * start);
        const float y = roundf(yf) + b + lowerBound_.y();
        result[precisionD + i + 2] = {start + a + lowerBound_.x(), y};
    }

    result.sort();
    return result;
}

Array<Point<float>> Ellipse::mirrorEllipseSegment(const Array<Point<float>>& points,
                                                  const Point<float>& lineStart,
                                                  const Point<float>& lineEnd) {
    Array<Point<float>> result(points);
    for (size_t i = 0; i < result.size(); i++) {
        result[i].mirror(lineStart, lineEnd);
    }
    return result;
}

Canvas& Ellipse::fill(const LinearGradient& gradient, LinearGradient::Type type) {
    lowerBound_.swap(upperBound_);
    const auto width = std::abs(upperBound_.x() - lowerBound_.x());
    const auto height = std::abs(upperBound_.y() - lowerBound_.y());
    gradient.apply(canvas_.get(), lowerBound_, upperBound_,
                   [width, height](size_t i, size_t j) {
                       const float a = (static_cast<float>(i) + 0.5f) / (width / 2.0f) - 1.0f;
                       const float b = (static_cast<float>(j) + 0.5f) / (height / 2.0f) - 1.0f;
                       return a * a + b * b <= 1.0f;
                       }, type);
    return canvas_;
}

Canvas& Rectangle::draw() {
    lowerBound_.swap(upperBound_);

    const float width = std::abs(upperBound_.x() - lowerBound_.x());
    const float height = std::abs(upperBound_.y() - lowerBound_.y());
    drawLine({lowerBound_.x(), lowerBound_.y()}, {lowerBound_.x() + width, lowerBound_.y()});
    drawLine({lowerBound_.x(), lowerBound_.y()}, {lowerBound_.x(), lowerBound_.y() + height});
    drawLine({upperBound_.x(), upperBound_.y()}, {upperBound_.x() - width, upperBound_.y()});
    drawLine({upperBound_.x(), upperBound_.y()}, {upperBound_.x(), upperBound_.y() - height});
    return canvas_;
}

Canvas& Rectangle::fill() {
    Pixels& pixels = canvas_.get();
    lowerBound_.swap(upperBound_);

    const auto width = static_cast<size_t>(std::abs(upperBound_.x() - lowerBound_.x()));
    const auto height = static_cast<size_t>(std::abs(upperBound_.y() - lowerBound_.y()));
    const auto widthF = static_cast<float>(width);
    const auto heightF = static_cast<float>(height);
    for (size_t x = 0; x <= width / 2; x++) {
        for (size_t  y = 0; y <= height / 2; y++) {
            const auto xF = static_cast<float>(x);
            const auto yF = static_cast<float>(y);
            pixels.set(static_cast<int64_t>(xF + lowerBound_.x()),
                       static_cast<int64_t>(yF + lowerBound_.y()), color_);
            pixels.set(static_cast<int64_t>(widthF - xF + lowerBound_.x()),
                       static_cast<int64_t>(heightF - yF + lowerBound_.y()), color_);
            pixels.set(static_cast<int64_t>(xF + lowerBound_.x()),
                       static_cast<int64_t>(heightF - yF + lowerBound_.y()), color_);
            pixels.set(static_cast<int64_t>(widthF - xF + lowerBound_.x()),
                       static_cast<int64_t>(yF + lowerBound_.y()), color_);
        }
    }
    return canvas_;
}

Canvas& Rectangle::fill(const LinearGradient& gradient, LinearGradient::Type type) {
    lowerBound_.swap(upperBound_);
    gradient.apply(canvas_.get(), lowerBound_, upperBound_,
                   [](size_t i, size_t j) { return true; }, type);
    return canvas_;
}
