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

private:
    Ui::RandomView *ui;
    ImgLabel *m_currentLabel;
    QPoint m_mousePos;
};

#endif // RANDOMVIEW_H
