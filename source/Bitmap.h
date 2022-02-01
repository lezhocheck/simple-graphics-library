#pragma once
#include <cstdint>
#include <fstream>
#include <utility>
#include "Color.h"
#include "Pixels.h"

namespace sglib {
    class Bitmap {
    public:
        enum class Type {
            bit24,
            bit32
        };

        explicit Bitmap(const std::string& path) : filePath(path) { }
        explicit Bitmap(const Bitmap& other) = delete;
        Bitmap& operator=(const Bitmap& other) = delete;
        virtual void write(const Pixels& pixels) = 0;
        virtual void read() = 0;
        virtual ~Bitmap() = default;
        const Pixels& getPixels() const;

    protected:
        class Header {
        public:
            Header() : bytes(nullptr), hSize(0) { }
            explicit Header(uint8_t headerSize);
            Header(const Header& other);
            Header(Header&& other) noexcept;
            Header& operator=(const Header& other);
            Header& operator=(Header&& other) noexcept;
            ~Header();
            const uint8_t* getBytes() const;
            bool empty() const;

        protected:
            uint8_t* bytes;
            uint8_t hSize;
        };

        class FileHeader : public Header {
        public:
            const static uint8_t headerSize = 14;
            FileHeader() : Header() { }
            FileHeader(int32_t totalFileSize, uint8_t informationHeaderSize);
            explicit FileHeader(const uint8_t* bytesArray);
            int32_t fileSize() const;
            int32_t startOfPixelArray() const;
        };

        class InformationHeader : public Header {
        public:
            const static uint8_t headerSize = 40;
            InformationHeader() : Header() { }
            InformationHeader(int32_t width, int32_t height, uint8_t bytesPerPixel);
            explicit InformationHeader(const uint8_t* bytesArray);

            int32_t width() const;
            int32_t height() const;
            uint8_t bytesPerPixel() const;
        };

        std::string filePath;
        FileHeader fileHeader;
        InformationHeader informationHeader;
        Pixels pixelsData;
    };

    class Bitmap24 : public Bitmap {
    public:
        explicit Bitmap24(const std::string& path) : Bitmap(path) { }
        explicit Bitmap24(const Bitmap24& other) = delete;
        Bitmap24& operator=(const Bitmap24& other) = delete;

        void write(const Pixels& pixels) override;
        void read() override;
        ~Bitmap24() override = default;
    };

    class Bitmap32 : public Bitmap {
    public:
        explicit Bitmap32(const std::string& path) : Bitmap(path) { }
        explicit Bitmap32(const Bitmap32& other) = delete;
        Bitmap32& operator=(const Bitmap32& other) = delete;

        void write(const Pixels& pixels) override;
        void read() override;
        ~Bitmap32() override = default;
    };
}
