#ifndef RECOGNITION_H
#define RECOGNITION_H

#include <string>
#include <vector>

#include "image.h"

namespace arialrec
{

namespace segmentation
{
struct Line;
} // namespace segmentation

namespace recognition
{

using feature_t = int;

using FeatureVector = std::vector<feature_t>;

using FeatureMap = std::map<char, FeatureVector>;


FeatureMap readFeatureMap(const char *path);

void writeFeatureMap(const char *path, const FeatureMap &featureMap);

std::string recognizeText(const image::GrayscaleImage &img, const std::vector<segmentation::Line> &lines, const FeatureMap &featureMap);

} // namespace recognition

} // namespace arialrec

#endif // RECOGNITION_H
