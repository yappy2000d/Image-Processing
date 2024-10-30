#ifndef IMGPROCESS_HPP
#define IMGPROCESS_HPP

#include <vector>
#include <fstream>
#include <cstdint>
#include <string>
#include <initializer_list>
#include <array>

constexpr size_t COLOR_TABLE_SIZE = 256;

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

struct Matrix : std::vector<std::vector<double>>
{
    int width, height;
    Matrix(int height, int width) noexcept;
    Matrix(std::initializer_list<std::initializer_list<double>> list);
    Matrix operator*(const Matrix& other) const;
    Matrix operator*(double scalar) const noexcept;
    friend Matrix operator*(double scalar, const Matrix& matrix) noexcept;
    Matrix operator+(const Matrix& other) const;
    Matrix operator+(double scalar) const noexcept;
    friend Matrix operator+(double scalar, const Matrix& matrix) noexcept;
    Matrix operator-(const Matrix& other) const;
    Matrix operator-() const noexcept;
    Matrix operator-(double scalar) const noexcept;
    friend Matrix operator-(double scalar, const Matrix& matrix) noexcept;
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
    RGBImage(int height, int width) noexcept;
    static RGBImage fromFile(const std::string& filename);
    RGBImage& toFile(const std::string& filename);
    GrayImage toGray(const std::string& method);
};

RGBImage drawRect(RGBImage image, int y, int x, int h, int w, RGBTRIPLE color) noexcept;
RGBImage drawRect(GrayImage image, int y, int x, int h, int w, RGBTRIPLE color) noexcept;

///////////////////////////////////////
//      Matrix implementation        //
///////////////////////////////////////

Matrix::Matrix(int height, int width) noexcept : std::vector<std::vector<double>>(height, std::vector<double>(width)), height(height), width(width) {}
Matrix::Matrix(std::initializer_list<std::initializer_list<double>> list) : std::vector<std::vector<double>>(list.size(), std::vector<double>(list.begin()->size())), width(list.begin()->size()), height(list.size()) {
    int y = 0;
    for (auto row : list) {
        if (row.size() != width) throw std::runtime_error("All rows must have the same length!");
        int x = 0;
        for (auto value : row) {
            (*this)[y][x] = value;
            x++;
        }
        y++;
    }
}

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

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
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

    std::ofstream file(filename, std::ios::binary);

    if (!file) throw std::runtime_error("Failed to open the file: " + filename);

    file.write(reinterpret_cast<char*>(&fileHeader), sizeof(BITMAPFILEHEADER));
    file.write(reinterpret_cast<char*>(&infoHeader), sizeof(BITMAPINFOHEADER));
    
    // Color table(Mandatory for color depths ≤ 8 bits)
    std::array<uint8_t, COLOR_TABLE_SIZE * 4> colorTable;
    for (int i = 0; i < COLOR_TABLE_SIZE; ++i) {
        colorTable[i * 4 + 0] = static_cast<uint8_t>(i); // Blue
        colorTable[i * 4 + 1] = static_cast<uint8_t>(i); // Green
        colorTable[i * 4 + 2] = static_cast<uint8_t>(i); // Red
        colorTable[i * 4 + 3] = 0;                       // Alpha(Reserved)
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
            image[y][x] = matrix[y][x];
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
    int paddingSize = (4 - (width * 3 % 4)) % 4;

    std::ofstream file(filename, std::ios::binary);

    if (!file) throw std::runtime_error("Failed to open the file: " + filename);

    file.write(reinterpret_cast<char*>(&fileHeader), sizeof(BITMAPFILEHEADER));
    file.write(reinterpret_cast<char*>(&infoHeader), sizeof(BITMAPINFOHEADER));

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

GrayImage RGBImage::toGray(const std::string& method="HSI") {
    GrayImage grayImage(height, width);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            RGBTRIPLE pixel = (*this)[y][x];
            uint8_t gray = 0;
            if (method == "HSI") {
                gray = (pixel.rgbtRed + pixel.rgbtGreen + pixel.rgbtBlue) / 3;
            } else if (method == "YCC") {
                gray = static_cast<uint8_t>(0.299 * pixel.rgbtRed + 0.587 * pixel.rgbtGreen + 0.114 * pixel.rgbtBlue);
            } else {
                throw std::runtime_error("Unknown method! Supported methods are HSI and YCC.");
            }
            grayImage[y][x] = gray;
        }
    }

    return grayImage;
}

////////////////////////////////////////
//      Fuunction implementation      //
////////////////////////////////////////

RGBImage drawRect(RGBImage image, int y, int x, int h, int w, RGBTRIPLE color) noexcept {
    for (int i = 0; i < w; i++) {
        image[y][x + i] = color;
        image[y + h - 1][x + i] = color;
    }
    for (int i = 0; i < h; i++) {
        image[y + i][x] = color;
        image[y + i][x + w - 1] = color;
    }
    return image;
}

RGBImage drawRect(GrayImage image, int y, int x, int h, int w, RGBTRIPLE color) noexcept {
    RGBImage result(image.height, image.width);

    for(int i = 0; i < image.height; i++) {
        for (int j = 0; j < image.width; j++) {
            result[i][j] = { image[i][j], image[i][j], image[i][j] };
        }
    }

    for(int i = 0; i < w; i++) {
        result[y][x + i] = color;
        result[y + h - 1][x + i] = color;
    }

    for(int i = 0; i < h; i++) {
        result[y + i][x] = color;
        result[y + i][x + w - 1] = color;
    }

    return result;
}

#endif
