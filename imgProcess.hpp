#ifndef IMGPROCESS_HPP
#define IMGPROCESS_HPP

#include <vector>
#include <fstream>
#include <cstdint>

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

    Matrix(int width, int height) : std::vector<std::vector<double>>(height, std::vector<double>(width)), width(width), height(height) {}

    Matrix operator*(const Matrix& other) const {
        if (width != other.height) throw std::runtime_error("Matrix dimensions do not match!");

        Matrix result(other.width, height);

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

    Matrix operator*(double scalar) const {
        Matrix result(width, height);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                result[y][x] = (*this)[y][x] * scalar;
            }
        }

        return result;
    }

    friend Matrix operator*(double scalar, const Matrix& matrix) {
        return matrix * scalar;
    }

    Matrix operator+(const Matrix& other) const {
        if (width != other.width || height != other.height) throw std::runtime_error("Matrix dimensions do not match!");

        Matrix result(width, height);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                result[y][x] = (*this)[y][x] + other[y][x];
            }
        }

        return result;
    }

    Matrix operator+(double scalar) const {
        Matrix result(width, height);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                result[y][x] = (*this)[y][x] + scalar;
            }
        }

        return result;
    }

    friend Matrix operator+(double scalar, const Matrix& matrix) {
        return matrix + scalar;
    }

    Matrix operator-(const Matrix& other) const {
        if (width != other.width || height != other.height) throw std::runtime_error("Matrix dimensions do not match!");

        Matrix result(width, height);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                result[y][x] = (*this)[y][x] - other[y][x];
            }
        }

        return result;
    }

    Matrix operator-(double scalar) const {
        return *this + (-scalar);
    }

    friend Matrix operator-(double scalar, const Matrix& matrix) {
        return (-scalar) + matrix;
    }

    Matrix transpose() const {
        Matrix result(height, width);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                result[y][x] = (*this)[x][y];
            }
        }

        return result;
    }

    Matrix T() const {
        return transpose();
    }

    // Frobenius inner product
    double dot(const Matrix& other) const {
        if (width != other.width || height != other.height) throw std::runtime_error("Matrix dimensions do not match!");

        double sum = 0;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                sum += (*this)[y][x] * other[y][x];
            }
        }

        return sum;
    }

    Matrix submatrix(int y, int x, int h, int w) const {
        if (y + h > height || x + w > width) throw std::runtime_error("Submatrix out of bounds!");

        Matrix result(w, h);

        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                result[i][j] = (*this)[y + i][x + j];
            }
        }

        return result;
    }
};


struct GrayImage : std::vector<std::vector<uint8_t>>
{
    int width, height;
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    
    GrayImage(int width, int height) : std::vector<std::vector<uint8_t>>(height, std::vector<uint8_t>(width)), width(width), height(height)
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

    GrayImage& toFile(const char* filename) {

        std::ofstream file(filename, std::ios::binary);

        if (!file) throw std::runtime_error("Could not open the image file!");

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

    Matrix toMatrix() const {
        Matrix matrix(width, height);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                matrix[y][x] = (*this)[y][x];
            }
        }

        return matrix;
    }

    static GrayImage fromMatrix(const Matrix& matrix) {
        GrayImage image(matrix.width, matrix.height);

        for (int y = 0; y < matrix.height; y++) {
            for (int x = 0; x < matrix.width; x++) {
                image[y][x] = matrix[y][x];
            }
        }

        return image;
    }
};

struct RGBImage : std::vector<std::vector<RGBTRIPLE>>
{
    int width, height;
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    
    RGBImage(int width, int height) : std::vector<std::vector<RGBTRIPLE>>(height, std::vector<RGBTRIPLE>(width)), width(width), height(height)
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

    static RGBImage fromFile(const char* filename) {
        BITMAPFILEHEADER fileHeader;
        BITMAPINFOHEADER infoHeader;
        
        std::ifstream file(filename, std::ios::binary);
        
        if (!file) throw std::runtime_error("Could not open the image file!");

        file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BITMAPFILEHEADER));
        file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BITMAPINFOHEADER));

        if(infoHeader.biCompression != BI_RGB) throw std::runtime_error("Only uncompressed BMP files are supported!");

        int width = infoHeader.biWidth;
        int height = infoHeader.biHeight;
        int paddingSize = (4 - (width * 3 % 4)) % 4;

        RGBImage image(width, height);

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

    RGBImage& toFile(const char* filename) {
        int paddingSize = (4 - (width * 3 % 4)) % 4;

        std::ofstream file(filename, std::ios::binary);

        if (!file) throw std::runtime_error("Could not open the image file!");

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

    GrayImage toGrayImage(const char* method="HSI") {
        GrayImage grayImage(width, height);

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
};

#endif
