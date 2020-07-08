#include "tilemodel.h"

TileModel::TileModel(QObject *parent) : QObject(parent)
{

}

TileModel::TileModel(int x, int y, int value, QObject *parent)
    : QObject(parent),
      m_x(x),
      m_y(y),
      m_value(value)
{

}

int TileModel::x() const
{
    return m_x;
}

void TileModel::setX(int x)
{
    m_x = x;
}

int TileModel::y() const
{
    return m_y;
}

void TileModel::setY(int y)
{
    m_y = y;
}

bool TileModel::isMerged() const
{
    return m_isMerged;
}

void TileModel::setIsMerged(bool isMerged)
{
    m_isMerged = isMerged;
}

int TileModel::value() const
{
    return m_value;
}

void TileModel::setValue(int value)
{
    m_value = value;
}

bool TileModel::isBlank() const
{
      return 0 == m_value;
}




