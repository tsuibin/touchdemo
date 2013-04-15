#include "touchdemo.h"
#include "ui_touchdemo.h"



#define PREVIOUS_PAGE   1
#define NEXT_PAGE       -1
#define CURRENT_PAGE    0


 TouchDemo::TouchDemo(QWidget * parent):
QWidget(parent), ui(new Ui::TouchDemo)
{
	ui->setupUi(this);
    this->setWindowTitle("看图 Demo");
	this->setMaximumSize(1280, 800);
	this->setMinimumSize(1280, 800);

    setAttribute(Qt::WA_AcceptTouchEvents);//add touch device support


	QStringList imgList = AppEnv::initImgList();

	QString picPath = "images/bg.jpg";
	ui->label->setPixmap(QPixmap(picPath));


	m_preview = new Preview(this);
    m_preview->hide();

    m_gridView = new GridView(this);
    m_gridView->show();

	m_topMenu = new TopMenu(this);
	m_topMenu->show();




    connect(m_gridView, SIGNAL(imgClicked()), m_preview, SLOT(test()));
    connect(m_gridView, SIGNAL(imgClicked()), m_preview, SLOT(show()));

    connect(m_gridView, SIGNAL(imgClicked()), m_gridView, SLOT(hide()));

    connect(m_topMenu, SIGNAL(retSignal()), m_gridView, SLOT(show()));
	connect(m_topMenu, SIGNAL(retSignal()), m_preview, SLOT(hide()));

	connect(this, SIGNAL(pressESC()), m_preview, SLOT(hide()));
    connect(this, SIGNAL(pressESC()), m_gridView, SLOT(show()));

    m_nextPageTimer = new QTimer(this);
    connect(m_nextPageTimer,SIGNAL(timeout()),this,SLOT(moveNextPage()));

    m_prevPageTimer = new QTimer(this);
    connect(m_prevPageTimer,SIGNAL(timeout()),this,SLOT(movePrevPage()));

    m_currentPageTimer = new QTimer(this);
    connect(m_currentPageTimer,SIGNAL(timeout()),this,SLOT(returnCurrentPage()));

    qDebug() << "总页数" << AppEnv::pageCount;

    m_movingDistance = 0;


}

TouchDemo::~TouchDemo()
{
	delete m_topMenu;
    delete m_gridView;
	delete ui;
}



void TouchDemo::previousPage()
{
    qDebug() <<"上一页";
    if (AppEnv::currentPage == 0)
    {
        qDebug() <<"当前是第一页";
        returnCurrentPage();
        return;
    }

    if(m_gridView->isVisible())
    {
        m_prevPageTimer->start(10);
    }

}
void TouchDemo::nextPage()
{


    if (AppEnv::currentPage+1 >= AppEnv::pageCount)
    {
        returnCurrentPage();
        return;
    }
    if(m_gridView->isVisible())
    {
        m_nextPageTimer->start(10);
    }
}

void TouchDemo::movePrevPage()
{

    int x = -1280*(AppEnv::currentPage - 1 );
    if(m_gridView->x() < x)
    {
        m_gridView->move(m_gridView->x()+50,0);
    }else{
        m_gridView->move( x, 0);
        AppEnv::currentPage--;
        m_prevPageTimer->stop();
    }

}

void TouchDemo::moveNextPage()
{
    int x = -1280*(AppEnv::currentPage + 1 );

    if(m_gridView->x() > x )
    {
        m_gridView->move(m_gridView->x()-50,0);

    }else{
        m_gridView->move( x, 0);
        AppEnv::currentPage++;
        m_nextPageTimer->stop();
    }

}


void TouchDemo::returnCurrentPage()
{
    int x = 1280*(AppEnv::currentPage + 1 );

    qDebug() << m_gridView->x()  <<  x;
    if (m_gridView->x() != x)
    {
        m_gridView->move(m_gridView->x() - m_movingDistance, 0);

    }else{
        m_gridView->move(x, 0);
        m_currentPageTimer->stop();
    }



    return;


}

void TouchDemo::touchBegin(QEvent * event)
{
    QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
    m_touchPoints = touchEvent->touchPoints();
    if (m_touchPoints.count() == 2)
    {
        const QTouchEvent::TouchPoint &touchPoint0 = m_touchPoints.first();
       // const QTouchEvent::TouchPoint &touchPoint1 = m_touchPoints.last();

        qDebug() <<"2 touch press";
        qDebug() << touchPoint0.pos();
    }


}
bool TouchDemo::event( QEvent * event )
{

   return QWidget::event(event);
}

void TouchDemo::mouseMoveEvent ( QMouseEvent * event )
{
    if (m_mouseOldPosX == 0)
    {
        m_mouseOldPosX = event->x();

    }
    int x = event->x() - m_mouseOldPosX;
   // qDebug() << "event->x()" << event->x() << "m_mouseOldPosX" << m_mouseOldPosX <<"x" << x;

    m_movingDistance += x;
    m_mouseOldPosX = event->x();
    if(m_gridView->isVisible())
    {
        m_gridView->move(m_gridView->x()+x,m_gridView->y());
    }

}

void TouchDemo::mousePressEvent ( QMouseEvent * event )
{


    m_movingDistance = 0;
    m_mouseOldPosX = event->x();

    qDebug() << "press" << m_mouseOldPosX <<"event->x()" << event->x();


}

void TouchDemo::mouseReleaseEvent ( QMouseEvent * event )
{

    qDebug() << "mouseOldPosX" <<m_mouseOldPosX
             <<"movingDistance"<<m_movingDistance
            << "m_gridView" << m_gridView->x();


    pageDirection = 0;
    if (m_movingDistance > 200)
    {
        pageDirection = 1;
    }
    else if (m_movingDistance < (-200))
    {
        pageDirection = -1;
    }

    if(m_gridView->isVisible())
    {
        automaticPage(pageDirection);
    }


    if(m_preview->isVisible())
    {
        automaticImg(pageDirection);
    }
    qDebug() <<"+-" << pageDirection;
    QWidget::mouseReleaseEvent(event);
}

void TouchDemo::keyPressEvent(QKeyEvent * event)
{

	switch (event->key()) {

	case Qt::Key_Up:
		qDebug() << "up";
		m_preview->prevImage();
		break;

	case Qt::Key_Down:
		qDebug() << "down";
		m_preview->nextImage();
		break;

	case Qt::Key_Left:
		qDebug() << "left";
		m_preview->prevImage();
		break;

	case Qt::Key_Right:
		qDebug() << "right";
		m_preview->nextImage();
		break;

	case Qt::Key_Escape:
		qDebug() << "esc";
		emit pressESC();
		break;
    case Qt::Key_PageUp:
        qDebug() << "prev";
        if(!m_prevPageTimer->isActive() && !m_nextPageTimer->isActive())
        {
            previousPage();
        }
        break;

    case Qt::Key_PageDown:
        qDebug() << "down";
        if(!m_prevPageTimer->isActive() && !m_nextPageTimer->isActive())
        {
            nextPage();
        }
        break;
    case Qt::Key_R:
        m_preview->rotate(90);
        break;
	default:
		qDebug() << "unknow";
		break;

	}

}



void TouchDemo::automaticImg(int direction)
{
    qDebug()<<"direction"<<direction;
    switch(direction)
    {
    case 1:
        m_preview->prevImage();
        break;
    case -1:
        m_preview->nextImage();
        break;
    case 0:
        break;
    default:
        break;

    }

}



void TouchDemo::automaticPage(int direction)
{
    qDebug()<<"direction"<<direction;
    switch(direction)
    {
    case 1:
        previousPage();
        break;
    case -1:
        nextPage();
        break;
    case 0:
        returnCurrentPage();
        break;
    default:
        break;

    }

}
