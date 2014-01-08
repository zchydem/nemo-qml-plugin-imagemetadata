/****************************************************************************************
**
** Copyright (C) 2014 Jolla Ltd.
** Contact: Marko Mattila <marko.mattila@jolla.com>
** All rights reserved.
**
** This file is part of Nemo Image Metadata package.
**
** You may use this file under the terms of the GNU Lesser General
** Public License version 2.1 as published by the Free Software Foundation
** and appearing in the file license.lgpl included in the packaging
** of this file.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file license.lgpl included in the packaging
** of this file.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
**
****************************************************************************************/

#include <QTest>
#include <QtDebug>
#include "ut_imagedata.h"
#include "ut_exifdata.h"
#include <exiv2/exiv2.hpp>

int main(int argc, char *argv[])
{
    int res = 0;
    try {
        ut_imagedata t1;
        res += QTest::qExec(&t1, argc, argv);

        ut_exifdata t2;
        res += QTest::qExec(&t2, argc, argv);

    }catch(Exiv2::AnyError& e) {
        qWarning() << "Catched exceptions from libexiv2." << e.what();
    }

    return res;
}
