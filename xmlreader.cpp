#include "xmlreader.h"
#include <QDebug>
#include <iostream>
#include <QMessageBox>
#include <QSharedPointer>

xml_reader::xml_reader(const QString& f_name, QObject *parent) : QObject(parent)
{
    qRegisterMetaType<COMMUTATOR>("COMMUTATOR");
    f_nm = f_name;
}

MAP_lsts xml_reader::getFileStrings() const
{
    return fileLsts;
}

QString xml_reader::getErrors() const
{
    return errors;
}

int xml_reader::getLinesCount() const
{
    return linesCount;
}

void xml_reader::resetObj()
{
    f_in.close();
    xml.clear();
    fileLsts.first.clear();
    fileLsts.second.clear();
    errors.clear();
    linesCount = 0;
}

void xml_reader::doWork(const QString nm)
{
    f_nm = nm;
    f_in.setFileName(f_nm);
    if (!f_in.open(QIODevice::ReadOnly/* | QFile::Text*/))
    {
        qDebug() << "Can`t read file";
        qDebug() << f_in.errorString();

        QMessageBox msgBox;
        msgBox.setText("Произошла ошибка при открытии файла.\n"
                       + f_in.errorString());
        msgBox.exec();

    }

    xml.setDevice(&f_in);

    xml.readNext();

    fileLsts.first.append(f_nm);     // Setup header (Name + Attributes)
    fileLsts.second.append(xml.documentVersion().toString() + "; "
                           + xml.documentEncoding().toString());

    while (!xml.atEnd())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;

        if (token == QXmlStreamReader::StartElement)
        {
            QString t_str(xml.name().toString());
            xml.readNext();
            fileLsts.first.append(t_str);
            fileLsts.second.append(xml.text().toString());
            ++linesCount;
        }
    }

    if (xml.hasError())
    {
        errors = xml.errorString();
    }

    objToSend.fileLsts = fileLsts;
    objToSend.errors = errors;
    objToSend.linesCount = linesCount;
    objToSend.f_nm = f_nm;

    f_in.close();
                                      // some progressBar math here

    ++calls;                          // filesCount - numser of files in list


    qDebug() << "(calls * scale) = " << (calls * scale);

    emit updateBar(calls * scale);
    emit resultReady(objToSend);
}

void xml_reader::clear()
{
    f_in.close();
    xml.clear();
    fileLsts.first.clear();
    fileLsts.second.clear();
    errors.clear();
   // linesCount = 0;
    f_nm.clear();

    objToSend.f_nm.clear();
    objToSend.errors.clear();
    objToSend.fileLsts.first.clear();
    objToSend.fileLsts.second.clear();
    //objToSend.linesCount = 0;
}

void xml_reader::setFilesCountSlot(const int cnt)
{
    filesCount = cnt;
    scale = 100.0 / cnt;
    calls = 0;
}
























