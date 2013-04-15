#include "appenv.h"

QString AppEnv::imgPath = "./deepin-wallpapers/";
QString currentImg = "";
int AppEnv::currentImageIndex = 0;
int AppEnv::imgCount = 0;
int AppEnv::currentPage = 0;
int AppEnv::pageCount = 0;
QStringList AppEnv::imgList;

AppEnv::AppEnv()
{

}

QStringList AppEnv::initImgList()
{

	QDir pdir;
	pdir.setPath(imgPath);
	imgList = pdir.entryList(QDir::Files);
	AppEnv::imgCount = pdir.entryList(QDir::Files).size();
    AppEnv::pageCount = qCeil(AppEnv::imgCount / 12);
	return pdir.entryList(QDir::Files);

}
