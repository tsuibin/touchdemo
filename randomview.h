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
        void touch1Slot();
        void touch2Slot();
        void zoomSlot(qreal tmp);
        void rotateSlot(qreal angle);
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
    bool m_touch;
    QTimer *zoom_timer;
};

#endif // RANDOMVIEW_H
