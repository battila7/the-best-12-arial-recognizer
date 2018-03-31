#ifndef POSITION_H
#define POSITION_H

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

int verticalPositionInLine(const segmentation::CharacterBox &charBox);

} // namespace position

} // namespace feature

} // namespace arialrec

#endif // POSITION_H
