
#include "gridview.h"
#include "ui_gridview.h"


 GridView::GridView(QWidget * parent):
QWidget(parent), ui(new Ui::GirdView)
{
	ui->setupUi(this);
    setAttribute(Qt::WA_AcceptTouchEvents);

	this->setStyleSheet(QString::fromUtf8("QLabel {\n"
					      "border-width:10px;\n"
					      "border-style: solid;\n"
					      "border-color: #676767;\n"
					      " }\n" ""));

	initImg();

    AppEnv::currentPage = 0;


}

GridView::~GridView()
{
	delete ui;
}

void GridView::initImg()
{

    initPage(0);
    initPage(1);
    initPage(2);
    initPage(3);

}

void GridView::initPage(int page)
{
    QDir pdir;
    pdir.setPath(AppEnv::imgPath);
    QFileInfoList t = pdir.entryInfoList(QDir::Files);


    int col = 0;
    int row = 0;
    int count = 1;
    int imgX = 0;
    int imgY = 0;
    int max = 12+12*page;

    m_imgCount = t.size();
    if (t.size() < 12) {
        max = t.size();

    }


    for (int i = page*12; i < max; i++) {


            m_imgList.append(t.at(i).absoluteFilePath());
            QPixmap tmp =
                QPixmap(t.at(i).absoluteFilePath()).scaled(290, 197);
            imgX = 44 + tmp.width() * col + 10 +1280*page;
            imgY = 78 + tmp.height() * row + 10;

            if (count % 4 == 0)
                row++;
            col++;

            if (col == 4)
                col = 0;

            ImgLabel *label = new ImgLabel(this);
            label->setImgPath(t.at(i).absoluteFilePath());
            label->setPixmap(tmp);
            label->m_index = i;
            label->move(imgX, imgY);
            label->show();
            m_imgLabelmap.insert(i, label);
            count++;
            connect(label, SIGNAL(clicked()), this,
                SLOT(sendImgClickSignal()));

    }


}

void GridView::sendImgClickSignal()
{

	ImgLabel *label = qobject_cast < ImgLabel * >(sender());

	AppEnv::currentImg = label->getImgPath();
	AppEnv::currentImageIndex = label->m_index;


	emit imgClicked();
}



void GridView::nextPage()
{
    AppEnv::currentPage++;

    int star = AppEnv::currentPage * 12;//3 * 12 =  36,, 4*12 = 48
    int key = 0;


    if(star >=  AppEnv::imgCount ){ //no

        AppEnv::currentPage = qCeil(AppEnv::imgCount/12)-1;

        return;

    }
    for(int i = star;i < AppEnv::imgCount && i < star + 12; i++)
    {


        QPixmap tmp = QPixmap( AppEnv::imgPath + AppEnv::imgList.at(i) ).scaled(290, 197);

        ImgLabel *label =  m_imgLabelmap.value(key);
        label->setImgPath( AppEnv::imgPath + AppEnv::imgList.at(i) );
        label->setPixmap(tmp);
        label->m_index = i;

            key++;

    }

}

void GridView::prevPage()
{
    AppEnv::currentPage--;//3-1 = 2

    int star = AppEnv::currentPage * 12+12; // 24+12 = 36
    int key = 0;
    if(star <= 0){

        AppEnv::currentPage = 0;
        return;

    }


    for(int i = star-12; i < star; i++)//24~36
    {

        QPixmap tmp = QPixmap( AppEnv::imgPath + AppEnv::imgList.at(i) ).scaled(290, 197);

        ImgLabel *label =  m_imgLabelmap.value(key);
        label->setImgPath( AppEnv::imgPath + AppEnv::imgList.at(i) );
        label->setPixmap(tmp);
        label->m_index = i;

            key++;

    }

}



void GridView::mouseMoveEvent ( QMouseEvent * event )
{
    QWidget::mouseMoveEvent(event);

}

void GridView::mousePressEvent ( QMouseEvent * event )
{
    QWidget::mousePressEvent(event);

}

void GridView::mouseReleaseEvent ( QMouseEvent * event )
{
    QWidget::mouseReleaseEvent(event);
}

bool GridView::event( QEvent * event )
{
    return QWidget::event(event);
}


