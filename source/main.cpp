#include <iostream>
#include "Canvas.h"
#include <ctime>
#include <random>

using namespace std;
using namespace sglib;

void drawSimpleFigures() {
    const size_t width = 512;
    const size_t height = 512;
    Canvas canvas(width, height);
    canvas.addLine({0, 0}, {500, 500}, Color::blue)
          .addLine({0, 500}, {500, 0}, Color::red)
          .addRectangle({100, 100}, {400, 400}, Color::green)
          .addEllipse({100, 100}, {400, 400}, Color::green)
          .addEllipse({20, 20}, {500, 300}, Color::black)
          .addEllipse({480, 480}, {0, 200}, Color::black);

    canvas.draw(R"(simple-figures.bmp)");
}

void drawFilledFigures() {
    const size_t width = 1920;
    const size_t height = 1080;
    Canvas canvas(width, height);
    canvas.fill(Color("#4295f5"));
    canvas.addFilledRectangle({0, 0}, {1920, 400},
                              Color(Color::Hsl(152, 92, 45)))
          .addFilledEllipse({1800, 900}, {1900, 1000}, Color::yellow)
          .addFilledRectangle({200, 200}, {250, 400},
                              Color(Color::Rgb(59, 42, 29)))
          .addFilledEllipse({150, 300}, {300, 600}, Color("#17a607"))
          .addFilledRectangle({900, 150}, {950, 350},
                                Color(Color::Rgb(59, 42, 29)))
          .addFilledEllipse({850, 300}, {1000, 600}, Color("#63d656"))
          .addFilledRectangle({1600, 200}, {1650, 400},
                                Color(Color::Rgb(59, 42, 29)))
          .addFilledEllipse({1550, 300}, {1700, 600}, Color("#17a607"));
    canvas.draw(R"(filled-figures.bmp)");
}

void drawGradient() {
    const size_t width = 720;
    const size_t height = 1280;
    mt19937 gen(36);
    Canvas canvas(width, height);
    canvas.fill({Color("#461061"), Color("#31096e"), Color("#110326")},
                LinearGradient::Type::UpToBottom);
    for (int i = 0; i < 1000; i++) {
        int x = gen() % width;
        int y = gen() % height;
        int size = gen() % 5;
        canvas.addFilledEllipse({x, y}, {x + size, y + size}, Color("#f5b0f2"));
    }
    canvas.addFilledEllipse({160, 440}, {560, 840},
                            {Color("#c75fc2"), Color("#5e195b"), Color("#260315")})
          .addFilledEllipse({300, 600}, {320, 620},
                              {Color("#5e195b"), Color("#260315")},
                              LinearGradient::Type::RightToLeft)
          .addFilledEllipse({450, 700}, {500, 750},
                              {Color("#5e195b"), Color("#260315")},
                              LinearGradient::Type::RightToLeft)
          .addFilledEllipse({380, 620}, {420, 660},
                              {Color("#5e195b"), Color("#260315")},
                              LinearGradient::Type::RightToLeft)
          .addFilledEllipse({200, 650}, {230, 680},
                              {Color("#c75fc2"), Color("#5e195b")},
                              LinearGradient::Type::RightToLeft)
          .addFilledEllipse({290, 750}, {330, 790},
                              {Color("#c75fc2"), Color("#5e195b")},
                              LinearGradient::Type::RightToLeft)
          .addFilledEllipse({300, 470}, {350, 520},
                              {Color("#c75fc2"), Color("#5e195b")},
                              LinearGradient::Type::RightToLeft);
    canvas.draw(R"(gradient.bmp)");
}

void drawSmile() {
    Canvas canvas(500, 500);
    canvas.fill(Color::black);
    canvas.addFilledEllipse({0, 0}, {500, 500}, Color::yellow)
          .addFilledEllipse({95, 45}, {405, 175}, Color::black)
          .addFilledEllipse({100, 50}, {400, 175}, Color::white)
          .addFilledEllipse({95, 85}, {405, 225}, Color::black)
          .addFilledEllipse({100, 90}, {400, 220}, Color::yellow)
          .addFilledEllipse({80, 104}, {420, 220}, Color::yellow)
          .addFilledEllipse({80, 120}, {420, 250}, Color::yellow)
          .addFilledEllipse({115, 195}, {185, 455}, Color::black)
          .addFilledEllipse({315, 195}, {385, 455}, Color::black)
          .addFilledEllipse({120, 200}, {180, 450}, Color::white)
          .addFilledEllipse({320, 200}, {380, 450}, Color::white)
          .addFilledEllipse({135, 270}, {165, 320}, Color::black)
          .addFilledEllipse({335, 270}, {365, 320}, Color::black);
    canvas.draw(R"(smile.bmp)");
}

int main() {
    drawSimpleFigures();
    drawFilledFigures();
    drawGradient();
    drawSmile();
    return 0;
}
