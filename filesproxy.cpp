#include "filesproxy.h"
#include <QDebug>

FilesProxy::FilesProxy(const int q, const QStringList &nm_lst,
                       const QMap<QString, MAP_lsts>& f_maps,
                       const MAP_s_s_& e_map)
    : quantity(q), names(nm_lst), files_maps(f_maps), errors_map(e_map)
{

}

void FilesProxy::addLinesCount(int lines)
{
    quantity += lines;
}

void FilesProxy::addName(const QString &f_name)
{
    names.append(f_name);
}

void FilesProxy::addValue(const QString &f_name, const MAP_lsts& f_lsts)
{
    files_maps.insert(f_name, f_lsts);
}

void FilesProxy::addError(const QString &f_name, const QString &err_str)
{
    errors_map.insert(f_name, err_str);
}

bool FilesProxy::hasErrors() const
{
    return has_err;
}

void FilesProxy::setErr()
{
    has_err = true;
}

QStringList FilesProxy::getNamesLst() const
{
    return names;
}

MAP_lsts FilesProxy::getFilePairs(const QString &name) const
{
    return files_maps.value(name);
}

MAP_s_s_ FilesProxy::getErrorMap() const
{
    return errors_map;
}

int FilesProxy::getSize() const
{
    int comm_sz = 0;
    for (auto& keyStr : names)
    {
        comm_sz += files_maps[keyStr].first.size();
    }
    return  comm_sz;// + names.size();
}

void FilesProxy::clear()
{
    quantity = 0;
    size = 0;
    names.clear();
    files_maps.clear();
    errors_map.clear();
    has_err = false;
}































