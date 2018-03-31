#include "stdafx.h"
#include "image.h"
#include "preprocessing.h"
#include "segmentation.h"
#include "feature/walsh.h"

#include "subcommand/learn.h"

namespace arialrec
{

namespace command
{

struct Arguments
{
	std::string inputImagePath;
	image::brightness_t binaryThreshold = 127;
	std::string outputFile;
};

static void runSubcommand(const Arguments &args)
{
	image::RGBImage rgbImg;

	image::read(args.inputImagePath.c_str(), rgbImg);

	image::GrayscaleImage grayscaleImg = preprocessing::toGrayscale(rgbImg);

	preprocessing::toBinary(grayscaleImg, args.binaryThreshold);

	auto lines = segmentation::performSegmentation(grayscaleImg, 0, 2000);

	int counter = 0;

	std::stringstream ss;

	/*for (const auto &line : lines)
	{
	for (const auto character : line.characters)
	{
	ss << "out" << counter++ << ".bmp";

	auto characterImg = image::copyRect(rgbImg, character.topLeft, character.bottomRight);

	auto resizedImg = image::resize(characterImg, 32, 32);

	image::write(ss.str().c_str(), resizedImg);

	std::cout << counter << std::endl;

	ss.str("");
	}
	}*/

	/*std::vector<feature::walsh::WalshImage> wm = walsh::computeWalshMatrix();

	for (walsh::WalshImage &wi : wm)
	{
	image::GrayscaleImage gi = {
	wi.data(),
	64,
	64
	};

	auto grgb = image::expandToThreeComponents(gi);

	ss << "out" << counter++ << ".bmp";

	image::write(ss.str().c_str(), grgb);

	std::cout << counter << std::endl;

	ss.str("");
	}*/
}

void addLearnSubcommand(CLI::App &app)
{
	CLI::App *recognize = app.add_subcommand("learn", "Supervised learning of features based on an example image.");

	std::shared_ptr<Arguments> args = std::make_shared<Arguments>();

	recognize->add_option("-f", args->inputImagePath, "The image file that contains the text to recognize")
		->required();
	recognize->add_option("--binary-threshold", args->binaryThreshold, "The brightness value which is going to be used as a threshold when converting to binary", true);
	recognize->add_option("-o", args->outputFile, "The file to save the feature data into.")
		->required();

	recognize->set_callback([args]() { runSubcommand(*args); });
}

} // namespace command

} // namespace arialrec