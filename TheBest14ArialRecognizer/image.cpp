#include "stdafx.h"

#include "stbi/stb_image.h"
#include "stbi/stb_image_write.h"

#include "image.h"

namespace arialrec
{

namespace image
{

bool read(const char *path, Image &img)
{
	int channels;

	img.data = stbi_load(path, &img.width, &img.height, &channels, (int)ComponentCount::THREE);

	img.componentCount = ComponentCount::THREE;

	return img.data;
}

bool write(const char *path, Image &img)
{
	if (img.componentCount != ComponentCount::THREE)
	{
		return false;
	}

	return stbi_write_bmp(path, img.width, img.height, (int)img.componentCount, img.data);
}

Image copy(const Image &other)
{
	Image newImage = other;

	newImage.data = new brightness_t[other.physicalSize()];

	memcpy(newImage.data, other.data, other.physicalSize());

	return newImage;
}

void expandToThreeComponents(Image &img)
{
	if (img.componentCount != ComponentCount::SINGLE)
	{
		return;
	}

	brightness_t *newData = new brightness_t[img.width * img.height * (int)ComponentCount::THREE];

	for (size_t i = 0; i < img.logicalSize(); ++i)
	{
		memset(newData + i * (int)ComponentCount::THREE,
			   img.data[i * (int)img.componentCount],
			   (int)ComponentCount::THREE);
	}

	delete img.data;

	img.data = newData;
	img.componentCount = ComponentCount::THREE;
}

void tightenToSingleComponent(Image &img)
{
	if (img.componentCount != ComponentCount::THREE)
	{
		return;
	}

	brightness_t *newData = new brightness_t[img.width * img.height];

	for (size_t i = 0; i < img.logicalSize(); ++i)
	{
		newData[i] = img.data[i * (int)img.componentCount];
	}

	delete img.data;

	img.data = newData;
	img.componentCount = ComponentCount::SINGLE;
}

} // namespace image

} // namespace arialrec