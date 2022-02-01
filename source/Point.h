#pragma once
#include <cmath>

namespace sglib {
    template<typename T>
    class Point {
    public:
        Point() : x_(0), y_(0) { }
        Point(const T& x, const T& y) : x_(x), y_(y) { }
        bool operator<(const Point& other) const {
            if (x_ == other.x_) {
                return y_ < other.y_;
            }
            return x_ < other.x_;
        }

        bool operator>(const Point& other) const {
            if (x_ == other.x_) {
                return y_ > other.y_;
            }
            return x_ > other.x_;
        }

        bool operator>=(const Point& other) const {
            return !(*this < other);
        }

        bool operator<=(const Point& other) const {
            return !(*this > other);
        }

        void rotate(Point<T>& origin, int angle) {
            if (angle % 360 == 0) {
                return;
            }
            const auto PI = static_cast<float>(M_PI);
            const float angleRad = static_cast<float>(angle) * PI / -180.0f;
            const float s = sinf(angleRad);
            const float c = cosf(angleRad);

            x_ -= origin.x_;
            y_ -= origin.y_;

            float xNew = x_ * c - y_ * s;
            float yNew = x_ * s + y_ * c;

            x_ = xNew + origin.x_;
            y_ = yNew + origin.y_;
        }

        void mirror(Point<T> lineStart, Point<T> lineFinish) {
            float dx = lineFinish.x_ - lineStart.x_;
            float dy = lineFinish.y_ - lineStart.y_;

            float a = (dx * dx - dy * dy) / (dx * dx + dy * dy);
            float b = 2 * dx * dy / (dx * dx + dy * dy);

            float xNew = a * (x_ - lineStart.x_) + b * (y_ - lineStart.y_) + lineStart.x_;
            float yNew = b * (x_ - lineStart.x_) - a * (y_ - lineStart.y_) + lineStart.y_;
            x_ = xNew;
            y_ = yNew;
        }

        T& x() {
            return x_;
        }

        T& y() {
            return y_;
        }

        const T& x() const {
            return x_;
        }

        const T& y() const {
            return y_;
        }

        float getDistance(const Point<T>& other) {
            return sqrtf((x_ - other.x_) * (x_ - other.x_) + (y_ - other.y_) * (y_ - other.y_));
        }

        void swap(Point<T>& other) {
            if (x_ > other.x_) {
                std::swap(x_, other.x_);
            }
            if (y_ > other.y_) {
                std::swap(y_, other.y_);
            }
        }

    private:
        T x_, y_;
    };
}
