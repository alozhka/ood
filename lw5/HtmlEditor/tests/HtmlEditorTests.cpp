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

TEST_F(DocumentTests, InsertParagraphs)
{
	document.InsertParagraph("First paragraph", std::nullopt);
	document.InsertParagraph("Second paragraph", std::nullopt);
	document.InsertParagraph("Middle paragraph", 1);
	document.InsertParagraph("Very first paragraph", 0);

	EXPECT_EQ(4, document.GetItemsCount());
}

TEST_F(DocumentTests, InsertParagraphWithInvalidPosition)
{
	document.InsertParagraph("First paragraph", std::nullopt);

	// Попытка вставить в позицию 5, когда в документе только 1 элемент
	EXPECT_THROW(document.InsertParagraph("Invalid", 5), std::out_of_range);
}

TEST_F(CommandControllerTests, InsertParagraphViaController)
{
	SetupInput("InsertParagraph end First paragraph\nInsertParagraph end Second paragraph\nList\nExit\n");
	CommandController controller(input, output);

	controller.Run();

	EXPECT_TRUE(output.str().find("1. Paragraph: First paragraph") != std::string::npos);
	EXPECT_TRUE(output.str().find("2. Paragraph: Second paragraph") != std::string::npos);
}