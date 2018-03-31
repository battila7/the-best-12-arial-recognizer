#ifndef POSITION_H
#define POSITION_H

#include "feature/types.h"

namespace arialrec
{

namespace segmentation
{
struct CharacterBox;
} // namespace segmentation

namespace feature
{

namespace position
{

feature_t verticalPositionInLine(const segmentation::CharacterBox &charBox);

} // namespace position

} // namespace feature

} // namespace arialrec

#endif // POSITION_H
