#ifndef IMAGE_H
#define IMAGE_H

namespace arialrec
{

namespace image
{

using brightness_t = unsigned char;

constexpr brightness_t MIN_BRIGHTNESS_VALUE = 0;
constexpr brightness_t MAX_BRIGHTNESS_VALUE = 255;

enum class ColorSpace
{
	GRAYSCALE = 1,
	RGB = 3
};

template <ColorSpace C>
struct Image
{
	brightness_t *data = nullptr;
	size_t width, height;
	const int componentCount = (int)C;

	size_t logicalSize() const
	{
		return width * height;
	}

	size_t physicalSize() const
	{
		return width * height * (int)componentCount;
	}
};

using GrayscaleImage = Image<ColorSpace::GRAYSCALE>;
using RGBImage = Image<ColorSpace::RGB>;

struct LogicalPosition
{
	size_t row, column;

	size_t physical() const
	{
		return row * column;
	}
};

bool read(const char *path, RGBImage &img);

bool write(const char *path, const RGBImage &img);

template <ColorSpace C>
Image<C> resize(const Image<C> &source, const size_t width, const size_t height);

template <ColorSpace C>
Image<C> copy(const Image<C> &other);

template <ColorSpace C>
Image<C> copyRect(const Image<C> &source, const LogicalPosition &topLeft, const LogicalPosition &bottomRight);

RGBImage expandToThreeComponents(GrayscaleImage &img);

GrayscaleImage tightenToSingleComponent(RGBImage &img);

} // namespace arialrec

} // namespace image

#endif // IMAGE_H