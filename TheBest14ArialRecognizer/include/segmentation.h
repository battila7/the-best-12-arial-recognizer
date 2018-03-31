#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <vector>

#include "image.h"

namespace arialrec
{

namespace segmentation
{

struct CharacterBox
{
	image::LogicalPosition topLeft, bottomRight;

	size_t height() const
	{
		return bottomRight.row - topLeft.row;
	}

	size_t width() const
	{
		return bottomRight.column - topLeft.column;
	}

	size_t area() const
	{
		return width() * height();
	}
};


struct Line
{
	image::LogicalPosition topLeft, bottomRight;

	std::vector<CharacterBox> characters;
};

std::vector<Line> identifyCharacterBoxes(const image::Image &img, const size_t minArea, const size_t maxArea);

} // namespace segmentation

} // namespace arielrec

#endif // SEGMENTATION_H
