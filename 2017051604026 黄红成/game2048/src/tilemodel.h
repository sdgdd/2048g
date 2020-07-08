#ifndef TILEMODEL_H
#define TILEMODEL_H

#include <QObject>
  
class TileModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(int y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(int isMerged READ isMerged )
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
public:
    explicit TileModel(QObject *parent = nullptr);
    TileModel(int x,int y, int value,QObject *parent = nullptr);

    int x() const;
    void setX(int x);

    int y() const;
    void setY(int y);

    bool isMerged() const;
    void setIsMerged(bool isMerged);

    int value() const;
    void setValue(int value);

    bool isBlank() const;

signals:
    void xChanged();
    void yChanged();
    void valueChanged();
private:
    int m_x;
    int m_y;

    bool m_isMerged = false;
    int m_value;
};

#endif
