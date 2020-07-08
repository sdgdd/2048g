#include "gamecontroller.h"
#include "tilemodel.h"
#include <QRandomGenerator>
#include <QDebug>
#include <QSettings>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <fstream>

GameController::GameController(QObject *parent)
    : QObject(parent)
{
    QFile loadFile(QStringLiteral("../game2048/src/save.json"));

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");

    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc( QJsonDocument::fromJson(saveData));
    int maxs=loadDoc.object()["maxscore"].toInt();

    if (maxs > 0)
        setMaxScore(maxs);

}

void GameController::moveLeft()
{
    clearFlag();
    for (int i=0;i<4;i++) {
        for (int j=1;j<4;j++) {
            int k = j;

            if (m_tiles[i][j]->isBlank())
                continue;
            while (k >= 0
                   && !m_tiles[i][k]->isMerged()
                   && (m_tiles[i][k]->isBlank() || m_tiles[i][k]->value() == m_tiles[i][j]->value()))
            {
                k--;
            }
            k++;
            if (k == j)
                continue;
            m_isMoved = true;
            if (m_tiles[i][k]->isBlank()) {  
                moveTile(m_tiles[i][j], m_tiles[i][k]);
            }
            else {   
                mergeTile(m_tiles[i][j], m_tiles[i][k]);
            }

        }
    }
    checkGame();
}

void GameController::moveRight()
{
    clearFlag();
    for (int i=0;i<4;i++) {
        for (int j=2;j>-1;j--) {
            int k = j;

            if (m_tiles[i][j]->isBlank())
                continue;
            while (k <= 3
                   && !m_tiles[i][k]->isMerged()
                   && (m_tiles[i][k]->isBlank() || m_tiles[i][k]->value() == m_tiles[i][j]->value()))
            {
                k++;
            }
            k--;
            if (k == j)
                continue;
            m_isMoved = true;
            if (m_tiles[i][k]->isBlank()) {
                moveTile(m_tiles[i][j], m_tiles[i][k]);
            }
            else {
                mergeTile(m_tiles[i][j], m_tiles[i][k]);
            }
        }
    }
    checkGame();
}

void GameController::moveUp()
{
    clearFlag();
    for (int j=0;j<4;j++) {
        for (int i=1;i<4;i++) {
            int k = i;

            if (m_tiles[i][j]->isBlank())
                continue;
            while (k >= 0
                   && !m_tiles[k][j]->isMerged()
                   && (m_tiles[k][j]->isBlank() || m_tiles[k][j]->value() == m_tiles[i][j]->value()))
            {
                k--;
            }
            k++;
            if (k == i)
                continue;
            m_isMoved = true;
            if (m_tiles[k][j]->isBlank()) {
                moveTile(m_tiles[i][j], m_tiles[k][j]);
            }
            else {
                mergeTile(m_tiles[i][j], m_tiles[k][j]);
            }
        }
    }
    checkGame();
}

void GameController::moveDown()
{
    clearFlag();
    for (int j=0;j<4;j++) {
        for (int i=2;i>-1;i--) {
            int k = i;
            if (m_tiles[i][j]->isBlank())
                continue;
            while (k <= 3
                   && !m_tiles[k][j]->isMerged()
                   && (m_tiles[k][j]->isBlank() || m_tiles[k][j]->value() == m_tiles[i][j]->value()))
            {
                k++;
            }
            k--;
            if (k == i)
                continue;
            m_isMoved = true;
            if (m_tiles[k][j]->isBlank()) {
                moveTile(m_tiles[i][j], m_tiles[k][j]);
            }
            else {
                mergeTile(m_tiles[i][j], m_tiles[k][j]);
            }
        }
    }
    checkGame();
}

void GameController::gameStart()
{
    initTiles();
    newTile();
    newTile();
    setScore(0);
}

void GameController::initTiles()
{
    m_tiles.clear();
    for (int i=0;i<4;i++) {
        QVector<TileModel *> tempV;
        for (int j=0;j<4;j++) {
            tempV.append(new TileModel(i,j,0));
        }
        m_tiles.append(tempV);
    }
}

bool GameController::isGameOver()
{
    for (int i=0;i<4;i++) {
        for (int j=0;j<4;j++) {
            if (m_tiles[i][j]->value() == 0)
                return false;
            if (i > 0 &&
                    m_tiles[i-1][j]->value() == m_tiles[i][j]->value())
                return false;
            if (j > 0 &&
                    m_tiles[i][j-1]->value() == m_tiles[i][j]->value())
                return false;
        }
    }
    return true;
}

void GameController::newTile()
{

    QVector<TileModel *>  zeros;
    for (int i=0;i<4;i++) {
        for (int j=0;j<4;j++) {
            if (m_tiles[i][j]->value() == 0) {
                zeros.append(m_tiles[i][j]);
            }
        }
    }
    if (zeros.size() == 0)
        return;

    int new_i = QRandomGenerator::global()->bounded(0,zeros.size()-1);
    int new_value = (QRandomGenerator::global()->bounded(1,100) < 90) ? 1 : 2;
    zeros[new_i]->setValue(new_value);
    emit generateNewTile(zeros[new_i]->x(),
                         zeros[new_i]->y(),
                         zeros[new_i]->value());

}

void GameController::mergeTile(TileModel *a, TileModel *b)
{
    b->setValue(b->value()+1);
    a->setValue(0);
    b->setIsMerged(true);
    setScore(m_score + pow(2, b->value()));
    if (m_score > m_maxScore)
        setMaxScore(m_score);
    emit merge(a->x()*4+a->y(),b->x()*4+b->y(),b->value());
}

void GameController::moveTile(TileModel *a, TileModel *b)
{
    b->setValue(a->value());
    b->setIsMerged(a->isMerged());
    a->setValue(0);
    a->setIsMerged(false);
    emit move(a->x()*4+a->y(),b->x()*4+b->y(),b->value());
}

void GameController::clearFlag()
{
    for (int i=0;i<4;i++)
        for (int j=0;j<4;j++)
            m_tiles[i][j]->setIsMerged(false);
    m_isMoved = false;
}

void GameController::checkGame()
{
    if (isGameOver()) {

        QFile saveFile("../game2048/src/save.json");
        if(!saveFile.open(QIODevice::WriteOnly)) {
                qWarning("Couldn't open save file.");
                return ;
            }
        QJsonObject json;
        json.insert("maxscore",m_maxScore);
        QJsonDocument saveDoc(json);
        saveFile.write(saveDoc.toJson());

        emit gameIsOver();
        return;
    }
    if (m_isMoved)
        newTile();
}

int GameController::maxScore() const
{
    return m_maxScore;
}

void GameController::setMaxScore(int maxScore)
{
    m_maxScore = maxScore;
    emit maxScoreChanged();

}

int GameController::score() const
{
    return m_score;
}

void GameController::setScore(int score)
{
    m_score = score;
    emit scoreChanged();
}









