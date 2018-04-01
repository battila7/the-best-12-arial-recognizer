#include "stdafx.h"

#include "segmentation.h"

#include "feature/size.h"

namespace arialrec
{

namespace feature
{

namespace size
{

feature_t computeSize(const segmentation::CharacterBox &charBox)
{
	const float truncatedArea = (float)charBox.area() / 100.f;

	return (feature_t)std::exp(truncatedArea);
}

feature_t computeWidth(const segmentation::CharacterBox &charBox)
{
	return charBox.width();
}

feature_t computeHeight(const segmentation::CharacterBox &charBox)
{
	return charBox.height();
}

} // namespace size

} // namespace feature

} // namespace arialrec
