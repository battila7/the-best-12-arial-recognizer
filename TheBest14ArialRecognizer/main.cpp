#include "stdafx.h"

#include "image.h"
#include "subcommand/learn.h"
#include "subcommand/recognize.h"

int main(int argc, char **argv)
{
	CLI::App app{"The Best Size 14 Arial Text Recognizer"};

	arialrec::command::addRecognizeSubcommand(app);

	arialrec::command::addLearnSubcommand(app);

	app.require_subcommand();

	CLI11_PARSE(app, argc, argv);

    return 0;
}
