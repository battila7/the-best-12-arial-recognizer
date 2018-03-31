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

static bool hasBlackInRow(const image::Image &img, const size_t row)
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

static std::optional<size_t> nextEmptyRow(const image::Image &img, const size_t startingRow = 0)
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

static std::optional<size_t> nextNonEmptyRow(const image::Image &img, const size_t startingRow = 0)
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

static std::optional<size_t> nextEmptyColumn(const image::Image &img, const size_t lowerRow, const size_t upperRow, const size_t startingColumn)
{
	for (size_t column = startingColumn; column < img.width; ++column)
	{
		bool hasBlack = false;

		for (size_t row = lowerRow; row < upperRow; ++row)
		{
			hasBlack = isBlack(img.data[row * img.width + column]);
		}

		if (!hasBlack)
		{
			return { column };
		}
	}

	return {};
}

static std::optional<size_t> nextNonEmptyColumn(const image::Image &img, const size_t lowerRow, const size_t upperRow, const size_t startingColumn)
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

static std::pair<size_t, size_t> segmentBetweenRows(const image::Image &img, const size_t startingRow, const size_t endingRow, std::vector<CharacterBox> &characters)
{
	size_t column = 0;

	while (column < img.width)
	{
		if (const auto leftColumn = nextNonEmptyColumn(img, startingRow, endingRow, column); leftColumn.has_value())
		{
			if (const auto rightColumn = nextEmptyColumn(img, startingRow, endingRow, leftColumn.value()); rightColumn.has_value())
			{
				characters.push_back({ { leftColumn.value(), startingRow }, { rightColumn.value(), endingRow } });

				column = rightColumn.value();
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
}

static std::pair<Line, size_t> segmentLine(const image::Image &img, const size_t startingRow)
{
	Line line;

	const std::optional<size_t> emptyRow = nextEmptyRow(img, startingRow);

	if (emptyRow.has_value())
	{
		const auto [startingColumn, endingColumn] = segmentBetweenRows(img, startingRow, emptyRow.value() - 1, line.characters);

		line.topLeft = { startingColumn, startingRow };
		line.bottomRight = { endingColumn, emptyRow.value() - 1 };
	}

	return { line, emptyRow.value_or(startingRow + 1) };
}

std::vector<Line> identifyCharacterBoxes(const image::Image &img, const size_t minArea, const size_t maxArea)
{
	std::vector<Line> lines;

	if (img.componentCount != image::ComponentCount::SINGLE)
	{
		return lines;
	}

	size_t row = 0;

	while (row < img.height)
	{
		if (const auto nonEmptyRow = nextNonEmptyRow(img, row); nonEmptyRow.has_value())
		{
			const auto [line, endRow] = segmentLine(img, nonEmptyRow.value());

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

} // namespace arielrec
