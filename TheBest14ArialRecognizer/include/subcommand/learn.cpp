#include "stdafx.h"
#include "image.h"
#include "preprocessing.h"
#include "recognition.h"
#include "segmentation.h"
#include "feature/walsh.h"

#include "subcommand/learn.h"

namespace arialrec
{

namespace command
{

static constexpr int WINDOW_WIDTH = 200;
static constexpr int WINDOW_HEIGHT = 200;

struct Arguments
{
	std::string inputImagePath;
	image::brightness_t binaryThreshold = 127;
	std::string outputFile;
};

static void displayCharacterImage(sf::RenderWindow &window, const image::RGBImage &characterImage)
{
	auto rgbaCharacterImage = image::addAlphaChannel(characterImage);

	sf::Image sfImg;
	sfImg.create(rgbaCharacterImage.width, rgbaCharacterImage.height, rgbaCharacterImage.data);

	sf::Texture sfTexture;
	sfTexture.loadFromImage(sfImg);

	sf::Sprite sprite(sfTexture);
	sprite.setOrigin({ (float)rgbaCharacterImage.width / 2.f, (float)rgbaCharacterImage.height / 2.f });
	sprite.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);

	window.clear({ 255, 255, 255, 255 });
	window.draw(sprite);
	window.display();

	delete rgbaCharacterImage.data;
}

static feature::FeatureMap supervisedLearning(const image::RGBImage &rgbImage, const image::GrayscaleImage &grayscaleImage, const std::vector<segmentation::Line> &lines)
{
	std::cout << "Supervised learning has started!" << std::endl
		      << "Enter an empty line to exit." << std::endl;

	feature::FeatureMap featureMap;
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "The Best 14 Arial Recognizer");

	for (const auto &line : lines)
	{
		for (const auto &ch : line.characters)
		{
			auto characterImage = image::copyRect(rgbImage, ch.topLeft, ch.bottomRight);

			displayCharacterImage(window, characterImage);

			delete characterImage.data;

			std::string input;

			std::cout << ">> ";
			std::getline(std::cin, input);

			if (input.size() == 0)
			{
				goto endOfLearning;
			}
			else
			{
				featureMap[input[0]] = recognition::characterToFeatureVector(grayscaleImage, line, ch);
			}
		}
	}

	endOfLearning:
	window.close();
	return featureMap;
}

static void runSubcommand(const Arguments &args)
{
	image::RGBImage originalImage;
	
	image::read(args.inputImagePath.c_str(), originalImage);

	image::GrayscaleImage grayscaleImage = preprocessing::toGrayscale(originalImage);

	preprocessing::toBinary(grayscaleImage, args.binaryThreshold);

	const auto lines = segmentation::performSegmentation(grayscaleImage, 0, 2000);

	const feature::FeatureMap featureMap = supervisedLearning(originalImage, grayscaleImage, lines);

	recognition::writeFeatureMap(args.outputFile.c_str(), featureMap);
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
