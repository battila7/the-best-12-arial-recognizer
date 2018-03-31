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
	int channels, width, height;

	img.data = stbi_load(path, &width, &height, &channels, (int)ComponentCount::THREE);

	img.width = width;
	img.height = height;
	img.componentCount = ComponentCount::THREE;

	return img.data;
}

bool write(const char *path, const Image &img)
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

Image copyRect(const Image &source, const LogicalPosition &topLeft, const LogicalPosition &bottomRight)
{
	Image newImage = {
		nullptr,
		bottomRight.column - topLeft.column,
		bottomRight.row - topLeft.row,
		source.componentCount
	};

	newImage.data = new brightness_t[newImage.physicalSize()];

	brightness_t *copyPointer = newImage.data;

	for (size_t row = topLeft.row; row < bottomRight.row; ++row)
	{
		for (size_t column = topLeft.column; column < bottomRight.column; ++column)
		{
			const int components = (int)source.componentCount;

			const brightness_t *sourcePointer = source.data + (column * components + row * components);

			memcpy(copyPointer, sourcePointer, components);

			copyPointer += components;
		}
	}

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