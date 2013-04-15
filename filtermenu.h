#ifndef FILTERMENU_H
#define FILTERMENU_H

#include <QWidget>
#include "common.h"

namespace Ui {
class FilterMenu;
}

class FilterMenu : public QWidget
{
    Q_OBJECT
    
public:
    explicit FilterMenu(QWidget *parent = 0);
    ~FilterMenu();
    void initFilterItem();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::FilterMenu *ui;

    QGraphicsBlurEffect *m_effect0;
    QGraphicsColorizeEffect *m_effect1;
    QGraphicsDropShadowEffect *m_effect2;
    QGraphicsColorizeEffect *m_effect3;

    QBitmap bm;

};

#endif // FILTERMENU_H
