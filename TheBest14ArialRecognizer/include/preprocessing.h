#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include "image.h"

namespace arialrec
{

namespace preprocessing
{

image::GrayscaleImage toGrayscale(image::RGBImage &img);

void toBinary(image::GrayscaleImage &img, const image::brightness_t threshold);

void withAdditiveBinaryNoise(image::GrayscaleImage &img, const int percentage);

} // namespace preprocessing

} // namespace arielrec

#endif // PREPROCESSING_H