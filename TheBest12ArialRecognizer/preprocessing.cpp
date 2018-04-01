#include "stdafx.h"

#include "preprocessing.h"

static constexpr float R_WEIGHT = 0.3f;
static constexpr float G_WEIGHT = 0.59f;
static constexpr float B_WEIGHT = 0.11f;

namespace arialrec
{

namespace preprocessing
{

image::GrayscaleImage toGrayscale(image::RGBImage &img)
{
	using image::brightness_t;

	for (size_t i = 0; i < img.logicalSize(); ++i)
	{
		const size_t physicalPosition = i * 3;

		const brightness_t grayValue = (brightness_t)(R_WEIGHT * img.data[physicalPosition])
			+ (brightness_t)(G_WEIGHT * img.data[physicalPosition + 1])
			+ (brightness_t)(B_WEIGHT * img.data[physicalPosition + 2]);

		memset(img.data + physicalPosition, grayValue, img.componentCount);
	}

	return image::tightenToSingleComponent(img);
}

void toBinary(image::GrayscaleImage &img, const image::brightness_t threshold)
{
	using image::MIN_BRIGHTNESS_VALUE;
	using image::MAX_BRIGHTNESS_VALUE;

	if ((threshold < MIN_BRIGHTNESS_VALUE) || (threshold > MAX_BRIGHTNESS_VALUE))
	{
		return;
	}

	for (size_t i = 0; i < img.physicalSize(); ++i)
	{
		img.data[i] = img.data[i] < threshold ? MIN_BRIGHTNESS_VALUE : MAX_BRIGHTNESS_VALUE;
	}
}

void withAdditiveBinaryNoise(image::GrayscaleImage &img, const int percentage)
{
	using image::MIN_BRIGHTNESS_VALUE;
	using image::MAX_BRIGHTNESS_VALUE;

	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());
	std::uniform_int_distribution<> distribution(0, 99);

	for (size_t i = 0; i < img.physicalSize(); ++i)
	{
		if (distribution(generator) < percentage)
		{
			img.data[i] = img.data[i] == MIN_BRIGHTNESS_VALUE ? MAX_BRIGHTNESS_VALUE : MIN_BRIGHTNESS_VALUE;
		}
	}
}

} // namespace preprocessing

} // namespace arielrec