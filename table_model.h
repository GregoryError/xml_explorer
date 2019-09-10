#ifndef TABLE_MODEL_H
#define TABLE_MODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QMenu>


class TABLE_MODEL : public QAbstractTableModel
{
    Q_OBJECT
private:
    int m_nRows;
    int m_nColumns;
    QHash<QModelIndex, QVariant> m_hash;
    QStringList objName;
    QStringList rowsNames;
public:
    //TABLE_MODEL() = default;
    TABLE_MODEL(int nRows = 1, int nColumns = 1, QObject* pobj = nullptr)
        : QAbstractTableModel(pobj), m_nRows(nRows), m_nColumns(nColumns) {}


    void setSize(int rws, int clm) { m_nRows = rws; m_nColumns = clm; }

    QVariant data(const QModelIndex& index, int nRole) const;

    bool setData(const QModelIndex& index,
                 const QVariant& value,
                 int nRole);
    int rowCount(const QModelIndex&) const
    {
        return m_nRows;
    }

    int columnCount(const QModelIndex&) const
    {
        return m_nColumns;
    }

    Qt::ItemFlags flags(const QModelIndex& index) const;

    QVariant headerData(int nSection, Qt::Orientation orientation,
                        int nRole) const;

    void setObjName(const QStringList& lst)
    {
        objName = lst;
    }

    void setRowsNames(const QStringList& lst)
    {
        rowsNames = lst;
    }

    void setValue(const int nRows, const int nColumns, const QVariant& val);

    QVariant getCellContent(const int nRows, const int nColumns) const;

    int getRowsCount() { return m_nRows; }

    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());

    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());

    void addColumt() { ++m_nColumns; }

    // ~TABLE_MODEL();

public slots:
    void clear();

};

#endif // TABLE_MODEL_H





















