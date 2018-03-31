#include "stdafx.h"

#include "segmentation.h"

#include "recognition.h"

namespace arialrec
{

namespace recognition
{

FeatureMap readFeatureMap(const char *path)
{
	return {};
}

void writeFeatureMap(const char *path, const FeatureMap &featureMap)
{

}

FeatureVector characterToFeatureVector(const image::GrayscaleImage &img, const segmentation::CharacterBox charBox)
{
	FeatureVector featureVector;

	return featureVector;
}

std::string recognizeText(const image::GrayscaleImage &img, const std::vector<segmentation::Line> &lines, const FeatureMap &featureMap)
{
	std::stringstream text;

	for (const auto &line : lines)
	{
		text << std::endl;
	}

	return text.str();
}

} // namespace recognition

} // namespace arialrec
