-----

# VectorizerLib

A lightweight, standalone C++ library for vectorizing black and white images to generate optimized collision outlines.

This project implements the Marching Squares algorithm for contour extraction and the Ramer-Douglas-Peucker algorithm for simplification.

-----

## Features

  * **Vectorization:** Converts a bitmap image (PNG) into a series of vertex chains.
  * **Simplification:** Optimizes the generated geometry using the RDP algorithm with a customizable tolerance.
  * **Standalone:** Written in standard C++17 with minimal dependencies.
  * **CMake-friendly:** Designed to be easily integrated into other projects using `FetchContent`.

-----

## How to Use

This library is designed to be included in another CMake project using `FetchContent`.

**1. Add this to your `CMakeLists.txt`:**

```cmake
include(FetchContent)

FetchContent_Declare(
    VectorizerLib
    GIT_REPOSITORY https://github.com/Otoni24/VectorizerLib.git
    GIT_TAG        "master"
)

FetchContent_MakeAvailable(VectorizerLib)

# ... in your executable target ...
target_link_libraries(MyGame PRIVATE VectorizerLib)
```

**2. C++ Code Example:**

```cpp
#include <iostream>
#include <Vectorizer/Vectorizer.h>

int main()
{
    std::string imagePath = "collision_map.png";
    float tolerance = 2.0f;

    auto simplifiedChains = Vectorizer::vectorizeImage(imagePath, tolerance);

    std::cout << "Found " << simplifiedChains.size() << " simplified chains." << std::endl;

    // You can now use 'simplifiedChains' to create physics bodies in Box2D
    // or save them to a JSON file.
    
    return 0;
}
```

-----

## Dependencies

  * [stb\_image](https://github.com/nothings/stb) for image loading.
  * [nlohmann/json](https://github.com/nlohmann/json) for JSON handling (used by executables, not by the library itself).

-----

## License

This project is licensed under the MIT License. See the `LICENSE` file for more details.
