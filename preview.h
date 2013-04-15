#ifndef PREVIEW_H
#define PREVIEW_H

#include "common.h"
#include <QWidget>




namespace Ui {
class Preview;
}

class FilterMenu;

class Preview : public QWidget
{
    Q_OBJECT
    
public:
    explicit Preview(QWidget *parent = 0);
    ~Preview();
    void setImagePath(QString path);

public slots:

    void test();
    void nextImage();
    void prevImage();
    void rotate( qreal angle );

protected:

    bool event ( QEvent * event );


private:
    Ui::Preview *ui;
    QString m_imgPath;
    QLabel *m_imageLabel;
    qreal totalScaleFactor;
    QPixmap m_currentImgPixmap;
    qreal m_scaled;

    FilterMenu *m_filterMenu;

    void resetFilterMenu();
};

#endif // PREVIEW_H
