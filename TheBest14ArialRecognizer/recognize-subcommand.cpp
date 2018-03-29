#include "stdafx.h"

#include "recognize_subcommand.h"

namespace arialrec
{

namespace command
{

static struct Arguments
{
	std::string inputImagePath;
};

static void runSubcommand(const Arguments &args)
{
	std::cout << args.inputImagePath << std::endl;
}

void addRecognizeSubcommand(CLI::App &app)
{
	CLI::App *recognize = app.add_subcommand("recognize", "Recognize the text on the image using the prelearnt features.");

	std::shared_ptr<Arguments> args = std::make_shared<Arguments>();

	recognize->add_option("-f", args->inputImagePath, "The image file that contains the text to recognize")
		->required();

	recognize->set_callback([args]() { runSubcommand(*args); });
}

} // namespace command

} // namespace arialrec
