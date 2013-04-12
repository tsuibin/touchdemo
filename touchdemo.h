#ifndef TOUCHDEMO_H
#define TOUCHDEMO_H

#include <QWidget>
#include <QStringList>
#include <QDir>
#include <QKeyEvent>
#include <QImage>
#include <QMessageBox>
#include <QEvent>
#include <QTouchEvent>

namespace Ui {
class TouchDemo;
}


class TopMenu;
class GridView;
class Preview;

class TouchDemo : public QWidget
{
    Q_OBJECT
    
public:
    explicit TouchDemo(QWidget *parent = 0);
    ~TouchDemo();


    QStringList getImgList(QString path);
signals:
    void pressESC();
    
private slots:
    void previousPage();
    void nextPage();
    void returnCurrentPage();


protected:
    void keyPressEvent ( QKeyEvent * event );
    void mouseMoveEvent ( QMouseEvent * event );
    void mousePressEvent ( QMouseEvent * event );
    void mouseReleaseEvent ( QMouseEvent * event );
    bool event( QEvent * event );

    void touchBegin(QEvent * event);


private:
    Ui::TouchDemo *ui;
    TopMenu *m_topMenu;
    GridView *m_gridView;
    Preview *m_preview;
    QList<QTouchEvent::TouchPoint> m_touchPoints;

    int m_mouseOldPosX;
    int m_movingDistance;
    int pageDirection;
    void automaticPage(int direction);
    void automaticImg(int direction);

};

#endif // TOUCHDEMO_H
