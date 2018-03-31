#include "stdafx.h"

#include "segmentation.h"

namespace arialrec
{

namespace segmentation
{

using image::LogicalPosition;

static inline bool isBlack(const image::brightness_t value)
{
	return value == image::MIN_BRIGHTNESS_VALUE;
}

static bool hasBlackInRow(const image::GrayscaleImage &img, const size_t row)
{
	for (size_t column = 0; column < img.width; ++column)
	{
		if (isBlack(img.data[row * img.width + column]))
		{
			return true;
		}
	}

	return false;
}

static std::optional<size_t> nextEmptyRow(const image::GrayscaleImage &img, const size_t startingRow = 0)
{
	for (size_t row = startingRow; row < img.height; ++row)
	{
		if (!hasBlackInRow(img, row))
		{
			return { row };
		}
	}

	return {};
}

static std::optional<size_t> nextNonEmptyRow(const image::GrayscaleImage &img, const size_t startingRow = 0)
{
	for (size_t row = startingRow; row < img.height; ++row)
	{
		if (hasBlackInRow(img, row))
		{
			return { row };
		}
	}

	return {};
}

static std::optional<size_t> nextEmptyColumn(const image::GrayscaleImage &img, const size_t lowerRow, const size_t upperRow, const size_t startingColumn)
{
	for (size_t column = startingColumn; column < img.width; ++column)
	{
		bool hasBlack = false;

		for (size_t row = lowerRow; row < upperRow; ++row)
		{
			hasBlack |= isBlack(img.data[row * img.width + column]);
		}

		if (!hasBlack)
		{
			return { column };
		}
	}

	return {};
}

static std::optional<size_t> nextNonEmptyColumn(const image::GrayscaleImage &img, const size_t lowerRow, const size_t upperRow, const size_t startingColumn)
{
	for (size_t column = startingColumn; column < img.width; ++column)
	{
		for (size_t row = lowerRow; row < upperRow; ++row)
		{
			if (isBlack(img.data[row * img.width + column]))
			{
				return { column };
			}
		}
	}

	return {};
}

static void cropCharacter(const image::GrayscaleImage img, CharacterBox &box)
{
	for (size_t row = box.topLeft.row; row < box.bottomRight.row; ++row)
	{
		for (size_t column = box.topLeft.column; column < box.bottomRight.column; ++column)
		{
			if (isBlack(img.data[row * img.width + column]))
			{
				box.topLeft.row = row;
				goto searchFromTheBottom;
			}
		}
	}

	searchFromTheBottom:
	for (size_t row = box.bottomRight.row; row > box.topLeft.row; --row)
	{
		for (size_t column = box.topLeft.column; column < box.bottomRight.column; ++column)
		{
			if (isBlack(img.data[row * img.width + column]))
			{
				box.bottomRight.row = row;
				return;
			}
		}
	}
}

static std::pair<size_t, size_t> segmentBetweenRows(const image::GrayscaleImage &img, const size_t startingRow, const size_t endingRow, std::vector<CharacterBox> &characters)
{
	std::optional<size_t> startingColumn;
	size_t column = 0, endingColumn = 0;

	while (column < img.width)
	{
		if (const auto leftColumn = nextNonEmptyColumn(img, startingRow, endingRow, column); leftColumn.has_value())
		{
			if (!startingColumn.has_value())
			{
				startingColumn = { leftColumn.value() };
			}

			if (const auto rightColumn = nextEmptyColumn(img, startingRow, endingRow, leftColumn.value()); rightColumn.has_value())
			{
				characters.push_back({ { startingRow, leftColumn.value() }, { endingRow, rightColumn.value() - 1 } });

				column = rightColumn.value();

				endingColumn = column;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	return { startingColumn.value_or(0), endingColumn };
}

static std::pair<Line, size_t> segmentLine(const image::GrayscaleImage &img, const size_t startingRow)
{
	Line line;

	const std::optional<size_t> emptyRow = nextEmptyRow(img, startingRow);

	if (emptyRow.has_value())
	{
		const auto [startingColumn, endingColumn] = segmentBetweenRows(img, startingRow, emptyRow.value() - 1, line.characters);

		line.topLeft = { startingRow, startingColumn };
		line.bottomRight = { emptyRow.value() - 1, endingColumn };
	}

	return { line, emptyRow.value_or(startingRow + 1) };
}

static void filterByArea(std::vector<CharacterBox> &characters, const size_t minArea, const size_t maxArea)
{
	characters.erase(std::remove_if(characters.begin(), characters.end(), [minArea, maxArea](const CharacterBox &box)
	{
		return (box.area() <= minArea) || (box.area() >= maxArea);
	}), characters.end());
}

std::vector<Line> performSegmentation(const image::GrayscaleImage &img, const size_t minArea, const size_t maxArea)
{
	std::vector<Line> lines;

	size_t row = 0;

	while (row < img.height)
	{
		if (const auto nonEmptyRow = nextNonEmptyRow(img, row); nonEmptyRow.has_value())
		{
			auto [line, endRow] = segmentLine(img, nonEmptyRow.value());

			filterByArea(line.characters, minArea, maxArea);

			std::for_each(line.characters.begin(), line.characters.end(), [img](CharacterBox &box) { cropCharacter(img, box); });

			lines.push_back(line);

			row = endRow + 1;
		}
		else
		{
			break;
		}
	}

	return lines;
}

} // segmentation

} // namespace arialrec
