# RGBImage

`RGBImage` 是用來處理彩色影像的類別。本質上是一個二維的std::vector。

## 屬性 Attributes

+ `int width`：影像寬度
+ `int height`：影像高度

## 函數 Functions

+ `RGBImage(int width, int height)`：建構子
+ `toGrayImage(const char* method)`：將彩色影像轉換為灰階影像
+ `toFile(const char* filename)`：將影像儲存至檔案

### toGrayImage

將彩色影像轉換為灰階影像。

#### 參數 Parameters

+ `const char* method`：轉換方法

##### method：["HSI", "YCC"]，預設為"HSI"

###### HSI

$$gray=I=\frac{1}{3}(R+G+B)$$

###### YCC

$$gary=Y=gray=I=0.299\cdot{R}+0.587\cdot{G}+0.114\cdot{B}$$

#### 回傳 Return

## 靜態函數 Static Functions

+ `fromFile(const char* filename)`：從檔案建立 RGBImage 物件

### fromFile

從圖片建立 RGBImage 物件。

:::warning
目前僅支援 24 位元深度、無壓縮的 BMP 格式。
:::


#### 參數 Parameters

+ `const char* filename`：圖片檔案路徑

#### 回傳 Return

+ `RGBImage` 物件

## 範例 Example

```cpp
```