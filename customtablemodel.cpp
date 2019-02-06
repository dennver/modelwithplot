#include "customtablemodel.h"
#include <QtCore/QVector>
#include <QtCore/QRandomGenerator>
#include <QtCore/QRect>
#include <QtGui/QColor>
#include <QMimeData>
#include <QIODevice>
#include <QDataStream>

CustomTableModel::CustomTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

void CustomTableModel::setInternalData(QCPGraphDataContainer *data)
{
    m_data = data;
}

int CustomTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data->size();
}

int CustomTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant CustomTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        if (section % 2 == 0)
            return "x";
        else
            return "y";
    } else {
        return QString("%1").arg(section + 1);
    }
}

QVariant CustomTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        auto xy = m_data->at(index.row());
        return QVariant(index.column() == 0? xy->key : xy->value);
    }

    return QVariant();
}

bool CustomTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        auto xy = m_data->begin() + index.row();
        index.column() == 0? xy->key = value.toDouble() : xy->value = value.toDouble();
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags CustomTableModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

bool CustomTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    // only append
    beginInsertRows(parent, row, row + count - 1);
    for (auto i = 0; i < count ; ++i) {
        QVector<QCPGraphData> data(count, QCPGraphData(0.0, 0.0));
        m_data->add(data);
    }
    endInsertRows();
    return true;
}

bool CustomTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for (auto i = 0; i < count; ++i) {
        auto keyFrom = m_data->begin() + row;
        auto keyTo = m_data->begin() + row + count;

        m_data->remove(keyFrom->key, keyTo->key);
    }
    endRemoveRows();
    return true;
}

bool CustomTableModel::setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles)
{
    bool b = true;
    for (QMap<int, QVariant>::ConstIterator it = roles.begin(); it != roles.end(); ++it)
        b = b && setData(index, it.value(), it.key());
    return b;
}
