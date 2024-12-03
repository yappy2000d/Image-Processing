#ifndef IMGPROC_HPP
#define IMGPROC_HPP

#include <vector>
#include <fstream>
#include <cstdint>
#include <string>
#include <initializer_list>
#include <array>
#include <algorithm>
#include <filesystem>
#include <tuple>
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
    // GrayImage getChannel(const Channel& channel) const;
    std::tuple<GrayImage, GrayImage, GrayImage> getChannels() const noexcept;
    static RGBImage fromFile(const std::string& filename);
    RGBImage& toFile(const std::string& filename);
    GrayImage toGray(const ColorSpace& method);
    static RGBImage fromGrays(const GrayImage& bChannel, const GrayImage& gChannel, const GrayImage& rChannel) noexcept;
};

///////////////////////////////////////
//      Matrix implementation        //
///////////////////////////////////////

Matrix::Matrix(int height, int width) noexcept : std::vector<std::vector<double>>(height, std::vector<double>(width)), height(height), width(width) {}
Matrix::Matrix(std::initializer_list<std::initializer_list<double>> list) : std::vector<std::vector<double>>(list.size(), std::vector<double>(list.begin()->size())), width(list.begin()->size()), height(list.size()) {
    std::copy(list.begin(), list.end(), begin());
}
Matrix::Matrix(int height, int width, double value) noexcept : std::vector<std::vector<double>>(height, std::vector<double>(width, value)), height(height), width(width) {}

Matrix Matrix::operator*(const Matrix& other) const {
    if (width != other.height)
        throw std::runtime_error("Cannot multiply matrices with incompatible shapes! Got " + std::to_string(width) + "x" + std::to_string(height) + " and " + std::to_string(other.width) + "x" + std::to_string(other.height));

    Matrix result(height, other.width);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < other.width; x++) {
            double sum = 0;
            for (int i = 0; i < width; i++) {
                sum += (*this)[y][i] * other[i][x];
            }
            result[y][x] = sum;
        }
    }

    return result;
}

Matrix Matrix::operator*(double scalar) const noexcept {
    Matrix result(height, width);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            result[y][x] = (*this)[y][x] * scalar;
        }
    }

    return result;
}

Matrix operator*(double scalar, const Matrix& matrix) noexcept {
    return matrix * scalar;
}

Matrix Matrix::operator+(const Matrix& other) const {
    if (width != other.width || height != other.height)
        throw std::runtime_error("Cannot add matrices with different shapes! Got " + std::to_string(width) + "x" + std::to_string(height) + " and " + std::to_string(other.width) + "x" + std::to_string(other.height));

    Matrix result(height, width);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            result[y][x] = (*this)[y][x] + other[y][x];
        }
    }

    return result;
}

Matrix Matrix::operator+(double scalar) const noexcept {
    Matrix result(height, width);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            result[y][x] = (*this)[y][x] + scalar;
        }
    }

    return result;
}

Matrix operator+(double scalar, const Matrix& matrix) noexcept {
    return matrix + scalar;
}

Matrix Matrix::operator-(const Matrix& other) const {
    if (width != other.width || height != other.height)
        throw std::runtime_error("Cannot subtract matrices with different shapes! Got " + std::to_string(width) + "x" + std::to_string(height) + " and " + std::to_string(other.width) + "x" + std::to_string(other.height));

    Matrix result(height, width);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            result[y][x] = (*this)[y][x] - other[y][x];
        }
    }

    return result;
}

Matrix Matrix::operator-() const noexcept {
    Matrix result(height, width);

    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            result[y][x] = -(*this)[y][x];
        }
    }

    return result;
}

Matrix Matrix::operator-(double scalar) const noexcept {
    return *this + (-scalar);
}

Matrix operator-(double scalar, const Matrix& matrix) noexcept {
    return scalar + (-matrix);
}

Matrix& Matrix::operator*=(const Matrix& other) {
    return *this = *this * other;
}

Matrix& Matrix::operator*=(double scalar) noexcept {
    return *this = *this * scalar;
}

Matrix& Matrix::operator+=(const Matrix& other) {
    return *this = *this + other;
}

Matrix& Matrix::operator+=(double scalar) noexcept {
    return *this = *this + scalar;
}

Matrix& Matrix::operator-=(const Matrix& other) {
    return *this = *this - other;
}

Matrix& Matrix::operator-=(double scalar) noexcept {
    return *this = *this - scalar;
}

bool Matrix::operator==(const Matrix& other) const noexcept {
    if (width != other.width || height != other.height) return false;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if ((*this)[y][x] != other[y][x]) return false;
        }
    }

    return true;
}

bool Matrix::operator!=(const Matrix& other) const noexcept {
    return !(*this == other);
}

Matrix Matrix::transpose() const noexcept {
    Matrix result(width, height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            result[x][y] = (*this)[y][x];
        }
    }

    return result;
}

Matrix Matrix::T() const noexcept {
    return transpose();
}

// Frobenius inner product
double Matrix::dot(const Matrix& other) const {
    if (width != other.width || height != other.height)
        throw std::runtime_error("Cannot calculate dot product of matrices with different shapes! Got " + std::to_string(width) + "x" + std::to_string(height) + " and " + std::to_string(other.width) + "x" + std::to_string(other.height));

    double sum = 0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            sum += (*this)[y][x] * other[y][x];
        }
    }

    return sum;
}

Matrix Matrix::submatrix(int y, int x, int h, int w) const {
    if (y + h > height || x + w > width)
        throw std::runtime_error("Submatrix out of bounds! Matrix size is " + std::to_string(width) + "x" + std::to_string(height) + ", requested submatrix size is " + std::to_string(w) + "x" + std::to_string(h) + " at position " + std::to_string(x) + "," + std::to_string(y));

    if (y < 0 || x < 0 || h < 0 || w < 0)
        throw std::runtime_error("Submatrix size and position must be positive! Got " + std::to_string(w) + "x" + std::to_string(h) + " at position " + std::to_string(x) + "," + std::to_string(y));

    Matrix result(h, w);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            result[i][j] = (*this)[y + i][x + j];
        }
    }

    return result;
}

////////////////////////////////////////
//      GrayImage implementation      //
////////////////////////////////////////

GrayImage::GrayImage(int height, int width) noexcept : std::vector<std::vector<uint8_t>>(height, std::vector<uint8_t>(width)), height(height), width(width)
{
    // Initializer list
    fileHeader = {
        0x4D42,
        static_cast<uint32_t>(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + COLOR_TABLE_SIZE * 4 + height * ((width + 3) & ~3)),
        0, 0,
        static_cast<uint32_t>(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + COLOR_TABLE_SIZE * 4),
    };

    infoHeader = {
        sizeof(BITMAPINFOHEADER),
        width,
        height,
        1,
        8,
        BI_RGB,
        0, 0, 0,
        COLOR_TABLE_SIZE,
        0,
    };
}

GrayImage& GrayImage::toFile(const std::string& filename) {

    namespace fs = std::filesystem;

    if(!fs::exists(fs::path(filename).parent_path())) {
        fs::create_directories(fs::path(filename).parent_path());
    }

    std::ofstream file(filename, std::ios::binary);

    if (!file) throw std::runtime_error("Failed to open the file: " + filename);

    file.write(reinterpret_cast<char*>(&fileHeader), sizeof(BITMAPFILEHEADER));
    file.write(reinterpret_cast<char*>(&infoHeader), sizeof(BITMAPINFOHEADER));
    
    // Color table(Mandatory for color depths ≤ 8 bits)
    std::array<uint8_t, COLOR_TABLE_SIZE * 4> colorTable;
    for (uint16_t i = 0; i < COLOR_TABLE_SIZE; i++) {
        uint8_t* color = colorTable.data() + i * 4;
        color[0] = color[1] = color[2] = i; // BGR channels
        color[3] = 0;                       // Alpha channel
    }
    file.write(reinterpret_cast<char*>(colorTable.data()), colorTable.size() * sizeof(uint8_t));


    int padding = (4 - (width % 4)) % 4;

    // BMP files are stored bottom-up
    for (int y = height - 1; y >= 0; --y) {
        file.write(reinterpret_cast<char*>(this->at(y).data()), width);
        file.write("\0\0\0", padding);
    }

    file.close();
    return *this;
}

Matrix GrayImage::toMatrix() const noexcept {
    Matrix matrix(height, width);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            matrix[y][x] = (*this)[y][x];
        }
    }

    return matrix;
}

GrayImage GrayImage::fromMatrix(const Matrix& matrix) noexcept {
    GrayImage image(matrix.height, matrix.width);

    for (int y = 0; y < matrix.height; y++) {
        for (int x = 0; x < matrix.width; x++) {
            image[y][x] = std::clamp(static_cast<uint8_t>(matrix[y][x]), 0, 255);
        }
    }

    return image;
}

RGBImage::RGBImage(int height, int width) noexcept : std::vector<std::vector<RGBTRIPLE>>(height, std::vector<RGBTRIPLE>(width)), height(height), width(width)
{
    int paddingSize = (4 - (width * 3 % 4)) % 4;

    fileHeader = {
        0x4D42,
        static_cast<uint32_t>(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + height * width * 3 + height * paddingSize),
        0, 0,
        static_cast<uint32_t>(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)),
    };

    infoHeader = {
        sizeof(BITMAPINFOHEADER),
        width,
        height,
        1,
        24,
        BI_RGB,
        0, 0, 0, 0,
    };
}

std::tuple<GrayImage, GrayImage, GrayImage> RGBImage::getChannels() const noexcept {
    GrayImage bChannel(height, width);
    GrayImage gChannel(height, width);
    GrayImage rChannel(height, width);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            bChannel[y][x] = (*this)[y][x].rgbtBlue;
            gChannel[y][x] = (*this)[y][x].rgbtGreen;
            rChannel[y][x] = (*this)[y][x].rgbtRed;
        }
    }

    return { bChannel, gChannel, rChannel };
}

RGBImage RGBImage::fromFile(const std::string& filename) {
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    
    std::ifstream file(filename, std::ios::binary);
    
    if (!file) throw std::runtime_error("Failed to open the file: " + filename);

    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BITMAPFILEHEADER));
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BITMAPINFOHEADER));

    if(infoHeader.biCompression != BI_RGB) throw std::runtime_error("Only uncompressed BMP files are supported!");

    int width = infoHeader.biWidth;
    int height = infoHeader.biHeight;
    int paddingSize = (4 - (width * 3 % 4)) % 4;

    RGBImage image(height, width);

    file.seekg(fileHeader.bfOffBits, std::ios::beg);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            RGBTRIPLE pixel;
            file.read(reinterpret_cast<char*>(&pixel), sizeof(RGBTRIPLE));

            // BMP files are stored bottom-up
            image[height - y - 1][x] = pixel;
        }
        file.seekg(paddingSize, std::ios::cur);
    }

    file.close();
    return image;
}

RGBImage& RGBImage::toFile(const std::string& filename) {
    namespace fs = std::filesystem;

    if(!fs::exists(fs::path(filename).parent_path())) {
        fs::create_directories(fs::path(filename).parent_path());
    }

    std::ofstream file(filename, std::ios::binary);

    if (!file) throw std::runtime_error("Failed to open the file: " + filename);

    file.write(reinterpret_cast<char*>(&fileHeader), sizeof(BITMAPFILEHEADER));
    file.write(reinterpret_cast<char*>(&infoHeader), sizeof(BITMAPINFOHEADER));

    int paddingSize = (4 - (width * 3 % 4)) % 4;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            
            // BMP files are stored bottom-up
            RGBTRIPLE pixel = (*this)[height - y - 1][x];
            file.write(reinterpret_cast<char*>(&pixel), sizeof(RGBTRIPLE));
        }
        file.seekp(paddingSize, std::ios::cur);
    }

    file.close();
    return *this;
}

GrayImage RGBImage::toGray(const ColorSpace& method = ColorSpace::HSI) {
    GrayImage grayImage(height, width);

    switch (method) {
        case ColorSpace::HSI:
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    RGBTRIPLE pixel = (*this)[y][x];
                    uint8_t gray = (pixel.rgbtRed + pixel.rgbtGreen + pixel.rgbtBlue) / 3;
                    grayImage[y][x] = gray;
                }
            }
            break;
        case ColorSpace::YCC:
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    RGBTRIPLE pixel = (*this)[y][x];
                    uint8_t gray = static_cast<uint8_t>(0.299 * pixel.rgbtRed + 0.587 * pixel.rgbtGreen + 0.114 * pixel.rgbtBlue);
                    grayImage[y][x] = gray;
                }
            }
            break;
        default:
            throw std::runtime_error("Unknown method! Supported methods are HSI and YCC.");
    }

    return grayImage;
}

RGBImage RGBImage::fromGrays(const GrayImage& bChannel, const GrayImage& gChannel, const GrayImage& rChannel) noexcept {
    RGBImage image(bChannel.height, bChannel.width);

    for (int y = 0; y < bChannel.height; y++) {
        for (int x = 0; x < bChannel.width; x++) {
            image[y][x] = { bChannel[y][x], gChannel[y][x], rChannel[y][x] };
        }
    }

    return image;
}

////////////////////////////////////////
//   OutlineRenderer implementation   //
////////////////////////////////////////

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
    RGBImage renderRectangle() {
        if (height <= 0 || width <= 0) throw std::runtime_error("Rectangle dimensions must greater than 0!");
        if (thickness <= 0) throw std::runtime_error("Thickness must be greater than 0!");
        if (thickness >= height || thickness >= width) throw std::runtime_error("Thickness must be less than the height and width!");
        if (this->y + height > image.height || this->x + width > image.width) throw std::runtime_error("Rectangle out of bounds!");

        for (int y = this->y; y < this->y + height; y++) {
            for (int x = this->x; x < this->x + width; x++) {
                if (y - this->y < thickness || x - this->x < thickness || (this->y + height - 1 - y) < thickness || (this->x + width - 1 - x) < thickness) {
                    image[y][x] = color;
                }
            }
        }
        return image;
    }

    // Render the hollow circle
    // x, y is the center
    RGBImage renderCircle() {
        for (int y = this->y - radius; y <= this->y + radius; y++) {
            for (int x = this->x - radius; x <= this->x + radius; x++) {
                if (y < 0 || y >= image.height || x < 0 || x >= image.width) continue;
                if ((y - this->y) * (y - this->y) + (x - this->x) * (x - this->x) <= radius * radius) {
                    image[y][x] = color;
                }
            }
        }
        return image;
    }

public:
    OutlineRenderer() noexcept : y(0), x(0), color({ 0, 0, 0 }), thickness(1), shape(ShapeType::NONE) {}
    OutlineRenderer(const RGBImage& image) noexcept : y(0), x(0), color({ 0, 0, 0 }), thickness(1), image(image) {}
    OutlineRenderer(const GrayImage& image) noexcept : y(0), x(0), color({ 0, 0, 0 }), thickness(1) {
        this->setImage(image);
    }

    OutlineRenderer& setPos(int y, int x) noexcept {
        this->y = y;
        this->x = x;
        return *this;
    }

    OutlineRenderer& setColor(const RGBTRIPLE& color) noexcept {
        this->color = color;
        return *this;
    }

    OutlineRenderer& setThickness(int thickness) noexcept {
        this->thickness = thickness;
        return *this;
    }

    OutlineRenderer& setImage(const RGBImage& image) noexcept {
        this->image = image;
        return *this;
    }

    OutlineRenderer& setImage(const GrayImage& image) noexcept {
        // for(int i = 0; i < image.height; i++) {
        //     for (int j = 0; j < image.width; j++) {
        //         this->image[i][j] = { image[i][j], image[i][j], image[i][j] };
        //     }
        // }
        // return *this;
        return setImage(RGBImage::fromGrays(image, image, image));
    }

    OutlineRenderer& setShape(const ShapeType& shape) noexcept {
        this->shape = shape;
        return *this;
    }

    OutlineRenderer& setDimensions(int height, int width) noexcept {
        this->height = height;
        this->width = width;
        return *this;
    }

    OutlineRenderer& setRadius(int radius) noexcept {
        this->radius = radius;
        return *this;
    }

    RGBImage render() {
        switch (shape) {
            case ShapeType::RECTANGLE:
                return renderRectangle();
            case ShapeType::CIRCLE:
                return renderCircle();
            default:
                throw std::runtime_error("Unknown shape type!");
        }
    }
};

#endif
