#ifndef RECOGNITION_H
#define RECOGNITION_H

#include <string>
#include <vector>

#include "feature/types.h"
#include "image.h"

namespace arialrec
{

namespace segmentation
{
struct CharacterBox;
struct Line;
} // namespace segmentation

namespace recognition
{

using distance_t = float;

struct DefaultConfiguration
{
	static constexpr distance_t recognitionThreshold = 1000.f;
};

struct Configuration
{
	distance_t recognitionThreshold = DefaultConfiguration::recognitionThreshold;
};

feature::FeatureMap readFeatureMap(const char *path);

void writeFeatureMap(const char *path, const feature::FeatureMap &featureMap);

feature::FeatureVector characterToFeatureVector(const image::GrayscaleImage &img, const segmentation::Line line, const segmentation::CharacterBox &charBox);

std::string recognizeText(const image::GrayscaleImage &img, const std::vector<segmentation::Line> &lines, const feature::FeatureMap &featureMap, const Configuration &conf);

} // namespace recognition

} // namespace arialrec

#endif // RECOGNITION_H
