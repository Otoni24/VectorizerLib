#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "ImageLoader.h"

ImageData ImageLoader::loadImageData(const std::string& path)
{
	int width, height, channels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	return ImageData{ path, width, height, channels, data };
}