#include "titlescreen.h"

TitleScreen::TitleScreen(QObject* parent) : QGraphicsScene(parent)
{
    this->addPixmap(QPixmap(":/titlescreen/resource/bgScreen.png"));
    QGraphicsPixmapItem* itemPix = this->addPixmap(QPixmap(":/titlescreen/resource/titleScreen.png"));
    itemPix->setScale(0.9);
    itemPix->setPos(80, 25);
    m_itemTxt = this->addText("Press Enter or Start to Play");
    m_itemTxt->setFont(QFont("Times", 25, QFont::Normal));
    m_itemTxt->setDefaultTextColor(QColor(Qt::white));
    m_itemTxt->setPos(this->width() / 2 + 60, this->height() / 2 + 170);

    m_sound->setLoops(-1);
    m_sound->play();

    this->startTimer(700);
}

void TitleScreen::timerEvent(QTimerEvent* event)
{
    static bool hideOrShow = true;
    if(hideOrShow)
    {
        m_itemTxt->hide();
        hideOrShow = false;
    }
    else
    {
        m_itemTxt->show();
        hideOrShow = true;
    }
    m_timerId = event->timerId();
}

void TitleScreen::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        if(m_timerId != -1)
            this->killTimer(m_timerId);
        m_timerId = -1;
        m_sound->stop();
        emit startGame();
    }
}

void TitleScreen::mousePressEvent(QGraphicsSceneMouseEvent*)
{
    if(m_timerId != -1)
        this->killTimer(m_timerId);
    m_timerId = -1;
    m_sound->stop();
    emit startGame();
}

