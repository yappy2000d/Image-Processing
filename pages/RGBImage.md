# RGBImage

`RGBImage` 是用來處理彩色影像的類。繼承自 `vector<vector<RGBTRIPLE>>`。

## 屬性 Attributes

+ `int width`：影像寬度
+ `int height`：影像高度

## 函數 Functions

+ `RGBImage(int width, int height)`：建構子
+ `GrayImage toGray(const string& method)`：回傳彩色影像的灰階
+ `RGBImage& toFile(const string& filename)`：將影像儲存至檔案

### toGray

根據指定的方法，回傳一個灰階影像（不會更改原始影像）。

#### 參數 Parameters

+ `const string& method`：轉換方法

##### method
['HSI', 'YCC']，預設為 HSI。

###### HSI

使用 HSI 模型轉換。

$$gray=I=\frac{1}{3}(R+G+B)$$

###### YCC

使用 YCC 模型轉換。YIQ 與 YCbCr 的 Y 值相同。

$$gary=Y=0.299\cdot{R}+0.587\cdot{G}+0.114\cdot{B}$$

#### 回傳 Return

+ `GrayImage` 物件

### toFile

將影像儲存至檔案。

:::warning
目前僅支援 24 位元深度、無壓縮的 BMP 格式。
:::

:::warning
由於 ofstream 不會自動建立資料夾，請確保目錄存在。
:::

#### 參數 Parameters

+ `const string& filename`：檔案名稱

#### 回傳 Return

+ `RGBImage&`：自身的參考

## 靜態函數 Static Functions

+ `static RGBImage fromFile(const string& filename)`：從檔案建立 RGBImage 物件

### fromFile

從圖片建立 RGBImage 物件。

:::warning
目前僅支援 24 位元深度、無壓縮的 BMP 格式。
:::


#### 參數 Parameters

+ `const string& filename`：圖片檔案路徑

#### 回傳 Return

+ `RGBImage` 物件

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
