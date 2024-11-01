# OutlineRenderer

一個繪製邊框的工廠類。

## 屬性 Attributes

無公有屬性。

## 函數 Functions

+ `OutlineRenderer()`：建構子
+ `OutlineRenderer(const GrayImage& img)`：建構子
+ `OutlineRenderer(const RGBImage& img)`：建構子
+ `OutlineRenderer& setPos(int y, int x)`：設定位置
+ `OutlineRenderer& setColor(const RGBTRIPLE& color)`：設定邊框顏色
+ `OutlineRenderer& setThickness(int thickness)`：設定邊框粗細
+ `OutlineRenderer& setImage(const RGBImage& image)`：設定圖片
+ `OutlineRenderer& setImage(const GrayImage& image)`：設定圖片
+ `OutlineRenderer& setShape(const ShapeType& shape)`：設定形狀
+ `OutlineRenderer& setDimensions(int height, int width)`：設定尺寸
+ `OutlineRenderer& setRadius(int radius)`：設定半徑
+ `OutlineRenderer& render()`：繪製邊框

### setPos

當ShapeType為`ShapeType::RECTANGLE`時，設定左上角的座標。當ShapeType為`ShapeType::CIRCLE`時，設定圓心的座標。

### setColor

設定邊框的顏色。

### setShape

設定形狀。

+ `ShapeType::RECTANGLE`：矩形
+ `ShapeType::CIRCLE`：圓形

### setDimensions

設定矩形的高度和寬度。在設定形狀為`ShapeType::RECTANGLE`時使用。

### setRadius

設定圓形的半徑。在設定形狀為`ShapeType::CIRCLE`時使用。

## 範例 Example

```cpp
RGBImage img(300, 300);

OutlineRenderer renderer;
renderer.setImage(img)
        .setShape(ShapeType::RECTANGLE)
        .setPos(50, 50)
        .setDimensions(100, 100)
        .setColor({255, 0, 0})
        .setThickness(5)
        .render()
        .toFile("rectangle.bmp");
```
