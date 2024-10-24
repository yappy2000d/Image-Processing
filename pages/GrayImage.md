# GrayImage

`GrayImage` 是用來處理灰階影像的類。繼承自 `vector<vector<uint8_t>>`。

## 屬性 Attributes

+ `int width`：影像寬度
+ `int height`：影像高度

## 函數 Functions

+ `GrayImage(int width, int height)`：建構子
+ `GrayImage& toFile(const string& filename)`：將影像儲存至檔案
+ `Matrix toMatrix()`：回傳影像矩陣

## toFile

將影像儲存至檔案。

:::warning
目前僅支援 8 位元深度、無壓縮的 BMP 格式。
:::

:::warning
由於 ofstream 不會自動建立資料夾，請確保目錄存在。
:::

## 參數 Parameters

+ `const string& filename`：檔案名稱

## 回傳 Return

+ `GrayImage&`：自身的參考

## toMatrix

回傳影像矩陣。

## 靜態函數 Static Functions

+ `static GrayImage fromMatrix(const Matrix& matrix)`：從矩陣建立 GrayImage 物件

## 範例 Example

```cpp
#include <iostream>
#include "imgProcess.hpp"

int main() {
    GrayImage img = RGBImage::fromFile("image.bmp").toGray();
    img.toFile("gray.bmp");

    int x = 0, y = 0;
    uint8_t pixel = img[y][x];
    
    Matrix matrix = img.toMatrix();
    img = GrayImage::fromMatrix(matrix);

    return 0;
}
```