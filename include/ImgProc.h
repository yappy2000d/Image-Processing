#ifndef IMGPROC_HPP
#define IMGPROC_HPP

#include <vector>
#include <fstream>
#include <cstdint>
#include <string>
#include <initializer_list>
#include <array>
#include <algorithm>
#include <cstddef>

constexpr std::size_t COLOR_TABLE_SIZE = 256;

// Follow the naming convention of wingdi.h
#pragma pack(push, 1)
struct RGBTRIPLE {
    uint8_t rgbtBlue;
    uint8_t rgbtGreen;
    uint8_t rgbtRed;
};

struct BITMAPFILEHEADER {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};

struct BITMAPINFOHEADER {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeRGBImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
#pragma pack(pop)

enum Compression {
    BI_RGB,
    BI_RLE8,
    BI_RLE4,
    BI_BITFIELDS,
    BI_JPEG,
    BI_PNG,
};

enum class ColorSpace {
    RGB,
    HSI,
    YCC,
};

enum class Channel {
    BLUE,
    GREEN,
    RED,
};

struct Matrix : std::vector<std::vector<double>>
{
    int width, height;
    Matrix(int height, int width) noexcept;
    Matrix(int height, int width, double value) noexcept;
    Matrix(std::initializer_list<std::initializer_list<double>> list);
    Matrix operator-() const noexcept;          // Component-wise negation
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix operator+(double scalar) const noexcept;
    Matrix operator-(double scalar) const noexcept;
    Matrix operator*(double scalar) const noexcept;
    friend Matrix operator+(double scalar, const Matrix& matrix) noexcept;
    friend Matrix operator-(double scalar, const Matrix& matrix) noexcept;
    friend Matrix operator*(double scalar, const Matrix& matrix) noexcept;
    Matrix& operator+= (const Matrix& other);
    Matrix& operator-= (const Matrix& other);
    Matrix& operator*= (const Matrix& other);
    Matrix& operator+= (double scalar) noexcept;
    Matrix& operator-= (double scalar) noexcept;
    Matrix& operator*= (double scalar) noexcept;
    bool operator==(const Matrix& other) const noexcept;
    bool operator!=(const Matrix& other) const noexcept;
    Matrix transpose() const noexcept;
    Matrix T() const noexcept;
    double dot(const Matrix& other) const;
    Matrix submatrix(int y, int x, int h, int w) const;
};

struct GrayImage : std::vector<std::vector<uint8_t>>
{
    int width, height;
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    GrayImage() noexcept : width(0), height(0) {}
    GrayImage(int height, int width) noexcept;
    GrayImage& toFile(const std::string& filename);
    Matrix toMatrix() const noexcept;
    static GrayImage fromMatrix(const Matrix& matrix) noexcept;
};

struct RGBImage : std::vector<std::vector<RGBTRIPLE>>
{
    int width, height;
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    RGBImage() noexcept : width(0), height(0) {}
    RGBImage(int height, int width) noexcept;
    GrayImage getChannel(const Channel& channel) const;
    static RGBImage fromFile(const std::string& filename);
    RGBImage& toFile(const std::string& filename);
    GrayImage toGray(const ColorSpace& method);
    static RGBImage fromGrays(const GrayImage& bChannel, const GrayImage& gChannel, const GrayImage& rChannel) noexcept;
};

enum class ShapeType {
    NONE,
    CIRCLE,
    RECTANGLE,
};

class OutlineRenderer {
private:
    int y, x;
    int radius;
    int height, width;
    RGBTRIPLE color;
    int thickness;
    RGBImage image;
    ShapeType shape;

    // Render the hollow rectangle
    // x, y is the top-left corner
    RGBImage renderRectangle();
    // Render the hollow circle
    // x, y is the center
    RGBImage renderCircle();

public:
    OutlineRenderer() noexcept;
    OutlineRenderer(const RGBImage& image) noexcept;
    OutlineRenderer(const GrayImage& image) noexcept;

    OutlineRenderer& setPos(int y, int x) noexcept;
    OutlineRenderer& setColor(const RGBTRIPLE& color) noexcept;
    OutlineRenderer& setThickness(int thickness) noexcept;
    OutlineRenderer& setImage(const RGBImage& image) noexcept;
    OutlineRenderer& setImage(const GrayImage& image) noexcept;
    OutlineRenderer& setShape(const ShapeType& shape) noexcept;
    OutlineRenderer& setDimensions(int height, int width) noexcept;
    OutlineRenderer& setRadius(int radius) noexcept;

    RGBImage render();
};

#endif // IMGPROC_HPP