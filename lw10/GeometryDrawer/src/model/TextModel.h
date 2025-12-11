#pragma once
#include <qobject.h>

class TextModel : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString GetContent READ GetContent WRITE SetContent NOTIFY ContentChanged)

public:
	explicit TextModel(QObject* parent = nullptr)
		: QObject(parent)
	{
	}

	QString GetContent() const
	{
		return m_content;
	}

	void SetContent(const QString& content)
	{
		m_content = content;
		emit ContentChanged();
	}

signals:
	void ContentChanged();

private:
	QString m_content{ "" };
};
