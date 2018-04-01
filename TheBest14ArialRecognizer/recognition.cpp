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
	feature::FeatureMap featureMap;
	std::ifstream featureFile(path, std::ifstream::in);

	std::string input;

	while (std::getline(featureFile, input))
	{
		std::istringstream iss(input);

		char ch;
		feature::FeatureVector fv;

		while (iss)
		{
			feature::feature_t f;
			iss >> f;

			fv.push_back(f);
		}

		featureMap[ch] = fv;
	}
	
	return featureMap;
}

void writeFeatureMap(const char *path, const feature::FeatureMap &featureMap)
{
	std::ofstream featureFile(path, std::ofstream::out);

	for (auto it = featureMap.begin(); it != featureMap.end(); ++it)
	{
		featureFile << it->first;

		for (const feature::feature_t f : it->second)
		{
			featureFile << " " << f;
		}

		featureFile << std::endl;
	}

	featureFile.close();
}

feature::FeatureVector characterToFeatureVector(const image::GrayscaleImage &img, const segmentation::Line line, const segmentation::CharacterBox &charBox)
{
	feature::FeatureVector featureVector = feature::walsh::computeWalshValues(img, charBox);

	featureVector.push_back(feature::area::compute(charBox));
	featureVector.push_back(feature::position::verticalPositionInLine(line, charBox));

	return featureVector;
}

static int distanceBetweenChars(const segmentation::CharacterBox &lhs, const segmentation::CharacterBox &rhs)
{
	return lhs.topLeft.column - rhs.bottomRight.column;
}

static int averageDistanceBetweenCharacters(const std::vector<segmentation::Line> &lines)
{
	float sum = 0.f, count = 0.f;

	for (const auto &line : lines)
	{
		for (size_t i = 0; i < line.characters.size() - 1; ++i)
		{
			sum += distanceBetweenChars(line.characters[i + 1], line.characters[i]);
			++count;
		}
	}

	return (int)(sum / count);
}

static distance_t distanceBetweenFeatureVectors(const feature::FeatureVector &lhs, const feature::FeatureVector &rhs)
{
	feature::feature_t sum = 0;

	for (size_t i = 0; i < lhs.size(); ++i)
	{
		sum += (lhs[i] - rhs[i]) * (lhs[i] - rhs[i]);
	}

	return std::sqrtf((float)sum);
}

static std::pair<char, distance_t> tryRecognize(const image::GrayscaleImage &img, const segmentation::Line &line, const segmentation::CharacterBox charBox, const feature::FeatureMap &featureMap)
{
	const feature::FeatureVector charFeatures = characterToFeatureVector(img, line, charBox);
	char result = '?';
	distance_t minDistance = std::numeric_limits<distance_t>::max();

	for (auto it = featureMap.begin(); it != featureMap.end(); ++it)
	{
		if (const distance_t d = distanceBetweenFeatureVectors(charFeatures, it->second); d < minDistance)
		{
			minDistance = d;
			result = it->first;
		}
	}

	return { result, minDistance };
}

std::string recognizeText(const image::GrayscaleImage &img, const std::vector<segmentation::Line> &lines, const feature::FeatureMap &featureMap, const distance_t recognitionThreshold)
{
	std::stringstream text;

	int averageDistance = averageDistanceBetweenCharacters(lines);

	for (const auto &line : lines)
	{
		for (size_t i = 0; i < line.characters.size(); ++i)
		{
			if (i < line.characters.size() - 1)
			{
				if (const int d = distanceBetweenChars(line.characters[i], line.characters[i + 1]); d > averageDistance)
				{
					text << ' ';
				}
			}

			const auto [recognizedCharacter, similarity] = tryRecognize(img, line, line.characters[i], featureMap);

			text << (similarity > recognitionThreshold ? recognizedCharacter : '?');
		}

		text << std::endl;
	}

	return text.str();
}

} // namespace recognition

} // namespace arialrec
