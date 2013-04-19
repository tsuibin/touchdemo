#include "randomview.h"
#include "ui_randomview.h"

 RandomView::RandomView(QWidget * parent):
QWidget(parent), ui(new Ui::RandomView)
{

    ui->setupUi(this);
    setAttribute(Qt::WA_AcceptTouchEvents);

    qsrand(QTime::currentTime().msec());

    setAttribute(Qt::WA_AcceptTouchEvents);



    totalScaleFactor = 1;
    m_touch = false;
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
    else if (t.size() > 20)
        max = 20;



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
            connect(label,SIGNAL(touch2Finger()),this,SLOT(touch2Slot()));
            connect(label,SIGNAL(touch1Finger()),this,SLOT(touch1Slot()));
            connect(label,SIGNAL(zoom(qreal)),this,SLOT(zoomSlot(qreal)));
            connect(label,SIGNAL(rotate(qreal)),this,SLOT(rotateSlot(qreal)));


    }
    m_currentLabel = NULL;

    m_angle = 0;

    zoom_timer = new QTimer(this);

}

RandomView::~RandomView()
{
	delete ui;
}

void RandomView::touch2Slot()
{
    m_touch = true;
}

void RandomView::touch1Slot()
{
    m_touch = false;
}

void RandomView::rotateSlot(qreal angle)
{
    AppEnv::currentImg = m_currentLabel->getImgPath();


    QImage image(AppEnv::currentImg);
    if (image.isNull()) {
        QMessageBox::information(this, tr("Image Viewer"),
                     tr("Cannot load %1.").arg(AppEnv::currentImg));
        return;
    }
    int w,h;
    w = 270 * m_currentLabel->m_scaled;
    h = 197 * m_currentLabel->m_scaled;
    QMatrix matrix;
    matrix.rotate(m_currentLabel->m_angle);

    m_currentImgPixmap = QPixmap::fromImage(image).scaled(w, h);
    m_currentImgPixmap = m_currentImgPixmap.transformed(matrix);


    m_currentLabel->setPixmap( m_currentImgPixmap); //Qt::KeepAspectRatio

    int lw = qSqrt(w*w+h*h);
    m_currentLabel->resize( lw, lw);

}

void RandomView::zoomSlot(qreal tmp)
{

    AppEnv::currentImg = m_currentLabel->getImgPath();


    QImage image(AppEnv::currentImg);
    if (image.isNull()) {
        QMessageBox::information(this, tr("Image Viewer"),
                     tr("Cannot load %1.").arg(AppEnv::currentImg));
        return;
    }
    int w,h;
    w = 270 * tmp;
    h = 197 * tmp;
    QMatrix matrix;
    matrix.rotate(m_currentLabel->m_angle);

    m_currentImgPixmap = QPixmap::fromImage(image).scaled(w, h);
    m_currentImgPixmap = m_currentImgPixmap.transformed(matrix);


    m_currentLabel->setPixmap( m_currentImgPixmap); //Qt::KeepAspectRatio

    int lw = qSqrt(w*w+h*h);
    m_currentLabel->resize( lw, lw);


}
bool RandomView::event( QEvent * event )
{

    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
    {





        //zoom+-

        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
        QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
        if (touchPoints.count() == 2) {


            // determine scale factor
            const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
            const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
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

            if(qAbs(tmp - m_scaled) > 0.12){



                AppEnv::currentImg = m_currentLabel->getImgPath();


                QImage image(AppEnv::currentImg);
                if (image.isNull()) {
                    QMessageBox::information(this, tr("Image Viewer"),
                                 tr("Cannot load %1.").arg(AppEnv::currentImg));
                    return QWidget::event(event);
                }



                int w,h;
                w = 270 * tmp;
                h = 197 * tmp;
                QMatrix matrix;
                matrix.rotate(m_currentLabel->m_angle);


                m_currentImgPixmap = QPixmap::fromImage(image).scaled(w, h);
                m_currentImgPixmap = m_currentImgPixmap.transformed(matrix);



                m_currentLabel->setPixmap( m_currentImgPixmap); //Qt::KeepAspectRatio

                int lw = qSqrt(w*w+h*h);
                m_currentLabel->resize( lw, lw);
                m_scaled = tmp;
            }
            //zoom+- end

        }
        return QWidget::event(event);

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


    int w,h;



    w = 290 * 2;
    h = 197 * 2;
    m_currentLabel = qobject_cast < ImgLabel * >(sender());
    m_currentLabel->raise();

    AppEnv::currentImg = m_currentLabel->getImgPath();

}

void RandomView::mousePressEvent ( QMouseEvent * event )
{

        if (m_currentLabel != NULL){
            m_mousePos =  event->globalPos() -m_currentLabel->geometry().topLeft();
        }

    QWidget::mousePressEvent(event);
}


void RandomView::mouseMoveEvent ( QMouseEvent * event )
{

    if (m_touch == false){

        m_currentLabel->move(event->globalPos() - m_mousePos);
    }
    QWidget::mouseMoveEvent(event);

}
void RandomView::mouseReleaseEvent ( QMouseEvent * event )
{

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

    int w = m_currentLabel->m_width * m_currentLabel->m_scaled;
    int h = m_currentLabel->m_heigh * m_currentLabel->m_scaled;

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


        int lw = qSqrt(w*w+h*h);
        m_currentLabel->resize( lw, lw);



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


        int lw = qSqrt(w*w+h*h);
        m_currentLabel->resize( lw, lw);


    }



}


