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

struct Arguments
{
	std::string inputImagePath;
	int noise = 0;
	image::brightness_t binaryThreshold = 127;
};

static void runSubcommand(const Arguments &args)
{
	image::RGBImage originalImage;

	image::read(args.inputImagePath.c_str(), originalImage);

	const feature::FeatureMap featureMap = recognition::readFeatureMap("features2.txt");

	image::GrayscaleImage grayscaleImage = preprocessing::toGrayscale(originalImage);

	preprocessing::toBinary(grayscaleImage, 127);

	/*if (args.noise > 0)
	{
		preprocessing::withAdditiveBinaryNoise(grayscaleImage, args.noise);
	}*/

	const auto lines = segmentation::performSegmentation(grayscaleImage, 0, 2000);

	const std::string text = recognition::recognizeText(grayscaleImage, lines, featureMap, 1000);

	std::ofstream o("out2.txt", std::ofstream::out);

	o << text;

	o.close();
}

void addRecognizeSubcommand(CLI::App &app)
{
	CLI::App *recognize = app.add_subcommand("recognize", "Recognize the text on the image using the prelearnt features.");

	std::shared_ptr<Arguments> args = std::make_shared<Arguments>();

	recognize->add_option("-i", args->inputImagePath, "The image file that contains the text to recognize.")
		->required();
	/*recognize->add_option("-f", args->outputFile, "The feature file location to load the features from.")
		->required();*/
	recognize->add_option("--binary-threshold", args->binaryThreshold, "The brightness value which is going to be used as a threshold when converting to binary", true);
	recognize->add_option("--noise", args->noise, "The percentage of binary noise to add to the input picture (0-100).", true);

	recognize->set_callback([args]() { runSubcommand(*args); });
}

} // namespace command

} // namespace arialrec
