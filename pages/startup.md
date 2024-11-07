# 開始

## 下載

到 [GitHub](https://github.com/yappy2000d/Image-Processsing/blob/main/ImgProc.hpp) 下載 **ImgProc.hpp**，然後將其放入你的專案中。

## 使用

```cpp
#include "ImgProc.hpp"

int main() {
    RGBImage img = RGBImage::fromFile("path/to/image.bmp");
    GrayImage gray = img.toGrayImage();
    gray.toFile("path/to/gray.bmp");
    return 0;
}
```
