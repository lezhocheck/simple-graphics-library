#include "Color.h"
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <iomanip>

using namespace sglib;

const Color Color::white = Color(Color::Rgb(255, 255, 255));
const Color Color::black = Color(Color::Rgb(0, 0, 0));
const Color Color::red = Color(Color::Rgb(255, 0, 0));
const Color Color::green = Color(Color::Rgb(0, 255, 0));
const Color Color::blue = Color(Color::Rgb(0, 0, 255));
const Color Color::yellow = Color(Color::Rgb(255, 255, 0));

Color::Rgb Color::toRgb(Color::Hsl hsl) {
    const double h = hsl.hue / 360.0;
    const double s = hsl.saturation / 100.0;
    const double l = hsl.lightness / 100.0;

    double r, g, b;
    if (s == 0.0) {
        r = g = b = l;
    } else {
        double q = l + s - l * s;
        if(l < 0.5) {
            q = l * (1.0 + s);
        }
        double p = 2.0 * l - q;
        r = hueToRgb(p, q, h + 1.0 / 3.0);
        g = hueToRgb(p, q, h);
        b = hueToRgb(p, q, h  - 1.0 / 3.0);
    }

    return {static_cast<uint8_t>(round(r * 255.0)),
            static_cast<uint8_t>(round(g * 255.0)),
            static_cast<uint8_t>(round(b * 255.0))};
}

double Color::hueToRgb(double p, double q, double t) {
    if (t < 0.0) t += 1.0;
    if (t > 1.0) t -= 1.0;

    if (t < 1.0 / 6.0) {
        return p + (q - p) * 6.0 * t;
    }
    if (t < 0.5) {
        return q;
    }
    if (t < 2.0 / 3.0) {
        return p + (q - p) * (2.0 / 3.0 - t) * 6.0;
    }
    return p;
}

void Color::set(const Rgb& rgb, uint8_t alpha) {
    rgb_ = rgb;
    alpha_ = alpha;
}

void Color::set(const Hsl& hsl, uint8_t alpha) {
    if (hsl.hue > 360 || hsl.saturation > 100 || hsl.lightness > 100) {
        throw std::invalid_argument("invalid format");
    }
    rgb_ = toRgb(hsl);
    alpha_ = alpha;
}

void Color::set(const std::string& hex, uint8_t alpha) {
    rgb_ = toRgb(hex);
    alpha_ = alpha;
}

Color::Rgb Color::toRgb(const std::string &hex) {
    if (hex[0] != '#' || hex.size() != 7) {
        throw std::invalid_argument("invalid format");
    }
    std::istringstream rStream(hex.substr(1, 2));
    std::istringstream gStream(hex.substr(3, 2));
    std::istringstream bStream(hex.substr(5, 2));
    int r, g, b;
    rStream >> std::hex >> r;
    gStream >> std::hex >> g;
    bStream >> std::hex >> b;
    return {static_cast<uint8_t>(r),
            static_cast<uint8_t>(g),
            static_cast<uint8_t>(b)};
}

Color::Rgb Color::getRgb() const {
    return rgb_;
}

Color::Hsl Color::getHsl() const {
    const double r = rgb_.red / 255.0;
    const double g = rgb_.green / 255.0;
    const double b = rgb_.blue / 255.0;

    const double max = (r > g && r > b) ? r : (g > b) ? g : b;
    const double min = (r < g && r < b) ? r : (g < b) ? g : b;

    double h, s, l;
    l = (max + min) / 2.0;

    if (max == min) {
        h = s = 0.0;
    } else {
        const double d = max - min;
        s = (l > 0.5) ? d / (2.0 - max - min) : d / (max + min);

        if (r > g && r > b) {
            h = (g - b) / d + (g < b ? 6.0 : 0.0);
        }
        else if (g > b) {
            h = (b - r) / d + 2.0;
        }
        else {
            h = (r - g) / d + 4.0;
        }

        h /= 6.0;
    }

    return {static_cast<uint16_t>(round(h * 360.0)),
            static_cast<uint16_t>(round(s * 100.0)),
            static_cast<uint16_t>(round(l * 100.0))};
}

std::string Color::getHex() const {
    std::ostringstream stream;
    stream << '#';
    stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(rgb_.red);
    stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(rgb_.green);
    stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(rgb_.blue);
    return stream.str();
}

uint8_t& Color::r() {
    return rgb_.red;
}

uint8_t& Color::g() {
    return rgb_.green;
}

uint8_t& Color::b() {
    return rgb_.blue;
}

uint8_t& Color::a() {
    return alpha_;
}

const uint8_t &Color::r() const {
    return rgb_.red;
}

const uint8_t &Color::g() const {
    return rgb_.green;
}

const uint8_t &Color::b() const {
    return rgb_.blue;
}

const uint8_t &Color::a() const {
    return alpha_;
}




