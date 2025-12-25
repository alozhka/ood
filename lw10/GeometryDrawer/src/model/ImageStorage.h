#pragma once
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QImageReader>
#include <QStandardPaths>
#include <QString>
#include <QUuid>

class ImageStorage
{
public:
	static QString SaveTemporary(const QString& sourcePath)
	{
		QImageReader reader(sourcePath);
		if (!reader.canRead())
		{
			throw std::runtime_error("Unsupported image format or file not found: " + sourcePath.toStdString());
		}

		QString tempStoragePath = GetTemporaryStoragePath();

		QFileInfo sourceInfo(sourcePath);
		QString extension = sourceInfo.suffix();
		QString uniqueFileName = QUuid::createUuid().toString(QUuid::WithoutBraces) + "." + extension;
		QString destinationPath = tempStoragePath + "/" + uniqueFileName;

		if (!QFile::copy(sourcePath, destinationPath))
		{
			throw std::runtime_error("Failed to copy image to temporary storage: " + sourcePath.toStdString());
		}

		return destinationPath;
	}

	static QString Export(const QString& tempPath, const QString& documentImagesDir)
	{
		QFileInfo tempFileInfo(tempPath);
		if (!tempFileInfo.exists())
		{
			throw std::runtime_error("Source image not found in temporary storage: " + tempPath.toStdString());
		}

		QDir dir;
		if (!dir.mkpath(documentImagesDir))
		{
			throw std::runtime_error("Failed to create document images directory: " + documentImagesDir.toStdString());
		}

		QString fileName = tempFileInfo.fileName();
		QString destinationPath = documentImagesDir + "/" + fileName;

		if (QFile::exists(destinationPath))
		{
			QFile::remove(destinationPath);
		}

		if (!QFile::copy(tempPath, destinationPath))
		{
			throw std::runtime_error("Failed to copy image to document storage: " + destinationPath.toStdString());
		}

		return fileName;
	}

	static QString LoadFromDocument(const QString& relativePath, const QString& documentDir)
	{
		QString sourcePath = documentDir + "/" + relativePath;
		QFileInfo sourceInfo(sourcePath);

		if (!sourceInfo.exists())
		{
			throw std::runtime_error("Image file not found: " + sourcePath.toStdString());
		}

		QString tempStoragePath = GetTemporaryStoragePath();
		QString fileName = sourceInfo.fileName();
		QString destinationPath = tempStoragePath + "/" + fileName;

		if (QFile::exists(destinationPath))
		{
			return destinationPath;
		}

		if (!QFile::copy(sourcePath, destinationPath))
		{
			throw std::runtime_error("Failed to copy image to temporary storage: " + destinationPath.toStdString());
		}

		return destinationPath;
	}

	static void Delete(const QString& imagePath)
	{
		if (imagePath.isEmpty())
		{
			return;
		}

		QFileInfo fileInfo(imagePath);
		if (fileInfo.exists() && imagePath.contains("GeometryDrawer_Images"))
		{
			QFile::remove(imagePath);
		}
	}

private:
	static QString GetTemporaryStoragePath()
	{
		QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
		QString appTempDir = tempDir + "/GeometryDrawer_Images";

		QDir dir;
		if (!dir.mkpath(appTempDir))
		{
			throw std::runtime_error("Failed to create temporary storage directory");
		}

		return appTempDir;
	}
};