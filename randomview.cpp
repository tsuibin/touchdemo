#include "randomview.h"
#include "ui_randomview.h"

 RandomView::RandomView(QWidget * parent):
QWidget(parent), ui(new Ui::RandomView)
{
     qDebug();
    ui->setupUi(this);
    setAttribute(Qt::WA_AcceptTouchEvents);//add touch device support

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
            label->m_width = 290;
            label->m_heigh = 197;
            label->m_angle = 0;
            label->raise();
            label->show();
            count++;


            connect(label,SIGNAL(pressed()),this,SLOT(imgClickSlot()));
            connect(label,SIGNAL(released()),this,SLOT(imgReleaseSlot()));

    }
    m_currentLabel = NULL;

    m_angle = 0;


}

RandomView::~RandomView()
{
	delete ui;
}



bool RandomView::event( QEvent * event )
{


    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
    {
        qDebug() <<"touch";

        //zoom+-
        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
        QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
        if (touchPoints.count() == 2) {
            // determine scale factor
            const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
            const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();


            qDebug() <<" QLineF(touchPoint0.pos(), touchPoint1.pos()).length()" <<  QLineF(touchPoint0.pos(), touchPoint1.pos()).length()
                    << "QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length()" << QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();



            qreal currentScaleFactor =
                    QLineF(touchPoint0.pos(), touchPoint1.pos()).length()
                    / QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();



            if (touchEvent->touchPointStates() & Qt::TouchPointReleased) {
                // if one of the fingers is released, remember the current scale
                // factor so that adding another finger later will continue zooming
                // by adding new scale factor to the existing remembered value.
                totalScaleFactor *= currentScaleFactor;
                currentScaleFactor = 1;
            }

            qreal tmp =  totalScaleFactor * currentScaleFactor;
            if(tmp > 2)
            {
                tmp = 1.5;

            }

            if(qAbs(tmp - m_scaled) > 0.12){

                QMatrix matrix;
                matrix.rotate(m_currentLabel->m_angle);
                m_currentImgPixmap = m_currentImgPixmap.transformed(matrix);
                m_currentLabel->setPixmap(m_currentImgPixmap.scaled(m_currentLabel->m_width*tmp, m_currentLabel->m_heigh * tmp)); //Qt::KeepAspectRatio

                m_scaled = tmp;
            }
            //zoom+- end

        }
        QWidget::event(event);
        return true;
    }
    default:
        break;
    }
   return QWidget::event(event);
}

void RandomView::imgReleaseSlot()
{
   // m_currentLabel = NULL;
}

void RandomView::imgClickSlot()
{
    qDebug();

    int w,h;


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

    QMatrix matrix;
    matrix.rotate(m_currentLabel->m_angle);
    m_currentImgPixmap = QPixmap::fromImage(image).scaled(m_currentLabel->m_width, m_currentLabel->m_heigh);
    m_currentImgPixmap = m_currentImgPixmap.transformed(matrix);

    m_currentLabel->setPixmap( m_currentImgPixmap);


    qDebug();
}

void RandomView::mousePressEvent ( QMouseEvent * event )
{
    qDebug();
    if (m_currentLabel != NULL){
        m_mousePos =  event->globalPos() -m_currentLabel->geometry().topLeft();

    }
    qDebug();
}


void RandomView::mouseMoveEvent ( QMouseEvent * event )
{
  //  qDebug();

    m_currentLabel->move(event->globalPos() - m_mousePos);

}
void RandomView::mouseReleaseEvent ( QMouseEvent * event )
{
    qDebug();



}



void RandomView::wheelEvent(QWheelEvent *event)
{
    QString str = "";

    switch(event->orientation())
    {
    case Qt::Horizontal:
       // str = "Horizontal Whell Move";
        break;
    case Qt::Vertical:
       // str = "Vertical Whell Move";
        break;
    default:
        break;
    }

    int w = m_currentLabel->m_width;
    int h = m_currentLabel->m_heigh;

    if(event->delta()>0)
{



        AppEnv::currentImg = m_currentLabel->getImgPath();


        QImage image(AppEnv::currentImg);
        if (image.isNull()) {
            QMessageBox::information(this, tr("Image Viewer"),
                         tr("Cannot load %1.").arg(AppEnv::currentImg));
            return;
        }

        QMatrix matrix;
        m_angle = m_currentLabel->m_angle;
        m_angle += 5;
        matrix.rotate(m_angle);
        m_currentLabel->m_angle = m_angle;
        m_currentImgPixmap = QPixmap::fromImage(image).scaled(w, h);
        m_currentImgPixmap = m_currentImgPixmap.transformed(matrix);
        m_currentLabel->setPixmap(m_currentImgPixmap);

        m_currentLabel->setPixmap( m_currentImgPixmap );
        m_currentLabel->resize(w+100,h+100);
        m_currentLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        qDebug();

 }
    else
    {

        AppEnv::currentImg = m_currentLabel->getImgPath();
        QImage image(AppEnv::currentImg);
        if (image.isNull()) {
            QMessageBox::information(this, tr("Image Viewer"),
                         tr("Cannot load %1.").arg(AppEnv::currentImg));
            return;
        }

        QMatrix matrix;
        m_angle = m_currentLabel->m_angle;
        m_angle -= 5;
        matrix.rotate(m_angle);

        m_currentLabel->m_angle = m_angle;
        m_currentImgPixmap = QPixmap::fromImage(image).scaled(w, h);
        m_currentImgPixmap = m_currentImgPixmap.transformed(matrix);
        m_currentLabel->setPixmap(m_currentImgPixmap);

        m_currentLabel->setPixmap( m_currentImgPixmap );
        m_currentLabel->resize(w+100,h+100);
        m_currentLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        qDebug();

    }



}


