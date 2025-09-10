#pragma once

#include <string>
#include "Vectorizer/Math.h"
#include "Utils/Util.h"

namespace Vectorizer {
	List<Math::Chain> vectorizeImage(std::string path, float tolerance);
}
