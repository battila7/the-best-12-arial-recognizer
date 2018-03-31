#ifndef IMAGE_H
#define IMAGE_H

namespace arialrec
{

namespace image
{

using brightness_t = unsigned char;

enum class ComponentCount
{
	SINGLE = 1,
	THREE = 3
};

struct LogicalPosition
{
	size_t row, column;

	size_t physical() const
	{
		return row * column;
	}
};

struct Image
{
	brightness_t *data;
	int width, height;
	ComponentCount componentCount;

	size_t logicalSize() const
	{
		return width * height;
	}

	size_t physicalSize() const
	{
		return width * height * (int)componentCount;
	}
};

bool read(const char *path, Image &img);

bool write(const char *path, Image &img);

Image copy(const Image &other);

void expandToThreeComponents(Image &img);

void tightenToSingleComponent(Image &img);

} // namespace arialrec

} // namespace image

#endif // IMAGE_H