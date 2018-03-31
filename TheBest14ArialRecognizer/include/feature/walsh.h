#ifndef WALSH_H
#define WALSH_H

#include "image.h"

namespace arialrec
{

namespace segmentation
{
struct CharacterBox;
} // namespace segmentation

namespace feature
{

namespace walsh
{

std::vector<int> computeWalshValues(const image::GrayscaleImage &img, const segmentation::CharacterBox &charBox);

} // namespace walsh

} // namespace feature

} // namespace arialrec

#endif // WALSH_H
