#ifndef APPENV_H
#define APPENV_H

#include "common.h"

class AppEnv
{
public:
    AppEnv();
    static QStringList imgList;
    static QStringList initImgList();

    static QString currentImg;
    static QString imgPath;
    static int currentImageIndex;
    static int imgCount;
    static int currentPage;
    static int pageCount;
};

#endif // APPENV_H
