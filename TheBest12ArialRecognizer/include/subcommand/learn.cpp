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

struct DefaultArguments
{
	static constexpr image::brightness_t binaryThreshold = 127;
	static constexpr size_t minCharacterArea = segmentation::DefaultConfiguration::minArea;
	static constexpr size_t maxCharacterArea = segmentation::DefaultConfiguration::maxArea;
};

struct Arguments
{
	std::string inputImagePath;
	std::string featureFilePath;
	image::brightness_t binaryThreshold;
	size_t minCharacterArea;
	size_t maxCharacterArea;
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
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "The Best 12 Arial Recognizer");

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

	const auto lines = segmentation::performSegmentation(grayscaleImage, {
		args.minCharacterArea,
		args.maxCharacterArea
	});

	const feature::FeatureMap featureMap = supervisedLearning(originalImage, grayscaleImage, lines);

	recognition::writeFeatureMap(args.featureFilePath.c_str(), featureMap);
}

void learn(args::Subparser &parser)
{
	args::ValueFlag<std::string> inputImagePath(parser, "input", "The image file that contains the text to recognize.",
		{ 'i' }, args::Options::Required);
	args::ValueFlag<std::string> featureFilePath(parser, "features", "The feature file location to save the learnt features into.", 
		{ 'f' }, args::Options::Required);
	args::ValueFlag<image::brightness_t> binaryThreshold(parser, "binary threshold", "The brightness value which is going to be used as a threshold when converting to binary.", 
		{ "binary-threshold" }, DefaultArguments::binaryThreshold);
	args::ValueFlag<size_t> minCharacterArea(parser, "minimal character area", "The minimum area a character should take up.",
		{ "min-area" }, DefaultArguments::minCharacterArea);
	args::ValueFlag<size_t> maxCharacterArea(parser, "maximal character area", "The maximum area a character should take up.",
		{ "max-area" }, DefaultArguments::maxCharacterArea);

	parser.Parse();

	Arguments arguments{};

	arguments.inputImagePath = args::get(inputImagePath);
	arguments.featureFilePath = args::get(featureFilePath);
	arguments.binaryThreshold = args::get(binaryThreshold);
	arguments.minCharacterArea = args::get(minCharacterArea);
	arguments.maxCharacterArea = args::get(maxCharacterArea);

	runSubcommand(arguments);
}

} // namespace command

} // namespace arialrec
