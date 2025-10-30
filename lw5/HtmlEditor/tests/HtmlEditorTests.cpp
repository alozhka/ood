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
		"  InsertParagraph: Usage: InsertParagraph <position>|end <text>. Inserts a paragraph.\n"
		"  ReplaceText: Usage: ReplaceText <position> <text>. Replaces a paragraph with specified text.\n"
		"  DeleteItem: Usage: DeleteItem <position>. Deletes the item at specified position.\n"
		"  InsertImage: Usage: InsertImage <position>|end <width> <height> <path>. Inserts an image.\n"
		"  ResizeImage: Usage: ResizeImage <position> <width> <height>. Resizes an image.\n",
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
			   "InsertParagraph 2 Middle paragraph\n"
			   "InsertParagraph 1 Very first paragraph\n"
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

TEST_F(CommandControllerTests, ReplacesParagraph)
{
	SetupInput("InsertParagraph end First paragraph\n"
			   "InsertParagraph end Second paragraph\n"
			   "List\n");
	CommandController controller(input, output);

	controller.Run();

	EXPECT_EQ("Title: \n"
			  "1. Paragraph: First paragraph\n"
			  "2. Paragraph: Second paragraph\n",
		output.str());

	output.str("");
	SetupInput("ReplaceText 2 New paragraph\nList\n");

	controller.Run();

	EXPECT_EQ("Title: \n"
			  "1. Paragraph: First paragraph\n"
			  "2. Paragraph: New paragraph\n",
		output.str());
}

TEST_F(CommandControllerTests, DeletesItem)
{
	SetupInput("InsertParagraph end First paragraph\n"
			   "InsertParagraph end Second paragraph\n"
			   "InsertParagraph end Third paragraph\n"
			   "List\n");
	CommandController controller(input, output);

	controller.Run();

	EXPECT_EQ("Title: \n"
			  "1. Paragraph: First paragraph\n"
			  "2. Paragraph: Second paragraph\n"
			  "3. Paragraph: Third paragraph\n",
		output.str());

	output.str("");
	SetupInput("DeleteItem 2\nList\n");

	controller.Run();

	EXPECT_EQ("Title: \n"
			  "1. Paragraph: First paragraph\n"
			  "2. Paragraph: Third paragraph\n",
		output.str());
}

TEST_F(CommandControllerTests, CannotDeleteItemAtInvalidPosition)
{
	SetupInput("InsertParagraph end First paragraph\n"
			   "InsertParagraph end Third paragraph\n"
			   "DeleteItem 3\n");
	CommandController controller(input, output);

	controller.Run();

	EXPECT_EQ("Invalid item index\n", output.str());
}

// InsertImage tests
TEST_F(CommandControllerTests, InsertsImageAtEnd)
{
	SetupInput("InsertImage end 400 300 ../tests/images/test.svg\nList\n");
	CommandController controller(input, output);

	controller.Run();

	EXPECT_EQ("Title: \n"
			  "1. Image: 400 300 images/image_1.svg\n",
		output.str());
}

TEST_F(CommandControllerTests, InsertsImageAtPosition)
{
	SetupInput("InsertParagraph end First paragraph\n"
			   "InsertParagraph end Second paragraph\n"
			   "InsertImage 2 400 300 ../tests/images/test.svg\n"
			   "List\n");
	CommandController controller(input, output);

	controller.Run();

	EXPECT_EQ("Title: \n"
			  "1. Paragraph: First paragraph\n"
			  "2. Image: 400 300 images/image_1.svg\n"
			  "3. Paragraph: Second paragraph\n",
		output.str());
}

TEST_F(CommandControllerTests, CannotInsertImageAtInvalidPosition)
{
	SetupInput("InsertImage 5 400 300 ../tests/images/test.svg\n");
	CommandController controller(input, output);

	controller.Run();

	EXPECT_EQ("Invalid position for insertion\n", output.str());
}

TEST_F(CommandControllerTests, CannotInsertImageWithInvalidDimensions)
{
	SetupInput("InsertImage end 0 300 ../tests/images/test.svg\n");
	CommandController controller(input, output);

	controller.Run();

	EXPECT_EQ("Invalid image dimensions (must be 1-10000)\n", output.str());
}

TEST_F(CommandControllerTests, CannotInsertImageWithNonexistentFile)
{
	SetupInput("InsertImage end 400 300 ../tests/images/nonexistent.svg\n");
	CommandController controller(input, output);

	controller.Run();

	EXPECT_EQ("Image file not found\n", output.str());
}

TEST_F(CommandControllerTests, ResisesImage)
{
	SetupInput("InsertImage end 400 300 ../tests/images/test.svg\nResizeImage 1 200 250\nList\n");
	CommandController controller(input, output);

	controller.Run();

	EXPECT_EQ("Title: \n"
			  "1. Image: 200 250 images/image_1.svg\n",
		output.str());
}

TEST_F(CommandControllerTests, CannotResizeImageAtInvalidIndex)
{
	SetupInput("ResizeImage 1 200 250\n");
	CommandController controller(input, output);

	controller.Run();

	EXPECT_EQ("Invalid item index\n", output.str());
}

TEST_F(CommandControllerTests, SavesDocumentInHtml)
{
	SetupInput("SetTitle Document with <special> & \"characters\"\n"
			   "InsertParagraph end First paragraph\n"
			   "InsertImage end 400 300 ../tests/images/test.svg\n"
			   "InsertParagraph end Text with <tags> & \"quotes\" and 'apostrophes'\n"
			   "InsertImage end 640 480 ../tests/images/test.svg\n"
			   "InsertParagraph end Last paragraph\n"
			   "Save test_document.html\n");
	CommandController controller(input, output);

	controller.Run();

	// Проверяем, что файл создан
	ASSERT_TRUE(std::filesystem::exists("test_document.html"));

	// Читаем содержимое файла
	std::ifstream file("test_document.html");
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content = buffer.str();

	// Ожидаемый HTML
	std::string expectedHtml =
		"<!DOCTYPE html>\n"
		"<html>\n"
		"<head>\n"
		"<title>Document with &lt;special&gt; &amp; &quot;characters&quot;</title>\n"
		"</head>\n"
		"<body>\n"
		"<p>First paragraph</p>\n"
		"<img src=\"images/image_1.svg\" width=\"400\" height=\"300\" />\n"
		"<p>Text with &lt;tags&gt; &amp; &quot;quotes&quot; and &apos;apostrophes&apos;</p>\n"
		"<img src=\"images/image_2.svg\" width=\"640\" height=\"480\" />\n"
		"<p>Last paragraph</p>\n"
		"</body>\n"
		"</html>\n";

	EXPECT_EQ(expectedHtml, content);

	// Удаляем тестовый файл
	std::filesystem::remove("test_document.html");
	std::filesystem::remove_all("images");
}