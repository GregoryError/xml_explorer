#include "xmlwriter.h"
#include <QDebug>
#include <QMessageBox>

xml_writer::xml_writer(QObject *parent) : QObject(parent)
{


}

QString xml_writer::writeToFile(const QStringList &names, const QStringList &col_0, const QStringList &col_1)
{
    qDebug() << "FileList: " << names;

    int i = 0;
    int n_count = 1;
    for (const QString& nm : names)
    {
        f_out.setFileName(nm);
        if (!f_out.open(QIODevice::WriteOnly/* | QFile::Text*/))
        {
            QMessageBox msgBox;
            msgBox.setText("Произошла ошибка при открытии файла.\n"
                           + f_out.errorString());
            msgBox.exec();
        }

        xml.setDevice(&f_out);
        xml.setAutoFormatting(true);
        xml.writeStartDocument();

        xml.writeStartElement("root");     // <root>

        for (; i < col_0.size(); ++i)
        {
            if (!(n_count > names.size() - 1))
                if (col_0[i] == names[n_count] && i > 1)
                    break;
            if (col_0[i] != nm && col_0[i] != "root")
                xml.writeTextElement(col_0[i], col_1[i]);
        }

        xml.writeEndElement();             // </root>
        xml.writeEndDocument();
        f_out.close();
        ++n_count;

    }
    if (xml.hasError())
        return "Произошла ошибка при записи в файл.";
    else
        return "Данные успешно записаны.";

}







































