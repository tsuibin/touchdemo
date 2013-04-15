#ifndef GIRDVIEW_H
#define GIRDVIEW_H

#include <QWidget>
#include "common.h"

namespace Ui {
class GirdView;
}

class ImgLabel;

class GridView : public QWidget
{
    Q_OBJECT
    
public:
    explicit GridView(QWidget *parent = 0);
    ~GridView();

signals:
    void imgClicked();

public slots:
    void initImg();
    void sendImgClickSignal();
    void nextPage();
    void prevPage();

protected:
    void mouseMoveEvent ( QMouseEvent * event );
    void mousePressEvent ( QMouseEvent * event );
    void mouseReleaseEvent ( QMouseEvent * event );
    bool event( QEvent * event );

private:
    Ui::GirdView *ui;

    QStringList m_imgList;
    int m_imgCount;
    QMap<int, ImgLabel*> m_imgLabelmap;

    void initPage(int page);
};

#endif // GIRDVIEW_H
