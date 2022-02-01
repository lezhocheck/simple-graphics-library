#pragma once
#include "Color.h"
#include "Canvas.h"
#include "Array.h"
#include <limits>

namespace sglib {
    class Shape {
    public:
        Shape(Canvas& canvas, const Color& color) : canvas_(canvas), color_(color) { }
        virtual Canvas& draw() = 0;
        virtual ~Shape() = default;

    protected:
        Canvas& canvas_;
        Color color_;

        void drawLine(Point<float> start, Point<float> end);
    };

    class Line : public Shape {
    public:
        Line(Canvas& canvas, const Color& color, Point<float> start, Point<float> finish) :
                Shape(canvas, color), start_(start), finish_(finish) { }
        Canvas& draw() override;
        Line rotate(Point<float> origin, int angle);
        Line mirror(const Line& other);
        ~Line() override = default;
    private:
        Point<float> start_, finish_;
    };

    class Ellipse : public Shape {
    public:
        Ellipse(Canvas& canvas, const Color& color,
                Point<float> lowerBound, Point<float> upperBound) :
                Shape(canvas, color),
                lowerBound_(lowerBound),
                upperBound_(upperBound) { }
        Canvas& fill();
        Canvas& fill(const LinearGradient& gradient, LinearGradient::Type type);
        Canvas& draw() override;
        ~Ellipse() override = default;
    private:
        Point<float> lowerBound_;
        Point<float> upperBound_;


        Array<Point<float>> generatePointsOnEllipseSegment(size_t precision = 20);
        Array<Point<float>> mirrorEllipseSegment(const Array<Point<float>>& points,
                                                 const Point<float>& lineStart,
                                                 const Point<float>& lineEnd);
    };

    class Rectangle : public Shape {
    public:
        Rectangle(Canvas& canvas, const Color& color,
                  Point<float> lowerBound, Point<float> upperBound) :
        Shape(canvas, color),
        lowerBound_(lowerBound),
        upperBound_(upperBound) { }
        Canvas& fill();
        Canvas& fill(const LinearGradient& gradient, LinearGradient::Type type);
        Canvas& draw() override;
        ~Rectangle() override = default;
    private:
        Point<float> lowerBound_;
        Point<float> upperBound_;
    };
}