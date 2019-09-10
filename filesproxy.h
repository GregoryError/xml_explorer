#ifndef FILESPROXY_H
#define FILESPROXY_H

// structure for store/transfer sets of data to model

#include <QMap>
#include <QString>
#include <QStringList>


typedef  QMap<QString, QString> MAP_s_s_;

typedef  QPair<QStringList, QStringList> MAP_lsts;

class FilesProxy
{
private:
    int quantity = 0;                    // data strings counter
    int size;
    QStringList names;                   // file names
    QMap<QString, MAP_lsts> files_maps;  // stores maps of values by file name as a key
    MAP_s_s_ errors_map;                 // stores errors in files by file name as a key
    bool has_err = false;

public:
    FilesProxy() = default;
    FilesProxy(const int q, const QStringList &nm_lst,
               const QMap<QString, MAP_lsts>& f_maps,
               const MAP_s_s_& e_map);

    void addLinesCount(int lines);
    void addName(const QString& f_name);
    void addValue(const QString& f_name, const MAP_lsts& f_lsts);
    void addError(const QString& f_name, const QString& err_str);
    bool hasErrors() const;
    void setErr();

    int obj_count() { return quantity; }
    QStringList getNamesLst() const;
    MAP_lsts getFilePairs(const QString& name) const;
    MAP_s_s_ getErrorMap() const;
    int getSize() const;
    void clear();

};


class COMMUTATOR
{

public:
    MAP_lsts fileLsts;
    QString errors;
    int linesCount = 0;
    QString f_nm;
    COMMUTATOR() = default;
    MAP_lsts getFileStrings() const { return fileLsts; }
    QString getErrors() const { return errors; }
    int getLinesCount() const { return linesCount; }
    QString getFNm() { return f_nm; }

};

#endif // FILESPROXY_H
