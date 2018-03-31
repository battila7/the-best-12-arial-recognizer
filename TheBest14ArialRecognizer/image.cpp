#include "stdafx.h"

#include "stbi/stb_image.h"
#include "stbi/stb_image_resize.h"
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
Image<C> resize(const Image<C> &source, const size_t width, const size_t height)
{
	Image<C> newImage = {
		nullptr,
		width,
		height
	};

	newImage.data = new brightness_t[newImage.physicalSize()];

	stbir_resize_uint8(source.data, source.width, source.height, 0, newImage.data, width, height, 0, newImage.componentCount);

	return newImage;
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
		bottomRight.column - topLeft.column + 1,
		bottomRight.row - topLeft.row + 1
	};

	newImage.data = new brightness_t[newImage.physicalSize()];

	brightness_t *copyPointer = newImage.data;

	for (size_t row = topLeft.row; row <= bottomRight.row; ++row)
	{
		for (size_t column = topLeft.column; column <= bottomRight.column; ++column)
		{
			const int components = source.componentCount;

			const brightness_t *sourcePointer = source.data + (row * source.width * components + column * components);

			memcpy(copyPointer, sourcePointer, components);

			copyPointer += components;
		}
	}

	return newImage;
}

RGBImage expandToThreeComponents(GrayscaleImage &img)
{
	RGBImage result = {
		new brightness_t[img.width * img.height * (int)ColorSpace::RGB],
		img.width,
		img.height
	};

	for (size_t i = 0; i < img.physicalSize(); ++i)
	{
		memset(result.data + i * result.componentCount, img.data[i], result.componentCount);
	}

	return result;
}

GrayscaleImage tightenToSingleComponent(RGBImage &img)
{
	GrayscaleImage result = {
		new brightness_t[img.width * img.height],
		img.width,
		img.height
	};

	for (size_t i = 0; i < img.logicalSize(); ++i)
	{
		result.data[i] = img.data[i * (int)img.componentCount];
	}

	return result;
}

template Image<ColorSpace::GRAYSCALE> resize(const Image<ColorSpace::GRAYSCALE> &source, const size_t width, const size_t height);
template Image<ColorSpace::RGB> resize(const Image<ColorSpace::RGB> &source, const size_t width, const size_t height);

template Image<ColorSpace::GRAYSCALE> copy(const Image<ColorSpace::GRAYSCALE> &other);
template Image<ColorSpace::RGB> copy(const Image<ColorSpace::RGB> &other);

template Image<ColorSpace::GRAYSCALE> copyRect(const Image<ColorSpace::GRAYSCALE> &source, const LogicalPosition &topLeft, const LogicalPosition &bottomRight);
template Image<ColorSpace::RGB> copyRect(const Image<ColorSpace::RGB> &source, const LogicalPosition &topLeft, const LogicalPosition &bottomRight);

} // namespace image

} // namespace arialrec