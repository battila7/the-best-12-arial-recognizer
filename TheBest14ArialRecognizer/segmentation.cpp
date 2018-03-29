#include "stdafx.h"
#include "image.h"

#include "segmentation.h"

namespace arialrec
{

namespace segmentation
{

std::vector<CharacterBox> identifyCharacterBoxes(const image::Image &img, const size_t minArea, const size_t maxArea)
{
	if (img.componentCount != image::ComponentCount::SINGLE)
	{
		return std::vector<CharacterBox>();
	}

	return std::vector<CharacterBox>();
}

} // segmentation

} // namespace arielrec
