# RGBImage

`RGBImage` 是用來處理彩色影像的類別。本質上是一個二維的std::vector。

## 屬性 Attributes

+ `int width`：影像寬度
+ `int height`：影像高度

## 函數 Functions

+ `RGBImage(int width, int height)`：建構子
+ `toGray(const string& method)`：將彩色影像轉換為灰階影像
+ `toFile(const string& filename)`：將影像儲存至檔案

### toGray

根據指定的方法，回傳一個灰階影像（不會更改原始影像）。

#### 參數 Parameters

+ `const string& method`：轉換方法

##### method
["HSI", "YCC"]，預設為"HSI"。

###### HSI

$$gray=I=\frac{1}{3}(R+G+B)$$

###### YCC

$$gary=Y=0.299\cdot{R}+0.587\cdot{G}+0.114\cdot{B}$$

#### 回傳 Return

+ `GrayImage` 物件

### toFile

將影像儲存至檔案。

## 靜態函數 Static Functions

+ `fromFile(const string& filename)`：從檔案建立 RGBImage 物件

### fromFile

從圖片建立 RGBImage 物件。

:::warning
目前僅支援 24 位元深度、無壓縮的 BMP 格式。
:::


#### 參數 Parameters

+ `const string& filename`：圖片檔案路徑

#### 回傳 Return

+ `RGBImage` 物件

# RGBTRIPLE

`RGBTRIPLE` 結構定義了一個RGB圖片的像素。由於 BMP 格式的圖片是以 BGR 的順序儲存像素，因此 `RGBTRIPLE` 的成員變數依序代表藍色、綠色和紅色。

## 定義 Definition

```cpp
struct RGBTRIPLE {
    uint8_t rgbtBlue;
    uint8_t rgbtGreen;
    uint8_t rgbtRed;
};
```

## 範例 Example

```cpp
#include <iostream>
#include "imgProcess.hpp"

int main() {
    RGBImage img = RGBImage::fromFile("image.bmp");
    img.toFile("copy.bmp");

    int x = 0, y = 0;
    RGBTRIPLE pixel = img[y][x];
    GrayImage gray = img.toGray("YCC");

    return 0;
}
```
