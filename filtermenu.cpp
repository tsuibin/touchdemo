#include "filtermenu.h"
#include "ui_filtermenu.h"


 FilterMenu::FilterMenu(QWidget * parent):
QWidget(parent), ui(new Ui::FilterMenu)
{
	ui->setupUi(this);


    m_effect0 = new QGraphicsBlurEffect(this);
    m_effect0->setBlurRadius(0.2);


    m_effect1 = new QGraphicsColorizeEffect(this);
    m_effect1->setColor(QColor(0,0,192));

    m_effect2 = new QGraphicsDropShadowEffect(this);
    m_effect2->setOffset(8,8);


    m_effect3 = new QGraphicsColorizeEffect(this);
    m_effect3->setColor(QColor(192,0,0));

    ui->pushButton->setAttribute(Qt::WA_AcceptTouchEvents);
    ui->pushButton_2->setAttribute(Qt::WA_AcceptTouchEvents);
    ui->pushButton_3->setAttribute(Qt::WA_AcceptTouchEvents);
    ui->pushButton_4->setAttribute(Qt::WA_AcceptTouchEvents);

    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(blurEffectSlot()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(colorizeEffectSlot()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(dropShadowEffectSlot()));
    connect(ui->pushButton_4,SIGNAL(clicked()),this,SLOT(colorizeEffectSlot1()));

}

FilterMenu::~FilterMenu()
{
	delete ui;
}

void FilterMenu::on_pushButton_clicked()
{


}

void FilterMenu::initFilterItem()
{



    QPixmap *pm = new QPixmap( AppEnv::currentImg );

    bm = pm->mask();

     QIcon *m_ico = new QIcon(AppEnv::currentImg);

    ui->pushButton->setMask( bm );
    ui->pushButton->setIconSize(QSize(80 ,60));
    ui->pushButton->setIcon(*m_ico);
    ui->pushButton->setGraphicsEffect(m_effect0);


    ui->pushButton_2->setMask( bm );
    ui->pushButton_2->setIconSize(QSize(80 ,60));
    ui->pushButton_2->setIcon(*m_ico);
    ui->pushButton_2->setGraphicsEffect(m_effect1);

    ui->pushButton_3->setMask( bm );
    ui->pushButton_3->setIconSize(QSize(80 ,60));
    ui->pushButton_3->setIcon(*m_ico);
    ui->pushButton_3->setGraphicsEffect(m_effect2);

    ui->pushButton_4->setMask( bm );
    ui->pushButton_4->setIconSize(QSize(80 ,60));
    ui->pushButton_4->setIcon(*m_ico);
    ui->pushButton_4->setGraphicsEffect(m_effect3);


    delete m_ico;
    delete pm;
}


void FilterMenu::blurEffectSlot()
{
    emit blurEffect();
}
void FilterMenu::colorizeEffectSlot()
{

    emit colorizeEffect();

}
void FilterMenu::dropShadowEffectSlot()
{

     emit dropShadowEffect();
}
void FilterMenu::colorizeEffectSlot1()
{
    emit colorizeEffect1();
}
