#include "stdafx.h"

#include "segmentation.h"

#include "feature/walsh.h"

namespace arialrec
{

namespace feature
{

namespace walsh
{

using namespace arialrec;

static constexpr size_t MATRIX_DIMENSION = 8;
static constexpr size_t IMAGE_DIMENSION = 64;

using WalshImage = std::array<image::brightness_t, IMAGE_DIMENSION * IMAGE_DIMENSION>;

enum FillDirection
{
	ROW,
	COLUMN
};

static WalshImage createBorderWalsh(std::array<char, 8> values, FillDirection direction)
{
	WalshImage img;

	std::fill(img.begin(), img.end(), image::MAX_BRIGHTNESS_VALUE);

	for (size_t i = 0; i < IMAGE_DIMENSION; ++i)
	{
		// Divided by 8 because although Walsh images have size of 64x64 pixels, they're
		// actually 8x8 grids of 8x8 squares.
		if (!values[i / 8])
		{
			continue;
		}

		for (size_t j = 0; j < IMAGE_DIMENSION; ++j)
		{
			const size_t physicalIndex = (direction == COLUMN ? i * IMAGE_DIMENSION + j : j * IMAGE_DIMENSION + i);

			img[physicalIndex] = image::MIN_BRIGHTNESS_VALUE;
		}
	}

	return img;
}

static WalshImage xor(const WalshImage &lhs, const WalshImage &rhs)
{
	WalshImage xord;

	for (size_t i = 0; i < IMAGE_DIMENSION * IMAGE_DIMENSION; ++i)
	{
		xord[i] = (lhs[i] == rhs[i] ? image::MAX_BRIGHTNESS_VALUE : image::MIN_BRIGHTNESS_VALUE);
	}

	return xord;
}

static feature_t countMatches(const image::GrayscaleImage &target, const WalshImage &probe)
{
	feature_t sum = 0;

	for (size_t i = 0; i < target.physicalSize(); ++i)
	{
		if ((target.data[i] == image::MIN_BRIGHTNESS_VALUE) && (probe[i] == image::MIN_BRIGHTNESS_VALUE))
		{
			++sum;
		}
	}

	return sum;
}

static std::vector<WalshImage> computeWalshMatrix()
{
	// The images in first row and the first column are hardcoded.
	// Other matrix elements are calculated from the hardcoded values
	// by XOR-ing them together.
	std::vector<WalshImage> images = {
		createBorderWalsh({ 1, 1, 1, 1, 1, 1, 1, 1 }, ROW),
		createBorderWalsh({ 1, 0, 1, 0, 1, 0, 1, 0 }, ROW),
		createBorderWalsh({ 1, 1, 0, 0, 1, 1, 0, 0 }, ROW),
		createBorderWalsh({ 1, 0, 0, 1, 1, 0, 0, 1 }, ROW),
		createBorderWalsh({ 1, 1, 1, 1, 0, 0, 0, 0 }, ROW),
		createBorderWalsh({ 1, 0, 1, 0, 0, 1, 0, 1 }, ROW),
		createBorderWalsh({ 1, 1, 0, 0, 0, 0, 1, 1 }, ROW),
		createBorderWalsh({ 1, 0, 0, 1, 0, 1, 1, 0 }, ROW),

		createBorderWalsh({ 1, 0, 1, 0, 1, 0, 1, 0 }, COLUMN),
		createBorderWalsh({ 1, 1, 0, 0, 1, 1, 0, 0 }, COLUMN),
		createBorderWalsh({ 1, 0, 0, 1, 1, 0, 0, 1 }, COLUMN),
		createBorderWalsh({ 1, 1, 1, 1, 0, 0, 0, 0 }, COLUMN),
		createBorderWalsh({ 1, 0, 1, 0, 0, 1, 0, 1 }, COLUMN),
		createBorderWalsh({ 1, 1, 0, 0, 0, 0, 1, 1 }, COLUMN),
		createBorderWalsh({ 1, 0, 0, 1, 0, 1, 1, 0 }, COLUMN),
	};


	for (size_t row = 1; row < MATRIX_DIMENSION; ++row)
	{
		for (size_t column = 1; column < MATRIX_DIMENSION; ++column)
		{
			const size_t rowImageIndex = row - 1;
			// 7 is the base offset because the indices 0-7 are used for the first row.
			const size_t columnImageIndex = 7 + column;

			images.push_back(xor(images[row - 1], images[columnImageIndex]));
		}
	}

	return images;
}

FeatureVector computeWalshValues(const image::GrayscaleImage &img, const segmentation::CharacterBox &charBox)
{
	// The Walsh matrix is calculated only once, on the first function call.
	// It is an 8x8 matrix of 64x64 binary images.
	static std::vector<WalshImage> matrix = computeWalshMatrix();

	FeatureVector result;

	auto charImg = image::copyRect(img, charBox.topLeft, charBox.bottomRight);
	auto resizedCharImg = image::resize(charImg, IMAGE_DIMENSION, IMAGE_DIMENSION);

	// We AND the Walsh image and the character image and count the matches.
	// A pixel is considered a match if it is black on both the Walsh and the character image.
	std::transform(matrix.begin(), matrix.end(), std::back_inserter(result), [&resizedCharImg](const WalshImage &probe)
	{
		return countMatches(resizedCharImg, probe);
	});

	delete charImg.data;
	delete resizedCharImg.data;

	return result;
}

} // namespace walsh

} // namespace feature

} // namespace arialrec
