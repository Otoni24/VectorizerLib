#pragma once

#include <string>
#include "Vectorizer/Math.h"
#include "Vectorizer/Util.h"

namespace Vectorizer {
	List<Math::Chain> vectorizeImage(std::string path, float tolerance);
}
