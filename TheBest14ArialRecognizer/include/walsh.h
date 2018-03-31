#ifndef WALSH_H
#define WALSH_H

namespace arialrec
{

namespace image
{
struct GrayscaleImage;
} // namespace image

namespace walsh
{

std::vector<int> computeWalshValues(const image::GrayscaleImage &img);

} // namespace walsh

} // namespace arialrec

#endif // WALSH_H
