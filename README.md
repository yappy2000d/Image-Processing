<p align="right"><b>[English] / <a href="">[中文]</a></b></p>
 
# Image Processing Library

![C++11](https://img.shields.io/badge/C++-std11%20-blue.svg?style=flat-square)
![License](https://img.shields.io/badge/License-MIT-blue.svg?style=flat-square)
![Docs](https://img.shields.io/badge/Docs-中文-blue.svg?style=flat-square)

This library is designed for image processing implementation courses, focusing on simplifying file reading/writing and array operations. This allows students to concentrate on developing image processing algorithms without having to deal with complex image formats.

## Why Not Use OpenCV?

OpenCV is a powerful image processing library that provides many ready-to-use algorithms. Therefore, course assignments often prohibit the use of OpenCV to ensure that students learn the fundamental principles of image processing.

## Features

- **Lightweight**: This library offers only the most basic functionalities, excluding any complex algorithms.
- **Portability**: This library uses only the standard library, making it compatible with any compiler that supports C++11.

## Comparison with Windows C++/CLI

| Feature       | Windows C++/CLI | Image Processing |
|---------------|------------------|------------------|
| Language      | C++/CLI          | Pure C++11       |
| GUI           | ✅               | ❌               |
| Portability   | ❌               | ✅               |
| Open Source   | ❌               | ✅               |

To maintain simplicity, this library does not provide a GUI interface; it only offers file reading/writing and array operation functionalities. Instead of using Windows C++/CLI to call the .NET Framework, it may be better to use C#. If you want to create a GUI application, Qt or Tk would be better options for portability.
