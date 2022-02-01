#include "Bitmap.h"
#include <fstream>
#include <cstring>
#include <stdexcept>
#include <algorithm>

using namespace sglib;

Bitmap::Header::Header(uint8_t headerSize) {
    bytes = new uint8_t[headerSize];
    std::memset(bytes, 0, headerSize);
    hSize = headerSize;
}

Bitmap::Header::~Header() {
    delete[] bytes;
    hSize = 0;
}

const uint8_t* Bitmap::Header::getBytes() const {
    return bytes;
}

Bitmap::Header::Header(const Bitmap::Header& other) :
        hSize(other.hSize) {
    bytes = new uint8_t[hSize];
    std::copy_n(other.bytes, hSize, bytes);
}

Bitmap::Header::Header(Bitmap::Header&& other) noexcept :
        hSize(other.hSize) {
    other.hSize = 0;
    bytes = other.bytes;
    other.bytes = nullptr;
}

Bitmap::Header &Bitmap::Header::operator=(Bitmap::Header&& other) noexcept {
    hSize = other.hSize;
    other.hSize = 0;
    bytes = other.bytes;
    other.bytes = nullptr;
    return *this;
}

Bitmap::Header &Bitmap::Header::operator=(const Bitmap::Header& other) {
    if (this == &other) {
        return *this;
    }
    hSize = other.hSize;
    bytes = new uint8_t[hSize];
    std::copy_n(other.bytes, hSize, bytes);
    return *this;
}

bool Bitmap::Header::empty() const {
    return hSize == 0;
}

Bitmap::FileHeader::FileHeader(int32_t totalFileSize,
                               uint8_t informationHeaderSize) :
                               Header(headerSize) {
    /*   0,0,      signature                 */
    /*   0,0,0,0,  image file size in bytes  */
    /*   0,0,0,0,  reserved                  */
    /*   0,0,0,0   start of pixel array      */

    bytes[0] = static_cast<uint8_t>('B');
    bytes[1] = static_cast<uint8_t>('M');
    bytes[2] = static_cast<uint8_t>(totalFileSize);
    bytes[3] = static_cast<uint8_t>(totalFileSize >>  8);
    bytes[4] = static_cast<uint8_t>(totalFileSize >> 16);
    bytes[5] = static_cast<uint8_t>(totalFileSize >> 24);
    bytes[10] = static_cast<uint8_t>(headerSize + informationHeaderSize);
}

Bitmap::FileHeader::FileHeader(const uint8_t* bytesArray) :
                               Header(headerSize) {
    std::copy_n(bytesArray, headerSize, bytes);
}

int32_t Bitmap::FileHeader::fileSize() const {
    if (bytes == nullptr) {
        throw std::out_of_range("header is empty");
    }
    int64_t result = bytes[2] +
                     (static_cast<int64_t>(bytes[3]) << 8) +
                     (static_cast<int64_t>(bytes[4]) << 16) +
                     (static_cast<int64_t>(bytes[5]) << 32);

    return static_cast<int32_t>(result);
}

int32_t Bitmap::FileHeader::startOfPixelArray() const {
    if (bytes == nullptr) {
        throw std::out_of_range("header is empty");
    }
    int64_t result = bytes[10] +
                     (static_cast<int64_t>(bytes[11]) << 8) +
                     (static_cast<int64_t>(bytes[12]) << 16) +
                     (static_cast<int64_t>(bytes[13]) << 32);

    return static_cast<int32_t>(result);
}

Bitmap::InformationHeader::InformationHeader(int32_t width,
                                             int32_t height,
                                             uint8_t bytesPerPixel) :
                                             Header(headerSize) {
    /*   0,0,0,0,  header size             */
    /*   0,0,0,0,  image width             */
    /*   0,0,0,0,  image height            */
    /*   0,0,      number of color planes  */
    /*   0,0,      bits per pixel          */
    /*   0,0,0,0,  compression             */
    /*   0,0,0,0,  image size              */
    /*   0,0,0,0,  horizontal resolution   */
    /*   0,0,0,0,  vertical resolution     */
    /*   0,0,0,0,  colors in color table   */
    /*   0,0,0,0  important color count    */

    bytes[0] = static_cast<uint8_t>(headerSize);
    bytes[4] = static_cast<uint8_t>(width);
    bytes[5] = static_cast<uint8_t>(width >> 8);
    bytes[6] = static_cast<uint8_t>(width >> 16);
    bytes[7] = static_cast<uint8_t>(width >> 24);
    bytes[8] = static_cast<uint8_t>(height);
    bytes[9] = static_cast<uint8_t>(height >> 8);
    bytes[10] = static_cast<uint8_t>(height >> 16);
    bytes[11] = static_cast<uint8_t>(height >> 24);
    bytes[12] = static_cast<uint8_t>(1);
    bytes[14] = static_cast<uint8_t>(bytesPerPixel * 8);
}

Bitmap::InformationHeader::InformationHeader(const uint8_t* bytesArray) :
                                             Header(headerSize) {
    std::copy_n(bytesArray, headerSize, bytes);
}

int32_t Bitmap::InformationHeader::width() const {
    if (bytes == nullptr) {
        throw std::out_of_range("header is empty");
    }

    int64_t result = bytes[4] +
            (static_cast<int64_t>(bytes[5]) << 8) +
            (static_cast<int64_t>(bytes[6]) << 16) +
            (static_cast<int64_t>(bytes[7]) << 32);

    return static_cast<int32_t>(result);
}

int32_t Bitmap::InformationHeader::height() const {
    if (bytes == nullptr) {
        throw std::out_of_range("header is empty");
    }
    int64_t result = bytes[8] +
                     (static_cast<int64_t>(bytes[9]) << 8) +
                     (static_cast<int64_t>(bytes[10]) << 16) +
                     (static_cast<int64_t>(bytes[11]) << 32);

    return static_cast<int32_t>(result);
}

uint8_t Bitmap::InformationHeader::bytesPerPixel() const {
    if (bytes == nullptr) {
        throw std::out_of_range("header is empty");
    }
    return bytes[14] / 8;
}

const Pixels& Bitmap::getPixels() const {
    return pixelsData;
}

void Bitmap24::read() {
    std::ifstream file(filePath, std::ios::in | std::ios::binary);
    if (!file.is_open()){
        throw std::invalid_argument("cannot open the file");
    }

    char fileHeaderArray[FileHeader::headerSize];
    char informationHeaderArray[InformationHeader::headerSize];
    file.read(fileHeaderArray, FileHeader::headerSize);
    file.read(informationHeaderArray, InformationHeader::headerSize);

    auto* fileHeaderPtr = reinterpret_cast<uint8_t*>(fileHeaderArray);
    auto* informationHeaderPtr = reinterpret_cast<uint8_t*>(informationHeaderArray);

    fileHeader = FileHeader(fileHeaderPtr);
    informationHeader = InformationHeader(informationHeaderPtr);

    const int32_t startOfPixelArray = fileHeader.startOfPixelArray();
    const int32_t width = informationHeader.width();
    const int32_t height = informationHeader.height();
    const uint8_t bytesPerPixel = informationHeader.bytesPerPixel();

    const size_t paddingSize = (4 - (width * 3) % 4) % 4;

    if (bytesPerPixel != 3) {
        throw std::runtime_error("supports only 24-bit format");
    }

    pixelsData = Pixels(width, height);
    file.seekg(startOfPixelArray, std::ios::beg);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            char values[3];
            file.read(values, 3);
            auto b = static_cast<uint8_t>(values[0]);
            auto g = static_cast<uint8_t>(values[1]);
            auto r = static_cast<uint8_t>(values[2]);
            pixelsData.set(x, y, Color(Color::Rgb(r, g, b)));
        }

        file.ignore(static_cast<int64_t>(paddingSize));
    }
    file.close();
}

void Bitmap32::read() {
    std::ifstream file(filePath, std::ios::in | std::ios::binary);
    if (!file.is_open()){
        throw std::invalid_argument("cannot open the file");
    }

    char fileHeaderArray[FileHeader::headerSize];
    char informationHeaderArray[InformationHeader::headerSize];
    file.read(fileHeaderArray, FileHeader::headerSize);
    file.read(informationHeaderArray, InformationHeader::headerSize);

    auto* fileHeaderPtr = reinterpret_cast<uint8_t*>(fileHeaderArray);
    auto* informationHeaderPtr = reinterpret_cast<uint8_t*>(informationHeaderArray);

    fileHeader = FileHeader(fileHeaderPtr);
    informationHeader = InformationHeader(informationHeaderPtr);

    const int32_t startOfPixelArray = fileHeader.startOfPixelArray();
    const int32_t width = informationHeader.width();
    const int32_t height = informationHeader.height();
    const uint8_t bytesPerPixel = informationHeader.bytesPerPixel();

    if (bytesPerPixel != 4) {
        throw std::runtime_error("supports only 32-bit format");
    }

    pixelsData = Pixels(width, height);
    file.seekg(startOfPixelArray, std::ios::beg);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            char values[4];
            file.read(values, 4);
            auto b = static_cast<uint8_t>(values[0]);
            auto g = static_cast<uint8_t>(values[1]);
            auto r = static_cast<uint8_t>(values[2]);
            auto a = static_cast<uint8_t>(values[3]);
            pixelsData.set(x, y, Color(Color::Rgb(r, g, b), a));
        }
    }
    file.close();
}


void Bitmap24::write(const Pixels& pixels) {
    std::ofstream file(filePath, std::ios::out | std::ios::binary);
    if (!file.is_open()){
        throw std::invalid_argument("cannot create the file");
    }
    pixelsData = pixels;
    const uint8_t fileHeaderSize = FileHeader::headerSize;
    const uint8_t informationHeaderSize = InformationHeader::headerSize;

    const size_t widthInBytes = pixelsData.width() * 3;
    const size_t paddingSize = (4 - (widthInBytes) % 4) % 4;
    uint8_t padding[3] = {0, 0, 0};

    const size_t stride = widthInBytes + paddingSize;

    const auto fileSize = static_cast<int32_t>(fileHeaderSize +
        informationHeaderSize + stride * pixelsData.height());

    informationHeader = InformationHeader(static_cast<int32_t>(pixelsData.width()),
        static_cast<int32_t>(pixelsData.height()), 3);
    fileHeader = FileHeader(fileSize, informationHeaderSize);
    file.write(reinterpret_cast<const char*>(fileHeader.getBytes()), fileHeaderSize);
    file.write(reinterpret_cast<const char*>(informationHeader.getBytes()), informationHeaderSize);
    for (int y = 0; y < pixelsData.height(); y++) {
        for (int x = 0; x < pixelsData.width(); x++) {
            Color::Rgb rgb = pixelsData.get(x, y).getRgb();
            uint8_t values[] = {rgb.blue, rgb.green, rgb.red};
            file.write(reinterpret_cast<const char*>(values), 3);
        }
        file.write(reinterpret_cast<char*>(padding), static_cast<int64_t>(paddingSize));
    }
    file.close();
}

void Bitmap32::write(const Pixels& pixels) {
    std::ofstream file(filePath, std::ios::out | std::ios::binary);
    if (!file.is_open()){
        throw std::invalid_argument("cannot create the file");
    }
    pixelsData = pixels;
    const uint8_t fileHeaderSize = FileHeader::headerSize;
    const uint8_t informationHeaderSize = InformationHeader::headerSize;

    const size_t widthInBytes = pixelsData.width() * 4;

    const auto fileSize = static_cast<int32_t>(fileHeaderSize +
                                               informationHeaderSize + widthInBytes * pixelsData.height());

    informationHeader = InformationHeader(static_cast<int32_t>(pixelsData.width()),
                                          static_cast<int32_t>(pixelsData.height()), 4);
    fileHeader = FileHeader(fileSize, informationHeaderSize);

    file.write(reinterpret_cast<const char*>(fileHeader.getBytes()), fileHeaderSize);
    file.write(reinterpret_cast<const char*>(informationHeader.getBytes()), informationHeaderSize);
    for (int y = 0; y < pixelsData.height(); y++) {
        for (int x = 0; x < pixelsData.width(); x++) {
            Color::Rgb rgb = pixelsData.get(x, y).getRgb();
            uint8_t values[] = {rgb.blue, rgb.green, rgb.red, pixelsData.get(x, y).a()};
            file.write(reinterpret_cast<const char*>(values), 4);
        }
    }
    file.close();
}