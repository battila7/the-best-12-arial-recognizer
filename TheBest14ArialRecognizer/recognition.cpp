#include "stdafx.h"

#include "feature/area.h"
#include "feature/position.h"
#include "feature/walsh.h"
#include "segmentation.h"

#include "recognition.h"

namespace arialrec
{

namespace recognition
{

feature::FeatureMap readFeatureMap(const char *path)
{
	return {};
}

void writeFeatureMap(const char *path, const feature::FeatureMap &featureMap)
{

}

feature::FeatureVector characterToFeatureVector(const image::GrayscaleImage &img, const segmentation::CharacterBox charBox)
{
	feature::FeatureVector featureVector;



	return featureVector;
}

std::string recognizeText(const image::GrayscaleImage &img, const std::vector<segmentation::Line> &lines, const feature::FeatureMap &featureMap)
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
