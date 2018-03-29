#include "stdafx.h"
#include "image.h"

#include "preprocessing.h"

static constexpr float R_WEIGHT = 0.3f;
static constexpr float G_WEIGHT = 0.59f;
static constexpr float B_WEIGHT = 0.11f;

static constexpr int MIN_RGB_VALUE = 0;
static constexpr int MAX_RGB_VALUE = 255;

namespace arialrec
{

namespace preprocessing
{

void toGrayscale(image::Image &img)
{
	if (img.componentCount != image::ComponentCount::THREE)
	{
		return;
	}

	for (size_t i = 0; i < img.logicalSize(); ++i)
	{
		const size_t physicalPosition = i * 3;

		const image::brightness_t grayValue = R_WEIGHT * img.data[physicalPosition]
			+ G_WEIGHT * img.data[physicalPosition + 1]
			+ B_WEIGHT * img.data[physicalPosition + 2];

		memset(img.data + physicalPosition, grayValue, (int)img.componentCount);
	}

	image::tightenToSingleComponent(img);
}

void toBinary(image::Image &img, const image::brightness_t threshold)
{
	if ((threshold < MIN_RGB_VALUE) || (threshold > MAX_RGB_VALUE) || (img.componentCount != image::ComponentCount::SINGLE))
	{
		return;
	}

	for (size_t i = 0; i < img.physicalSize(); ++i)
	{
		img.data[i] = img.data[i] < threshold ? MIN_RGB_VALUE : MAX_RGB_VALUE;
	}
}

} // namespace preprocessing

} // namespace arielrec