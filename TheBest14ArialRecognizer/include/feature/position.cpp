#include "stdafx.h"

#include "segmentation.h"

#include "position.h";

namespace arialrec
{

namespace feature
{

namespace position
{

enum VerticalPosition : int
{
	WHOLE = 0,
	UPPER_HALF = 1000,
	LOWER_HALF = 2000
};

int verticalPositionInLine(const segmentation::Line &line, const segmentation::CharacterBox &charBox)
{
	const size_t halfRow = (line.bottomRight.row + line.topLeft.row) / 2;

	if (charBox.topLeft.row >= halfRow)
	{
		return LOWER_HALF;
	}

	if (charBox.bottomRight.row <= halfRow)
	{
		return UPPER_HALF;
	}

	return WHOLE;
}

} // namespace position

} // namespace feature

} // namespace arialrec

