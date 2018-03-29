#include "stdafx.h"
#include "image.h"
#include "preprocessing.h"

#include "recognize_subcommand.h"

namespace arialrec
{

namespace command
{

static struct Arguments
{
	std::string inputImagePath;
	image::brightness_t binaryThreshold = 127;
};

static void runSubcommand(const Arguments &args)
{
	image::Image img;

	image::read(args.inputImagePath.c_str(), img);

	preprocessing::toGrayscale(img);

	preprocessing::toBinary(img, args.binaryThreshold);
}

void addRecognizeSubcommand(CLI::App &app)
{
	CLI::App *recognize = app.add_subcommand("recognize", "Recognize the text on the image using the prelearnt features.");

	std::shared_ptr<Arguments> args = std::make_shared<Arguments>();

	recognize->add_option("-f", args->inputImagePath, "The image file that contains the text to recognize")
		->required();
	recognize->add_option("--binary-threshold", args->binaryThreshold, "The brightness value which is going to be used as a threshold when converting to binary", true);

	recognize->set_callback([args]() { runSubcommand(*args); });
}

} // namespace command

} // namespace arialrec
