#include "stdafx.h"

#include "segmentation.h"

#include "feature/area.h"

namespace arialrec
{

namespace feature
{

namespace area
{

feature_t compute(const segmentation::CharacterBox &charBox)
{
	const int truncatedArea = (int)charBox.area() / 10;

	return (feature_t)std::exp(truncatedArea);
}

} // namespace area

} // namespace feature

} // namespace arialrec
