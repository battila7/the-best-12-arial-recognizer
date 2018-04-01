#ifndef SIZE_H
#define SIZE_H

#include "feature/types.h"

namespace arialrec
{

namespace segmentation
{
struct CharacterBox;
} // namespace segmentation

namespace feature
{

namespace size
{

feature_t computeSize(const segmentation::CharacterBox &charBox);

feature_t computeWidth(const segmentation::CharacterBox &charBox);

feature_t computeHeight(const segmentation::CharacterBox &charBox);

} // namespace size

} // namespace feature

} // namespace arialrec

#endif // SIZE_H
