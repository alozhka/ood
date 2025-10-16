#include "src/input/FileInputStream.h"
#include "src/input/decorators/DecompressInputStream.h"
#include "src/input/decorators/DecryptionInputStream.h"
#include "src/output/FileOutputStream.h"
#include "src/output/decorators/CompressionOutputStream.h"
#include "src/output/decorators/EncryptionOutputStream.h"
#include "src/parse/CommandLineParser.h"
#include <iostream>
#include <memory>
#include <ranges>

std::unique_ptr<IInputDataStream> CreateInputStream(const std::string& fileName, const std::vector<Operation>& operations)
{
	std::unique_ptr<IInputDataStream> stream = std::make_unique<FileInputStream>(fileName);

	for (const auto& op : operations)
	{
		switch (op.type)
		{
		case OperationType::DECOMPRESS:
			stream = std::make_unique<DecompressInputStream>(std::move(stream));
			break;
		case OperationType::DECRYPT:
			stream = std::make_unique<DecryptionInputStream>(std::move(stream), op.key);
			break;
		default:
			throw std::invalid_argument("Invalid input operation type");
		}
	}

	return stream;
}

std::unique_ptr<IOutputDataStream> CreateOutputStream(const std::string& fileName, const std::vector<Operation>& operations)
{
	std::unique_ptr<IOutputDataStream> stream = std::make_unique<FileOutputStream>(fileName);

	for (const auto& op : std::ranges::reverse_view(operations))
	{
		switch (op.type)
		{
		case OperationType::ENCRYPT:
			stream = std::make_unique<EncryptionOutputStream>(std::move(stream), op.key);
			break;
		case OperationType::COMPRESS:
			stream = std::make_unique<CompressionOutputStream>(std::move(stream));
			break;
		default:
			throw std::invalid_argument("Invalid output operation type");
		}
	}

	return stream;
}

void TransformFile(IInputDataStream& input, IOutputDataStream& output)
{
	while (!input.IsEOF())
	{
		output.WriteByte(input.ReadByte());
	}
}

int main(int argc, char* argv[])
{
	try
	{
		CommandLineArgs options = CommandLineParser::Parse(argc, argv);

		std::unique_ptr<IInputDataStream> inputStream = CreateInputStream(options.inputFile, options.inputOperations);
		std::unique_ptr<IOutputDataStream> outputStream = CreateOutputStream(options.outputFile, options.outputOperations);

		TransformFile(*inputStream, *outputStream);

		inputStream->Close();
		outputStream->Close();

		return EXIT_SUCCESS;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}