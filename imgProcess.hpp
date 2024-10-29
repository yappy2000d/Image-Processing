#ifndef IMGPROCESS_HPP
#define IMGPROCESS_HPP

#include <vector>
#include <fstream>
#include <cstdint>
#include <initializer_list>

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
    Matrix(int height, int width);
    Matrix(std::initializer_list<std::initializer_list<double>> list);
    Matrix operator*(const Matrix& other) const;
    Matrix operator*(double scalar) const;
    friend Matrix operator*(double scalar, const Matrix& matrix);
    Matrix operator+(const Matrix& other) const;
    Matrix operator+(double scalar) const;
    friend Matrix operator+(double scalar, const Matrix& matrix);
    Matrix operator-(const Matrix& other) const;
    Matrix operator-() const;
    Matrix operator-(double scalar) const;
    friend Matrix operator-(double scalar, const Matrix& matrix);
    Matrix transpose() const;
    Matrix T() const;
    double dot(const Matrix& other) const;
    Matrix submatrix(int y, int x, int h, int w) const;
};

struct GrayImage : std::vector<std::vector<uint8_t>>
{
    int width, height;
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    GrayImage(int height, int width);
    GrayImage& toFile(const std::string& filename);
    Matrix toMatrix() const;
    static GrayImage fromMatrix(const Matrix& matrix);
};

struct RGBImage : std::vector<std::vector<RGBTRIPLE>>
{
    int width, height;
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    RGBImage(int height, int width);
    static RGBImage fromFile(const std::string& filename);
    RGBImage& toFile(const std::string& filename);
    GrayImage toGray(const std::string& method);
};

RGBImage makeRectangle(RGBImage image, int y, int x, int h, int w, RGBTRIPLE color);
RGBImage makeRectangle(GrayImage image, int y, int x, int h, int w, RGBTRIPLE color);

///////////////////////////////////////
//      Matrix implementation        //
///////////////////////////////////////

Matrix::Matrix(int height, int width) : std::vector<std::vector<double>>(height, std::vector<double>(width)), height(height), width(width) {}
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
    if (width != other.height) throw std::runtime_error("Cannot multiply matrices with incompatible shapes! Got " + std::to_string(width) + "x" + std::to_string(height) + " and " + std::to_string(other.width) + "x" + std::to_string(other.height));

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

Matrix Matrix::operator*(double scalar) const {
    Matrix result(height, width);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            result[y][x] = (*this)[y][x] * scalar;
        }
    }

    return result;
}

Matrix operator*(double scalar, const Matrix& matrix) {
    return matrix * scalar;
}

Matrix Matrix::operator+(const Matrix& other) const {
    if (width != other.width || height != other.height) throw std::runtime_error("Cannot add matrices with different shapes! Got " + std::to_string(width) + "x" + std::to_string(height) + " and " + std::to_string(other.width) + "x" + std::to_string(other.height));

    Matrix result(height, width);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            result[y][x] = (*this)[y][x] + other[y][x];
        }
    }

    return result;
}

Matrix Matrix::operator+(double scalar) const {
    Matrix result(height, width);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            result[y][x] = (*this)[y][x] + scalar;
        }
    }

    return result;
}

Matrix operator+(double scalar, const Matrix& matrix) {
    return matrix + scalar;
}

Matrix Matrix::operator-(const Matrix& other) const {
    if (width != other.width || height != other.height) throw std::runtime_error("Cannot subtract matrices with different shapes! Got " + std::to_string(width) + "x" + std::to_string(height) + " and " + std::to_string(other.width) + "x" + std::to_string(other.height));

    Matrix result(height, width);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            result[y][x] = (*this)[y][x] - other[y][x];
        }
    }

    return result;
}

Matrix Matrix::operator-() const {
    Matrix result(height, width);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            result[y][x] = -(*this)[y][x];
        }
    }

    return result;
}

Matrix Matrix::operator-(double scalar) const {
    return *this + (-scalar);
}

Matrix operator-(double scalar, const Matrix& matrix) {
    return scalar + (-matrix);
}

Matrix Matrix::transpose() const {
    Matrix result(width, height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            result[x][y] = (*this)[y][x];
        }
    }

    return result;
}

Matrix Matrix::T() const {
    return transpose();
}

// Frobenius inner product
double Matrix::dot(const Matrix& other) const {
    if (width != other.width || height != other.height) throw std::runtime_error("Cannot calculate dot product of matrices with different shapes! Got " + std::to_string(width) + "x" + std::to_string(height) + " and " + std::to_string(other.width) + "x" + std::to_string(other.height));

    double sum = 0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            sum += (*this)[y][x] * other[y][x];
        }
    }

    return sum;
}

Matrix Matrix::submatrix(int y, int x, int h, int w) const {
    if (y + h > height || x + w > width) throw std::runtime_error("Submatrix out of bounds! Matrix size is " + std::to_string(width) + "x" + std::to_string(height) + ", requested submatrix size is " + std::to_string(w) + "x" + std::to_string(h) + " at position " + std::to_string(x) + "," + std::to_string(y));

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

GrayImage::GrayImage(int height, int width) : std::vector<std::vector<uint8_t>>(height, std::vector<uint8_t>(width)), height(height), width(width)
{
    fileHeader = {
        .bfType = 0x4D42,
        .bfSize = static_cast<uint32_t>(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + height * ((width + 3) & ~3)),
        .bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
    };

    infoHeader = {
        .biSize = sizeof(BITMAPINFOHEADER),
        .biWidth = width,
        .biHeight = height,
        .biPlanes = 1,
        .biBitCount = 8,
    };
}

GrayImage& GrayImage::toFile(const std::string& filename) {

    std::ofstream file(filename, std::ios::binary);

    if (!file) throw std::runtime_error("Failed to open the file: " + filename);

    file.write(reinterpret_cast<char*>(&fileHeader), sizeof(BITMAPFILEHEADER));
    file.write(reinterpret_cast<char*>(&infoHeader), sizeof(BITMAPINFOHEADER));
    
    // Color table(Mandatory for color depths ≤ 8 bits)
    for (int i = 0; i < 256; ++i) {
        uint8_t color[4] = { static_cast<uint8_t>(i), static_cast<uint8_t>(i), static_cast<uint8_t>(i), 0 };
        file.write(reinterpret_cast<char*>(color), sizeof(color));
    }

    int padding = (4 - (width % 4)) % 4;
    for (int y = 0; y < height; ++y) {
        file.write(reinterpret_cast<char*>(this->at(y).data()), width);
        uint8_t pad[3] = { 0 };
        file.write(reinterpret_cast<char*>(pad), padding);
    }

    file.close();
    return *this;
}

Matrix GrayImage::toMatrix() const {
    Matrix matrix(height, width);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            matrix[y][x] = (*this)[y][x];
        }
    }

    return matrix;
}

GrayImage GrayImage::fromMatrix(const Matrix& matrix) {
    GrayImage image(matrix.height, matrix.width);

    for (int y = 0; y < matrix.height; y++) {
        for (int x = 0; x < matrix.width; x++) {
            image[y][x] = matrix[y][x];
        }
    }

    return image;
}

RGBImage::RGBImage(int height, int width) : std::vector<std::vector<RGBTRIPLE>>(height, std::vector<RGBTRIPLE>(width)), height(height), width(width)
{
    int paddingSize = (4 - (width * 3 % 4)) % 4;

    fileHeader = {
        .bfType = 0x4D42,
        .bfSize = static_cast<uint32_t>(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + height * width * 3 + height * paddingSize),
        .bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
    };

    infoHeader = {
        .biSize = sizeof(BITMAPINFOHEADER),
        .biWidth = width,
        .biHeight = height,
        .biPlanes = 1,
        .biBitCount = 24,
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
            image[y][x] = pixel;
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
            RGBTRIPLE pixel = (*this)[y][x];
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
                gray = 0.299 * pixel.rgbtRed + 0.587 * pixel.rgbtGreen + 0.114 * pixel.rgbtBlue;
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

RGBImage makeRectangle(RGBImage image, int y, int x, int h, int w, RGBTRIPLE color) {
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

RGBImage makeRectangle(GrayImage image, int y, int x, int h, int w, RGBTRIPLE color) {
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
