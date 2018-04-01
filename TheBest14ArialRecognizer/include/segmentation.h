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
		return bottomRight.row - topLeft.row + 1;
	}

	size_t width() const
	{
		return bottomRight.column - topLeft.column + 1;
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

struct DefaultConfiguration
{
	static constexpr size_t minArea = 0;
	static constexpr size_t maxArea = 20000;
};

struct Configuration
{
	size_t minArea = DefaultConfiguration::minArea;
	size_t maxArea = DefaultConfiguration::maxArea;
};

std::vector<Line> performSegmentation(const image::GrayscaleImage &img, const Configuration &conf);

} // namespace segmentation

} // namespace arialrec

#endif // SEGMENTATION_H
