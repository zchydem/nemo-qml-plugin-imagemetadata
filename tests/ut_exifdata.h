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

#ifndef UT_EXIFDATA_H
#define UT_EXIFDATA_H

#include <QObject>

class ImageData;
class ExifData;
class ut_exifdata : public QObject
{
    Q_OBJECT
public:
    ut_exifdata(QObject * parent = 0);

private slots:
    void init();
    void cleanup();
    void testMetadataCount();
    void testIndex();
    void testModelData_data();
    void testModelData();
    void testModifyMetadata();

private:
    ImageData *m_imageData;
    ExifData *m_exif;
};
#endif
