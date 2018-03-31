#include "stdafx.h"

#include "segmentation.h"

#include "feature/area.h"

namespace arialrec
{

namespace feature
{

namespace area
{

int compute(const segmentation::CharacterBox &charBox)
{
	const int truncatedArea = (int)charBox.area() / 10;

	return (int)std::exp(truncatedArea);
}

} // namespace area

} // namespace feature

} // namespace arialrec
