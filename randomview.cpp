#include "randomview.h"
#include "ui_randomview.h"

 RandomView::RandomView(QWidget * parent):
QWidget(parent), ui(new Ui::RandomView)
{
    ui->setupUi(this);

 qsrand(QTime::currentTime().msec());

    //setAttribute(Qt::WA_AcceptTouchEvents);

    this->setStyleSheet(QString::fromUtf8("QLabel {\n"
                          "border-width:2px;\n"
                          "border-style: solid;\n"
                          "border-color: #676767;\n"
                          " }\n" ""));



    int page = 0;
    QDir pdir;
    pdir.setPath(AppEnv::imgPath);
    QFileInfoList t = pdir.entryInfoList(QDir::Files);


    int col = 0;
    int row = 0;
    int count = 1;
    int imgX = 0;
    int imgY = 0;
    int max = 12+12*page;

    max = t.size();
    if (t.size() < 24) {
        max = t.size();

    }



    for (int i = page*12; i < max; i++) {

            QPixmap tmp =
                QPixmap(t.at(i).absoluteFilePath()).scaled(290, 197);
            imgX = 44 + tmp.width() * col + 10 +1280*page;
            imgY = 78 + tmp.height() * row + 10;
            imgX = qrand()%1000;
            imgY = rand()%680;
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
            label->raise();
            label->show();
            count++;

           // connect(label,SIGNAL(clicked()),label,SLOT(raise()));
            connect(label,SIGNAL(pressed()),this,SLOT(imgClickSlot()));
            connect(label,SIGNAL(released()),this,SLOT(imgReleaseSlot()));

    }


}

RandomView::~RandomView()
{
	delete ui;
}

void RandomView::imgReleaseSlot()
{
   // m_currentLabel = NULL;
}

void RandomView::imgClickSlot()
{
    int w,h;
    qDebug() <<"1";
    if (m_currentLabel != NULL)
    {
        w = 290;
        h = 197;

        QImage image(AppEnv::currentImg);
        if (image.isNull()) {
            QMessageBox::information(this, tr("Image Viewer"),
                         tr("Cannot load %1.").arg(AppEnv::currentImg));
            return;
        }
        m_currentLabel->setPixmap( QPixmap::fromImage(image).scaled(w, h));
        m_currentLabel->resize(w,h);
    }

    w = 290 * 2;
    h = 197 * 2;
    m_currentLabel = qobject_cast < ImgLabel * >(sender());
    m_currentLabel->raise();

    AppEnv::currentImg = m_currentLabel->getImgPath();


    QImage image(AppEnv::currentImg);
    if (image.isNull()) {
        QMessageBox::information(this, tr("Image Viewer"),
                     tr("Cannot load %1.").arg(AppEnv::currentImg));
        return;
    }
    m_currentLabel->setPixmap( QPixmap::fromImage(image).scaled(w, h));
    m_currentLabel->resize(w,h);


}

void RandomView::mousePressEvent ( QMouseEvent * event )
{
    qDebug()  ;
    if (m_currentLabel != NULL)
        m_mousePos =  event->globalPos() -m_currentLabel->geometry().topLeft();
}


void RandomView::mouseMoveEvent ( QMouseEvent * event )

{

    m_currentLabel->move(event->globalPos() - m_mousePos);

}
void RandomView::mouseReleaseEvent ( QMouseEvent * event )
{

}


