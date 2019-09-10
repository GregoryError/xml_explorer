#ifndef XML_WRITER_H
#define XML_WRITER_H

#include <QObject>
#include <QFile>
#include <QXmlStreamWriter>
#include <QStringList>


class xml_writer : public QObject
{
    Q_OBJECT
private:
    QFile f_out;
    QXmlStreamWriter xml;
public:
    explicit xml_writer(QObject *parent = nullptr);
    QString writeToFile(const QStringList& names, const QStringList& col_0, const QStringList& col_1);
signals:

public slots:
};

#endif // XML_WRITER_H
