#ifndef RANDOMVIEW_H
#define RANDOMVIEW_H

#include <QWidget>
#include "common.h"

namespace Ui {
class RandomView;
}

class RandomView : public QWidget
{
    Q_OBJECT
    
public:
    explicit RandomView(QWidget *parent = 0);
    ~RandomView();
public slots:
    void imgClickSlot();
    void imgReleaseSlot();

protected:
    void mouseMoveEvent ( QMouseEvent * event ) ;
    void mousePressEvent ( QMouseEvent * event );
    void mouseReleaseEvent ( QMouseEvent * event );
    bool event( QEvent * event );
    void wheelEvent(QWheelEvent *event);

private:
    Ui::RandomView *ui;
    ImgLabel *m_currentLabel;
    QPoint m_mousePos;
    QPixmap m_currentImgPixmap;
    qreal m_angle;
    qreal totalScaleFactor;
    qreal m_scaled;
};

#endif // RANDOMVIEW_H
