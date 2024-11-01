# Matrix

`Matrix` 繼承自 `vector<vector<double>>`。

## 屬性 Attributes

+ `int height`：矩陣高度
+ `int width`：矩陣寬度

## 函數 Functions

+ `Matrix(int height, int width)`：建構子
+ `Matrix(std::initializer_list<std::initializer_list<double>> list)`：建構子

## 運算子 Operators

+ `Matrix operator+(const Matrix& other) const`：矩陣相加
+ `Matrix operator-(const Matrix& other) const`：矩陣相減
+ `Matrix operator*(const Matrix& other) const`：矩陣相乘
+ `Matrix operator+(double scalar) const`：矩陣加上常數
+ `Matrix operator-(double scalar) const`：矩陣減去常數
+ `Matrix operator*(double scalar) const`：矩陣乘以常數
+ `Matrix operator-() const`：矩陣取負
+ `Matrix transpose() const`：矩陣轉置
+ ~~`Matrix inverse() const`~~：矩陣求逆
+ `double dot(const Matrix& other) const`：矩陣內積
+ `Matrix submatrix(int y, int x, int h, int w) const`：取子矩陣

### transpose

將矩陣轉置。等效寫法：
    
```cpp
Matrix matrixA = matrixB.T();
```

### inverse

求逆矩陣，尚未實作。

### dot

計算兩矩陣的Frobenius inner product。

### submatrix

取子矩陣。自座標 (x, y) 起，向下取 h 列，向右取 w 行。

#### 參數 Parameters

+ `int y`：起點的 y 座標
+ `int x`：起點的 x 座標
+ `int h`：子矩陣的高度
+ `int w`：子矩陣的寬度

#### 回傳 Return

+ `Matrix` 物件

## 靜態函數 Static Functions

+ ~~`static Matrix identity(int n)`~~：回傳一個 n 階單位矩陣
+ ~~`static Matrix zero(int h, int w)`~~：回傳一個 h×w 的零矩陣

## 範例 Example
    
```cpp
#include <iostream>
#include "imgProcess.hpp"

int main() {
    Matrix mat(3, 3);    // 建立 3×3 矩陣
    Matrix A = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    Matrix B = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
    Matrix C = A + B;    // 矩陣相加
    Matrix D = A * B;    // 矩陣相乘
    Matrix E = A.submatrix(1, 1, 2, 2); // 子矩陣
    Matrix F = A.T();    // 矩陣轉置
    double G = A.dot(B); // 內積
    return 0;
}
```
