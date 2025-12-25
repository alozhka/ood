#pragma once
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QImageReader>
#include <QStandardPaths>
#include <QString>
#include <QUuid>

class ImageStorageManager
{
public:
	static QString ImportImage(const QString& sourcePath)
	{
		// Проверка формата изображения
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

	// Методы для будущих итераций
	// TODO: Итерация 3 - ExportToDocumentStorage(), LoadFromDocumentStorage()
	// TODO: Итерация 4 - IncrementRefCount(), DecrementRefCount(), CleanupUnusedImages()
};