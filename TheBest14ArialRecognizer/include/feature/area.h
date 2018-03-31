#ifndef AREA_H
#define AREA_H


namespace arialrec
{

namespace segmentation
{
struct CharacterBox;
} // namespace segmentation

namespace feature
{

namespace area
{

size_t compute(const segmentation::CharacterBox &charBox);

} // namespace area

} // namespace feature

} // namespace arialrec

#endif // AREA_H
