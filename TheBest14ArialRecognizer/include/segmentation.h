#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <vector>

namespace arialrec
{

namespace image
{
struct Image;
} // namespace image

namespace segmentation
{

struct LogicalPosition
{
	size_t row, column;

	size_t physical() const
	{
		return row * column;
	}
};

struct CharacterBox
{
	LogicalPosition topLeft, bottomRight;

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

std::vector<CharacterBox> identifyCharacterBoxes(const image::Image &img, const size_t minArea, const size_t maxArea);

} // namespace segmentation

} // namespace arielrec

#endif // SEGMENTATION_H
