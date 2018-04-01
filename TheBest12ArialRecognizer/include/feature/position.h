#ifndef POSITION_H
#define POSITION_H

#include "feature/types.h"

namespace arialrec
{

namespace segmentation
{
struct CharacterBox;
struct Line;
} // namespace segmentation

namespace feature
{

namespace position
{

feature_t verticalPositionInLine(const segmentation::Line &line, const segmentation::CharacterBox &charBox);

} // namespace position

} // namespace feature

} // namespace arialrec

#endif // POSITION_H
