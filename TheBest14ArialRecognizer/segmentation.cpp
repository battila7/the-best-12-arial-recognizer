#include "stdafx.h"

#include "segmentation.h"

namespace arialrec
{

namespace segmentation
{

std::vector<Line> identifyCharacterBoxes(const image::Image &img, const size_t minArea, const size_t maxArea)
{
	if (img.componentCount != image::ComponentCount::SINGLE)
	{
		return std::vector<Line>();
	}

	return std::vector<Line>();
}

} // segmentation

} // namespace arielrec
