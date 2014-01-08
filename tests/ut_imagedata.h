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

#ifndef UT_IMAGEDATA_H
#define UT_IMAGEDATA_H

#include <QObject>
#include <QString>

class ImageData;
class ut_imagedata : public QObject
{
    Q_OBJECT
public:
    ut_imagedata(QObject *parent = 0);

private slots:
    void init();
    void cleanup();
    void testImageStatus();
    void testByteOrder();
    void testMimeType();
    void testSize();
    void testAccess();
    void testCopyAndClear();

private:
    ImageData *m_imageData;
    QString m_copiedImagePath;
};
#endif
