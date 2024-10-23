# 快速上手

## 安装

到 [GitHub]() 下載 **ImgProcess.hpp**，然後將其放入你的專案中。

## 使用

```cpp
#include "ImgProcess.hpp"

int main() {
    RGBImage img = RGBImage::fromFile("path/to/image.bmp");
    GrayImage gray = img.toGrayImage();
    gray.toFile("path/to/gray.bmp");
    return 0;
}
```
