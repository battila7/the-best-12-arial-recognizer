#include "stdafx.h"

#include "segmentation.h"

#include "feature/area.h";

namespace arialrec
{

namespace feature
{

namespace area
{

size_t compute(const segmentation::CharacterBox &charBox)
{
	return charBox.area();
}

} // namespace area

} // namespace feature

} // namespace arialrec
