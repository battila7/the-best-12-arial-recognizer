#include "stdafx.h"

#include "stbi/stb_image.h"
#include "stbi/stb_image_write.h"

#include "image.h"

namespace arialrec
{

namespace image
{

bool read(const char *path, RGBImage &img)
{
	int channels, width, height;

	img.data = stbi_load(path, &width, &height, &channels, img.componentCount);

	img.width = width;
	img.height = height;

	return img.data;
}

bool write(const char *path, const RGBImage &img)
{
	return stbi_write_bmp(path, img.width, img.height, img.componentCount, img.data);
}

template <ColorSpace C>
Image<C> copy(const Image<C> &other)
{
	Image<C> newImage = other;

	newImage.data = new brightness_t[other.physicalSize()];

	memcpy(newImage.data, other.data, other.physicalSize());

	return newImage;
}

template <ColorSpace C>
Image<C> copyRect(const Image<C> &source, const LogicalPosition &topLeft, const LogicalPosition &bottomRight)
{
	Image<C> newImage = {
		nullptr,
		bottomRight.column - topLeft.column,
		bottomRight.row - topLeft.row,
	};

	newImage.data = new brightness_t[newImage.physicalSize()];

	brightness_t *copyPointer = newImage.data;

	for (size_t row = topLeft.row; row < bottomRight.row; ++row)
	{
		for (size_t column = topLeft.column; column < bottomRight.column; ++column)
		{
			const int components = source.componentCount;

			const brightness_t *sourcePointer = source.data + (column * components + row * components);

			memcpy(copyPointer, sourcePointer, components);

			copyPointer += components;
		}
	}

	return newImage;
}

RGBImage expandToThreeComponents(GrayscaleImage &img)
{
	brightness_t *newData = new brightness_t[img.width * img.height * img.componentCount];

	for (size_t i = 0; i < img.logicalSize(); ++i)
	{
		memset(newData + i * img.componentCount,
			   img.data[i * img.componentCount],
			   img.componentCount);
	}

	return {
		newData,
		img.width,
		img.height
	};
}

 GrayscaleImage tightenToSingleComponent(RGBImage &img)
{
	brightness_t *newData = new brightness_t[img.width * img.height];

	for (size_t i = 0; i < img.logicalSize(); ++i)
	{
		newData[i] = img.data[i * (int)img.componentCount];
	}

	return {
		newData,
		img.width,
		img.height
	};
}

} // namespace image

} // namespace arialrec