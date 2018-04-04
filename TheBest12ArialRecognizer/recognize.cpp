#include "stdafx.h"
#include "image.h"
#include "preprocessing.h"
#include "recognition.h"
#include "segmentation.h"

#include "command/recognize.h"

namespace arialrec
{

namespace command
{

static constexpr auto CHAR_OUTPUT_PREFIX = "ch-";
static constexpr auto BMP_FORMAT = ".bmp";

struct DefaultArguments
{
	static constexpr image::brightness_t binaryThreshold = 127;
	static constexpr int noise = 0;
	static constexpr recognition::distance_t recognitionThreshold = recognition::DefaultConfiguration::recognitionThreshold;
	static constexpr size_t minCharacterArea = segmentation::DefaultConfiguration::minArea;
	static constexpr size_t maxCharacterArea = segmentation::DefaultConfiguration::maxArea;
};

struct Arguments
{
	std::string inputImagePath;
	std::string featureFilePath;
	std::string outputFilePath;
	image::brightness_t binaryThreshold;
	int noise;
	bool outputCharacters;
	recognition::distance_t recognitionThreshold;
	size_t minCharacterArea;
	size_t maxCharacterArea;
};

static void writeCharacters(const image::RGBImage &image, const std::vector<segmentation::Line> &lines)
{
	std::stringstream filename;
	size_t counter = 0;

	for (const auto &line : lines)
	{
		for (const auto &ch : line.characters)
		{
			const auto chRect = image::copyRect(image, ch.topLeft, ch.bottomRight);

			filename << CHAR_OUTPUT_PREFIX << counter++ << BMP_FORMAT;

			image::write(filename.str().c_str(), chRect);

			delete chRect.data;

			filename.str("");
		}
	}
}

static void runSubcommand(const Arguments &args)
{
	image::RGBImage originalImage;

	image::read(args.inputImagePath.c_str(), originalImage);

	const feature::FeatureMap featureMap = recognition::readFeatureMap(args.featureFilePath.c_str());

	image::GrayscaleImage grayscaleImage = preprocessing::toGrayscale(originalImage);

	preprocessing::toBinary(grayscaleImage, args.binaryThreshold);

	if (args.noise > 0)
	{
		preprocessing::withAdditiveBinaryNoise(grayscaleImage, args.noise);
	}

	const auto lines = segmentation::performSegmentation(grayscaleImage, {
		args.minCharacterArea,
		args.maxCharacterArea
	});

	if (args.outputCharacters)
	{
		writeCharacters(originalImage, lines);
	}

	const std::string text = recognition::recognizeText(grayscaleImage, lines, featureMap, {
		args.recognitionThreshold
	});

	std::ofstream output(args.outputFilePath.c_str(), std::ofstream::out);

	output << text;

	output.close();
}

void recognize(args::Subparser &parser)
{
	args::ValueFlag<std::string> inputImagePath(parser, "input", "The image file that contains the text to recognize.",
		{ 'i' }, args::Options::Required);
	args::ValueFlag<std::string> featureFilePath(parser, "features", "The feature file location to load the features from.",
		{ 'f' }, args::Options::Required);
	args::ValueFlag<std::string> outputFilePath(parser, "output", "The file to write the recognized text into.",
		{ 'o' }, args::Options::Required);
	args::ValueFlag<image::brightness_t> binaryThreshold(parser, "binary threshold", "The brightness value which is going to be used as a threshold when converting to binary.",
		{ "binary-threshold" }, DefaultArguments::binaryThreshold);
	args::ValueFlag<int> noise(parser, "noise", "The percentage of binary noise to add to the input picture (0-100).",
		{ "noise" }, DefaultArguments::noise);
	args::ValueFlag<recognition::distance_t> recognitionThreshold(parser, "recognition threshold", "The maximal feature vector distance that is considered a match.",
		{ "recognition-threshold" }, DefaultArguments::recognitionThreshold);
	args::ValueFlag<size_t> minCharacterArea(parser, "minimal character area", "The minimum area a character should take up.",
		{ "min-area" }, DefaultArguments::minCharacterArea);
	args::ValueFlag<size_t> maxCharacterArea(parser, "maximal character area", "The maximum area a character should take up.",
		{ "max-area" }, DefaultArguments::maxCharacterArea);
	args::Flag outputCharacters(parser, "output characters", "Output the characters produced by the segmentation phase (writes to the current directory).",
		{ "output-characters" });

	parser.Parse();

	Arguments arguments{};

	arguments.inputImagePath = args::get(inputImagePath);
	arguments.featureFilePath = args::get(featureFilePath);
	arguments.outputFilePath = args::get(outputFilePath);
	arguments.binaryThreshold = args::get(binaryThreshold);
	arguments.noise = args::get(noise);
	arguments.outputCharacters = args::get(outputCharacters);
	arguments.recognitionThreshold = args::get(recognitionThreshold);
	arguments.minCharacterArea = args::get(minCharacterArea);
	arguments.maxCharacterArea = args::get(maxCharacterArea);

	runSubcommand(arguments);
}

} // namespace command

} // namespace arialrec
