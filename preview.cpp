#include "preview.h"
#include "ui_preview.h"

#include "appenv.h"

#include "imglabel.h"

 Preview::Preview(QWidget * parent):
QWidget(parent), ui(new Ui::Preview)
{
	ui->setupUi(this);
    setAttribute(Qt::WA_AcceptTouchEvents);//add touch device support

	m_imageLabel = new QLabel(this);
	m_imageLabel->resize(1280, 752);
    totalScaleFactor = 1;

    m_filterMenu = new FilterMenu(this);
    m_filterMenu->move(0, 725);

    connect(m_filterMenu,SIGNAL(blurEffect()),this,SLOT(blurEffectSlot()));
    connect(m_filterMenu,SIGNAL(colorizeEffect()),this,SLOT(colorizeEffectSlot()));
    connect(m_filterMenu,SIGNAL(dropShadowEffect()),this,SLOT(dropShadowEffectSlot()));
    connect(m_filterMenu,SIGNAL(colorizeEffect1()),this,SLOT(colorizeEffectSlot1()));
}

Preview::~Preview()
{
	delete ui;
}

void Preview::blurEffectSlot()
{
    QGraphicsBlurEffect *m_effect0 = new QGraphicsBlurEffect();
    m_effect0->setBlurRadius(0.2);
    m_imageLabel->setGraphicsEffect(m_effect0);
}

void Preview::colorizeEffectSlot()
{

    QGraphicsColorizeEffect *m_effect1 = new QGraphicsColorizeEffect();
    m_effect1->setColor(QColor(0,0,192));
    m_imageLabel->setGraphicsEffect(m_effect1);// setGraphicsEffect will free effect space automatic
}

void Preview::colorizeEffectSlot1()
{
    QGraphicsColorizeEffect *m_effect3 = new QGraphicsColorizeEffect();
    m_effect3->setColor(QColor(192,0,0));
    m_imageLabel->setGraphicsEffect(m_effect3);
}

void Preview::dropShadowEffectSlot()
{
    QGraphicsDropShadowEffect *m_effect2 = new QGraphicsDropShadowEffect();
    m_effect2->setOffset(8,8);
    m_imageLabel->setGraphicsEffect(m_effect2);
}


void Preview::resetFilterMenu()
{

    m_filterMenu->initFilterItem();

}
void Preview::setImagePath(QString path)
{
	m_imgPath = path;

}

void Preview::rotate( qreal angle )
{

    QMatrix matrix;
    matrix.rotate(angle);
    m_currentImgPixmap = m_currentImgPixmap.transformed(matrix);
    m_imageLabel->setPixmap(m_currentImgPixmap);

}

void Preview::test()
{

	QString fileName = AppEnv::currentImg;
	QImage image(fileName);

	if (image.isNull()) {
		QMessageBox::information(this, tr("Image Viewer"),
					 tr("Cannot load %1.").arg(fileName));
		return;
	}

    m_currentImgPixmap = QPixmap::fromImage(image).scaled(1280, 752);
    m_imageLabel->setPixmap(m_currentImgPixmap);
	m_imageLabel->move(0, 48);


    resetFilterMenu();



}

void Preview::nextImage()
{

    if (AppEnv::currentImageIndex >= AppEnv::imgCount-1)
    {
        AppEnv::currentImageIndex = AppEnv::imgCount-1;
        return;
    }
     AppEnv::currentImageIndex++;
    int no =  AppEnv::currentImageIndex;

	QString fileName =
        AppEnv::imgPath + AppEnv::imgList.at(no);

    AppEnv::currentImg = fileName;

	QImage image(fileName);

	if (image.isNull()) {
		QMessageBox::information(this, tr("Image Viewer"),
					 tr("Cannot load %1.").arg(fileName));
		return;
	}
    m_currentImgPixmap = QPixmap::fromImage(image).scaled(1280, 752);
    m_imageLabel->setPixmap(m_currentImgPixmap);
	m_imageLabel->move(0, 48);
    totalScaleFactor = 1;
    resetFilterMenu();

}

void Preview::prevImage()
{

    if (AppEnv::currentImageIndex <= 0)
    {
        AppEnv::currentImageIndex = 0;
		return;
    }

    AppEnv::currentImageIndex--;

	QString fileName =
        AppEnv::imgPath + AppEnv::imgList.at(AppEnv::currentImageIndex);

    AppEnv::currentImg = fileName;

	QImage image(fileName);

	if (image.isNull()) {
		QMessageBox::information(this, tr("Image Viewer"),
					 tr("Cannot load %1.").arg(fileName));
		return;
	}

    m_currentImgPixmap = QPixmap::fromImage(image).scaled(1280, 752);
    m_imageLabel->setPixmap(m_currentImgPixmap);
	m_imageLabel->move(0, 48);
    totalScaleFactor = 1;
    resetFilterMenu();

}


bool Preview::event( QEvent * event )
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
            if(tmp > 2)
            {
                tmp = 1.5;

            }

            if(qAbs(tmp - m_scaled) > 0.12){

                m_imageLabel->setPixmap(m_currentImgPixmap.scaled(1280*tmp, 752 * tmp)); //Qt::KeepAspectRatio
                m_imageLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
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
