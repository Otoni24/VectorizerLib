#pragma once

#include <string>
#include "Utils/Util.h"

struct ImageData {
	std::string path;
	int width, height, channels;
	const unsigned char* data;
	bool isValid() {
		return (data != nullptr);
	}
};
namespace ImageLoader
{
	ImageData loadImageData(const std::string& path);
}