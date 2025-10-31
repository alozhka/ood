#pragma once
#include <filesystem>

class ImageService
{
public:
	static std::string BuildImagePath(const std::string& sourcePath, size_t imageCounter)
	{
		const std::string extension = std::filesystem::path(sourcePath).extension().string();
		const std::string newImageName = "image_" + std::to_string(imageCounter) + extension;
		return "images/" + newImageName;
	}
	static void RemoveImage(const std::string& path)
	{
		if (std::filesystem::exists(path))
		{
			std::filesystem::remove(path);
		}
	}
};