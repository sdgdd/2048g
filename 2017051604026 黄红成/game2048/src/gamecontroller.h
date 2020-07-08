#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QQmlListProperty>
#include <QVector>
class TileModel;
class QSettings;
class GameController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int score READ score NOTIFY scoreChanged)
    Q_PROPERTY(int maxScore READ maxScore NOTIFY maxScoreChanged)
public:
    explicit GameController(QObject *parent = nullptr);
    enum SaveFormat {
        Json, Binary
    };

    Q_INVOKABLE void moveLeft();
    Q_INVOKABLE void moveRight();
    Q_INVOKABLE void moveUp();
    Q_INVOKABLE void moveDown();
    Q_INVOKABLE void gameStart();



    int score() const;
    void setScore(int score);

    int maxScore() const;
    void setMaxScore(int maxScore);

signals:
    void gameIsOver();
    void generateNewTile(int i, int j, int value);
    void merge(int src, int dst, int value);
    void move(int src, int dst,int value);
    void scoreChanged();
    void maxScoreChanged();
public slots:

private:
    void initTiles();
    bool isGameOver();
    void newTile();
    void mergeTile(TileModel *a, TileModel *b);
    void moveTile(TileModel *a, TileModel *b);
    void clearFlag();
    void checkGame();
    QVector<QVector<TileModel *>> m_tiles;
    bool m_isMoved = false;
    int m_score = 0; 
    int m_maxScore = 0;
};

#endif
