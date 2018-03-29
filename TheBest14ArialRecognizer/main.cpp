#include "stdafx.h"

#include "image.h"
#include "recognize_subcommand.h"


int main(int argc, char **argv)
{
	CLI::App app{"The Best Size 14 Arial Text Recognizer"};

	app.require_subcommand();

	arialrec::command::addRecognizeSubcommand(app);

	CLI11_PARSE(app, argc, argv);

    return 0;
}
