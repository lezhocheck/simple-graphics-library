#pragma once
#include <cstdint>
#include <string>

namespace sglib {
    class Color {
    public:
        class Rgb {
        public:
            uint8_t red, green, blue;
            Rgb(uint8_t r, uint8_t g, uint8_t b) : red(r), green(g), blue(b) { }
        };

        class Hsl {
        public:
            uint16_t hue, saturation, lightness;
            Hsl(uint16_t h, uint16_t s, uint16_t l) : hue(h), saturation(s), lightness(l) { }
        };

        const static Color white;
        const static Color black;
        const static Color red;
        const static Color blue;
        const static Color green;
        const static Color yellow;

        Color() : rgb_({0, 0, 0}), alpha_(255) { }
        explicit Color(const Rgb& rgb, uint8_t alpha = 255) : rgb_(rgb), alpha_(alpha) { }
        explicit Color(const Hsl& hsl, uint8_t alpha = 255) : rgb_(toRgb(hsl)), alpha_(alpha) { }
        explicit Color(const std::string& hex, uint8_t alpha = 255) : rgb_(toRgb(hex)), alpha_(alpha) { }

        void set(const Rgb& rgb, uint8_t alpha = 255);
        void set(const std::string& hex, uint8_t alpha = 255);
        void set(const Hsl& hsl, uint8_t alpha = 255);

        uint8_t& r();
        uint8_t& g();
        uint8_t& b();
        uint8_t& a();

        const uint8_t& r() const;
        const uint8_t& g() const;
        const uint8_t& b() const;
        const uint8_t& a() const;

        Rgb getRgb() const;
        Hsl getHsl() const;
        std::string getHex() const;

    private:
        Rgb rgb_;
        uint8_t alpha_;

        static Rgb toRgb(Hsl hsl);
        static Rgb toRgb(const std::string& hex);
        static double hueToRgb(double p, double q, double t);
    };
}