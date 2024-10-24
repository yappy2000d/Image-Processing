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