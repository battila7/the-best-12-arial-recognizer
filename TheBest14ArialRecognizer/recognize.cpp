#include "stdafx.h"
#include "image.h"
#include "preprocessing.h"
#include "recognition.h"
#include "segmentation.h"

#include "subcommand/recognize.h"

namespace arialrec
{

namespace command
{

struct DefaultArguments
{
	static constexpr image::brightness_t binaryThreshold = 127;
	static constexpr int noise = 0;
};

struct Arguments
{
	std::string inputImagePath;
	std::string featureFilePath;
	std::string outputFilePath;
	int binaryThreshold;
	int noise;
};

static void runSubcommand(const Arguments &args)
{
	image::RGBImage originalImage;

	image::read(args.inputImagePath.c_str(), originalImage);

	const feature::FeatureMap featureMap = recognition::readFeatureMap("features2.txt");

	image::GrayscaleImage grayscaleImage = preprocessing::toGrayscale(originalImage);

	preprocessing::toBinary(grayscaleImage, 127);

	if (args.noise > 0)
	{
		preprocessing::withAdditiveBinaryNoise(grayscaleImage, args.noise);
	}

	const auto lines = segmentation::performSegmentation(grayscaleImage, 0, 2000);

	const std::string text = recognition::recognizeText(grayscaleImage, lines, featureMap, 1000);

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

	parser.Parse();

	runSubcommand({
		inputImagePath.Get(),
		featureFilePath.Get(),
		outputFilePath.Get(),
		binaryThreshold.Get(),
		noise.Get()
	});
}

} // namespace command

} // namespace arialrec
