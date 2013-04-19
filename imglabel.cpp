#include "imglabel.h"
#include "appenv.h"

#include <QDebug>

QString AppEnv::currentImg;

 ImgLabel::ImgLabel(QWidget * parent):
QLabel(parent)
{

    setAttribute(Qt::WA_AcceptTouchEvents);
	m_activeImg = "";
	m_normalImg = "";
    m_preview = false;
    m_angle = 0;
    m_totalScaleFactor = 1;
    m_scaled = 1;
    zoom_timer = new QTimer(this);
    rotate_timer = new QTimer(this);
    connect(zoom_timer,SIGNAL(timeout()),zoom_timer,SLOT(stop()));
    connect(rotate_timer,SIGNAL(timeout()),rotate_timer,SLOT(stop()));

    m_rx = 0;
    m_ry = 0;
    m_oldAngle = 0;



}

void ImgLabel::mousePressEvent(QMouseEvent * event)
{

    m_preview = true;
    m_movingDistance = 0;
    m_mouseOldPosX = event->x();

	if (event->button() == Qt::LeftButton) {

		if (!m_activeImg.isEmpty())
			this->setPixmap(QPixmap(m_activeImg));

		emit pressed();
	}

	QLabel::mousePressEvent(event);

}

void ImgLabel::mouseMoveEvent ( QMouseEvent * event )
{
    m_preview = false;
    int x = event->x() - m_mouseOldPosX;
    m_movingDistance += x;
    m_mouseOldPosX = event->x();

    QLabel::mouseMoveEvent(event);

}

void ImgLabel::mouseReleaseEvent(QMouseEvent * event)
{

    if (!m_normalImg.isEmpty())
        this->setPixmap(QPixmap(m_normalImg));



    pageDirection = 0;
    if (m_movingDistance > 200)
    {
        pageDirection = 1;
    }
    else if (m_movingDistance < (-200))
    {
        pageDirection = -1;
    }


    if (event->button() == Qt::LeftButton && pageDirection == 0 && m_preview == true)
    {

		emit released();
		emit clicked();

	}

    QLabel::mouseReleaseEvent(event);

}

QString ImgLabel::getImgPath()
{
	return this->m_imgPath;
}

void ImgLabel::setImgPath(QString path)
{

	this->m_imgPath = path;
}


void ImgLabel::test()
{

	//sence
	QGraphicsScene *scene = new QGraphicsScene(0, 0, 1280, 800);	//x y width height

	//item
	QPixmap *img = new QPixmap(m_imgPath);

	Pixmap *item = new Pixmap(*img);

	item->setOffset(0, 0);

	scene->addItem(item);

	// Ui
	View *view = new View(scene);
	view->setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Deepin See"));
	view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	view->setCacheMode(QGraphicsView::CacheBackground);
	view->
	    setRenderHints(QPainter::Antialiasing | QPainter::
			   SmoothPixmapTransform);

	view->show();

}

bool ImgLabel::event(QEvent * event )
{


    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
    {
        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
        QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
        if (touchPoints.count() == 2) {
            emit touch2Finger();

         //   qDebug() <<"touch zoom";

            // determine scale factor
            const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
            const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
            qreal currentScaleFactor =
                    QLineF(touchPoint0.pos(), touchPoint1.pos()).length()
                    / QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();

            if(!rotate_timer->isActive() )
            {

                m_ry = touchPoint1.lastScreenPos().ry() - touchPoint1.startScreenPos().ry();

                 if(qAbs(m_ry - m_rx) >5)
                 {
                    m_rx = m_ry;
                    if(m_ry > 5)
                    {

                        this->m_angle += m_ry/10 * 4.5 - m_oldAngle;
                        m_oldAngle = m_ry/10 * 4.5;
                    }else
                    {

                        this->m_angle -= qAbs(m_ry/10 * 4.5) - m_oldAngle;

                        m_oldAngle = qAbs(m_ry/10 * 4.5);

                    }
                    emit rotate(this->m_angle);

                    rotate_timer->start(100);
                 }



            }
            if (touchEvent->touchPointStates() & Qt::TouchPointReleased) {
                // if one of the fingers is released, remember the current scale
                // factor so that adding another finger later will continue zooming
                // by adding new scale factor to the existing remembered value.
                m_totalScaleFactor *= currentScaleFactor;
                currentScaleFactor = 1;

                m_ry = 0;
                m_oldAngle = 0;



            }

            qreal tmp =  m_totalScaleFactor * currentScaleFactor;


            if (m_scaled == 1)
            {
                m_scaled = tmp;
            }

            if(qAbs(tmp - m_scaled) > 0.12){
                if(!zoom_timer->isActive())
                {
                    emit zoom(tmp);
                    m_scaled = tmp;
                    zoom_timer->start(100);
                }

            }


        }
        else if (touchPoints.count() == 1) {
            emit touch1Finger();
        }


        return QLabel::event(event);
    }
    default:
        break;
    }
   return QLabel::event(event);
}
