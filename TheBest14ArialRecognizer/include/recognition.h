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
struct Line;
} // namespace segmentation

namespace recognition
{

feature::FeatureMap readFeatureMap(const char *path);

void writeFeatureMap(const char *path, const feature::FeatureMap &featureMap);

std::string recognizeText(const image::GrayscaleImage &img, const std::vector<segmentation::Line> &lines, const feature::FeatureMap &featureMap);

} // namespace recognition

} // namespace arialrec

#endif // RECOGNITION_H
