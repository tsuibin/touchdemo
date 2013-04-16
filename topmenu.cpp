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



	QString createPicPath = "images/create.png";
    ImgLabel *label_cre = new ImgLabel(this);
    label_cre->move(1144, 0);
    label_cre->setPixmap(QPixmap(createPicPath));
    label_cre->m_activeImg = "images/create_active.png";
    label_cre->m_normalImg = "images/create.png";




	QString retPicPath = "images/ret.png";

	ImgLabel *label_ret = new ImgLabel(this);
	label_ret->move(1076, 0);
	label_ret->setPixmap(QPixmap(retPicPath));
	label_ret->m_activeImg = "images/ret_active.png";
	label_ret->m_normalImg = "images/ret.png";

	connect(label_ret, SIGNAL(clicked()), this, SIGNAL(retSignal()));

}

TopMenu::~TopMenu()
{
	delete ui;
}
