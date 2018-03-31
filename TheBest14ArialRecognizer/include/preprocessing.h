#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include "image.h"

namespace arialrec
{

namespace preprocessing
{

void toGrayscale(image::Image &img);

void toBinary(image::Image &img, const image::brightness_t threshold);

void withAdditiveBinaryNoise(image::Image &img, const int percentage);

} // namespace preprocessing

} // namespace arielrec

#endif // PREPROCESSING_H