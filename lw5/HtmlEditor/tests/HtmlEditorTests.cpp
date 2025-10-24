#include "../src/CommandController.h"
#include "../src/Document/Document.h"
#include "gtest/gtest.h"

class CommandControllerTests : public testing::Test
{
protected:
	void SetupInput(const std::string& inputData)
	{
		input.str(inputData);
		input.clear();
	}

	std::istringstream input;
	std::ostringstream output;
};

TEST_F(CommandControllerTests, PrintsHelp)
{
	SetupInput("Help\nExit\n");
	CommandController controller(input, output);

	controller.Run();

	EXPECT_EQ(
		"Commands list:\n"
		"  Help: Shows the available commands and required args\n"
		"  Exit: Exits the program.\n"
		"  Save: Usage: Save <path>. Saves the file.\n"
		"  SetTitle: Usage: SetTitle <title>. Sets the document title.\n"
		"  List: Shows the document title and items.\n"
		"  InsertParagraph: Usage: InsertParagraph <position>|end <text>. Inserts a paragraph.\n",
		output.str());
}

TEST_F(CommandControllerTests, SetsTitleAndShowsInList)
{
	SetupInput("SetTitle My Document Title\nList\nExit\n");
	CommandController controller(input, output);

	controller.Run();

	EXPECT_EQ("Title: My Document Title\n", output.str());
}

class DocumentTests : public testing::Test
{
protected:
	Document document;
};

TEST_F(CommandControllerTests, CannotInsertParagraphInInvalidPosition)
{
	SetupInput("InsertParagraph 5 First paragraph\n");
	CommandController controller(input, output);

	controller.Run();

	EXPECT_EQ("Invalid position for insertion\n", output.str());
}

TEST_F(CommandControllerTests, InsertsParagraphs)
{
	SetupInput("InsertParagraph end First paragraph\n"
			   "InsertParagraph end Second paragraph\n"
			   "InsertParagraph 1 Middle paragraph\n"
			   "InsertParagraph 0 Very first paragraph\n"
			   "List\n");
	CommandController controller(input, output);

	controller.Run();

	EXPECT_EQ("Title: \n"
			  "1. Paragraph: Very first paragraph\n"
			  "2. Paragraph: First paragraph\n"
			  "3. Paragraph: Middle paragraph\n"
			  "4. Paragraph: Second paragraph\n",
		output.str());
}