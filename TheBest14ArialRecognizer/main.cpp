#include "stdafx.h"

#include "subcommand/learn.h"
#include "subcommand/recognize.h"

int main(int argc, char **argv)
{
	args::ArgumentParser parser("The Best Size 14 Arial Text Recognizer");

	args::Group commands(parser, "commands");
	args::Command learn(commands, "learn", "Supervised learning of features based on an example image.", &arialrec::command::learn);
	args::Command recognize(commands, "recognize", "Recognize the text on the image using the prelearnt features.", &arialrec::command::recognize);

	args::Group arguments(parser, "arguments");
	args::HelpFlag h(arguments, "help", "help", { 'h', "help" });

	args::GlobalOptions globals(parser, arguments);

	try
	{
		parser.ParseCLI(argc, argv);
	}
	catch (args::Help)
	{
		std::cout << parser;
	}
	catch (const args::Error &e)
	{
		std::cerr << e.what() << std::endl << parser;

		return 1;
	}

	return 0;
}
