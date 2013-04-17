#include "topmenu.h"
#include "ui_topmenu.h"
#include "imglabel.h"

 TopMenu::TopMenu(QWidget * parent):
QWidget(parent), ui(new Ui::TopMenu)
{

	ui->setupUi(this);
    ui->label_title->setText("看图 Demo");
	QString picPath = "images/menu_bg.jpg";
	ui->label->setPixmap(QPixmap(picPath));


	QString editPicPath = "images/edit.png";
    ImgLabel *label_edit = new ImgLabel(this);
    label_edit->move(1212, 0);
    label_edit->setPixmap(QPixmap(editPicPath));
    label_edit->m_activeImg = "images/edit_active.jpg";
    label_edit->m_normalImg = "images/edit.png";


//    QString createPicPath = "images/create.png";
//    ImgLabel *label_cre = new ImgLabel(this);
//    label_cre->move(1144, 0);
//    label_cre->setPixmap(QPixmap(createPicPath));
//    label_cre->m_activeImg = "images/create_active.png";
//    label_cre->m_normalImg = "images/create.png";

    QString randomPicPath = "images/random.png";
    ImgLabel *label_random = new ImgLabel(this);
    label_random->move(1144, 0);
    label_random->setPixmap(QPixmap(randomPicPath));
    label_random->m_activeImg = "images/random_active.png";
    label_random->m_normalImg = "images/random.png";
    label_random->show();


    QString gridPicPath = "images/grid.png";
    ImgLabel *label_grid = new ImgLabel(this);
    label_grid->move(1144, 0);
    label_grid->setPixmap(QPixmap(gridPicPath));
    label_grid->m_activeImg = "images/grid_active.png";
    label_grid->m_normalImg = "images/grid.png";
    label_grid->hide();



	QString retPicPath = "images/ret.png";
	ImgLabel *label_ret = new ImgLabel(this);
	label_ret->move(1076, 0);
	label_ret->setPixmap(QPixmap(retPicPath));
	label_ret->m_activeImg = "images/ret_active.png";
	label_ret->m_normalImg = "images/ret.png";



    connect(label_grid,SIGNAL(clicked()),label_random,SLOT(show()));
    connect(label_grid,SIGNAL(clicked()),label_grid,SLOT(hide()));


    connect(label_random,SIGNAL(clicked()),label_grid,SLOT(show()));
    connect(label_random,SIGNAL(clicked()),label_random,SLOT(hide()));



    connect(label_ret, SIGNAL(clicked()), this, SIGNAL(retSignal()));
    connect(label_grid,SIGNAL(clicked()),this,SIGNAL(gridSignal()));
    connect(label_random,SIGNAL(clicked()),this,SIGNAL(randomSignal()));

    connect(label_ret,SIGNAL(clicked()),label_random,SLOT(show()));
    connect(label_ret,SIGNAL(clicked()),label_grid,SLOT(hide()));

}

TopMenu::~TopMenu()
{
	delete ui;
}


