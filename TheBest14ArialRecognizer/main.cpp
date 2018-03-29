#include "stdafx.h"

#include "image.h"
#include "recognize_subcommand.h"


int main(int argc, char **argv)
{
	CLI::App app{"The Best Size 14 Arial Text Recognizer"};

	arialrec::command::addRecognizeSubcommand(app);

	app.require_subcommand();

	CLI11_PARSE(app, argc, argv);

    return 0;
}
