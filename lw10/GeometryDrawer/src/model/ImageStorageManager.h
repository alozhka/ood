#pragma once
#include <QImageReader>
#include <QString>

class ImageStorageManager
{
public:
	// Итерация 1: базовая версия - просто возвращаем оригинальный путь
	// В итерации 2 добавим копирование во временное хранилище
	static QString ImportImage(const QString& sourcePath)
	{
		// Проверка формата изображения
		QImageReader reader(sourcePath);
		if (!reader.canRead())
		{
			throw std::runtime_error("Unsupported image format or file not found: " + sourcePath.toStdString());
		}

		// Пока просто возвращаем оригинальный путь
		// TODO: В итерации 2 копировать во временное хранилище
		return sourcePath;
	}

	// Методы для будущих итераций
	// TODO: Итерация 2 - GetTemporaryStoragePath()
	// TODO: Итерация 3 - ExportToDocumentStorage(), LoadFromDocumentStorage()
	// TODO: Итерация 4 - IncrementRefCount(), DecrementRefCount(), CleanupUnusedImages()
};