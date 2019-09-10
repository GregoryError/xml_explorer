#include "table_model.h"
#include <QDebug>


QVariant TABLE_MODEL::data(const QModelIndex &index, int nRole) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    QString str =
            QString("%1, %2").arg(index.row() + 1).arg(index.column() + 1);
    return (nRole == Qt::DisplayRole || nRole == Qt::EditRole)
            ? m_hash.value(index, QVariant())
            : QVariant();
}

bool TABLE_MODEL::setData(const QModelIndex &index, const QVariant &value, int nRole)
{
    if (index.isValid() && nRole == Qt::EditRole)
    {
        m_hash[index] = value;
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags TABLE_MODEL::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    return index.isValid() ? (flags | Qt::ItemIsEditable)
                           : flags;
}

QVariant TABLE_MODEL::headerData(int nSection, Qt::Orientation orientation, int nRole) const
{
    if (nRole != Qt::DisplayRole)
    {
        return QVariant();
    }

    return (orientation == Qt::Horizontal) ?
                objName[nSection]
                : rowsNames[nSection];
}

void TABLE_MODEL::setValue(const int nRows, const int nColumns, const QVariant &val)
{
    // TODO add range check
    this->setData(this->index(nRows, nColumns, QModelIndex()), val, Qt::EditRole);
}

QVariant TABLE_MODEL::getCellContent(const int nRows, const int nColumns) const
{
    return this->data(index(nRows, nColumns, QModelIndex()), Qt::EditRole);
}


bool TABLE_MODEL::insertRows(int position, int rows, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginInsertRows(parent, position, position + rows - 1);
    ++m_nRows;
    int rowNum = rowsNames.last().toInt();
    rowsNames.append(QString::number(++rowNum));
    endInsertRows();
    return true;
}



bool TABLE_MODEL::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    --m_nRows;
    endRemoveRows();
    return true;
}


void TABLE_MODEL::clear()
{
    this->beginResetModel();
    setSize(0, 0);
    m_hash.clear();
    objName.clear();
    rowsNames.clear();
    this-> endResetModel();

}























































