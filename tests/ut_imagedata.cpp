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

#include "ut_imagedata.h"
#include "imagedata.h"
#include "exifdata.h"
#include <QSignalSpy>


ut_imagedata::ut_imagedata(QObject *parent)
    : QObject(parent)
    , m_imageData(0)
    , m_copiedImagePath("/tmp/copied_image.jpg")
{
}


void ut_imagedata::init()
{
    QVERIFY(m_imageData == 0);
    m_imageData = new ImageData();
    m_imageData->setSource(QUrl::fromLocalFile("images/testimage-with-location.jpg"));
    m_imageData->componentComplete();
    QCOMPARE(m_imageData->status(), ImageData::Ready);
    QCOMPARE(m_imageData->valid(), true);
}

void ut_imagedata::cleanup()
{
    delete m_imageData;
    m_imageData = 0;

    if (QFile::exists(m_copiedImagePath)) {
        QFile::remove(m_copiedImagePath);
    }
}

void ut_imagedata::testImageStatus()
{
    // First get rid of the default initialization
    QVERIFY(m_imageData);
    delete m_imageData;

    m_imageData = new ImageData();
    QSignalSpy spy(m_imageData, SIGNAL(statusChanged()));

    // Test with non-existent image first
    m_imageData->setSource(QUrl::fromLocalFile("images/xxxx.jpg"));

    QCOMPARE(spy.count(), 0);
    QCOMPARE(m_imageData->status(), ImageData::Null);
    m_imageData->componentComplete();

    QCOMPARE(spy.count(), 2);
    QCOMPARE(m_imageData->status(), ImageData::Error);
    QCOMPARE(m_imageData->valid(), false);
    spy.clear();

    m_imageData->setSource(QUrl::fromLocalFile("images/testimage-with-location.jpg"));
    QCOMPARE(spy.count(), 2);
    QCOMPARE(m_imageData->status(), ImageData::Ready);
    QCOMPARE(m_imageData->valid(), true);
}

void ut_imagedata::testByteOrder()
{

}

void ut_imagedata::testMimeType()
{
    QVERIFY(m_imageData);
    QVERIFY(m_imageData->mimeType() == "image/jpeg");

    m_imageData->setSource(QUrl::fromLocalFile("images/test-image.png"));
    QCOMPARE(m_imageData->status(), ImageData::Ready);
    QVERIFY(m_imageData->mimeType() == "image/png");
}

void ut_imagedata::testSize()
{
    QVERIFY(m_imageData);
    QCOMPARE(m_imageData->valid(), true);

    // 2000x3552 but rotated testimage-with-location.jpg
    QCOMPARE(m_imageData->width(), 3552);
    QCOMPARE(m_imageData->height(), 2000);

    //2688 x 1520 // android-test-image.jpg
    m_imageData->setSource(QUrl::fromLocalFile("images/android-test-image.jpg"));
    QCOMPARE(m_imageData->status(), ImageData::Ready);
    QCOMPARE(m_imageData->width(), 2688);
    QCOMPARE(m_imageData->height(), 1520);

    // 1366 x 768 test-image.png
    m_imageData->setSource(QUrl::fromLocalFile("images/test-image.png"));
    QCOMPARE(m_imageData->status(), ImageData::Ready);
    QCOMPARE(m_imageData->width(), 1366);
    QCOMPARE(m_imageData->height(), 768);
}

void ut_imagedata::testAccess()
{
    QVERIFY(m_imageData);
    QCOMPARE(m_imageData->valid(), true);

    QCOMPARE(m_imageData->exifAccess(), ImageData::ReadAndWrite);
    QCOMPARE(m_imageData->iptcAccess(), ImageData::ReadAndWrite);
    QCOMPARE(m_imageData->xmpAccess(), ImageData::ReadAndWrite);
    QCOMPARE(m_imageData->commentAccess(), ImageData::ReadAndWrite);

    QCOMPARE(m_imageData->supportsExif(), true);
    QCOMPARE(m_imageData->supportsIptc(), true);
    QCOMPARE(m_imageData->supportsXmp(), true);
    QCOMPARE(m_imageData->supportsComment(), true);

    //Test bmp image next, which has restricted access
    m_imageData->setSource(QUrl::fromLocalFile("images/test-image.bmp"));
    QCOMPARE(m_imageData->valid(), true);
    QCOMPARE(m_imageData->exifAccess(), ImageData::None);
    QCOMPARE(m_imageData->iptcAccess(), ImageData::None);
    QCOMPARE(m_imageData->xmpAccess(), ImageData::None);
    QCOMPARE(m_imageData->commentAccess(), ImageData::None);

    QCOMPARE(m_imageData->supportsExif(), false);
    QCOMPARE(m_imageData->supportsIptc(), false);
    QCOMPARE(m_imageData->supportsXmp(), false);
    QCOMPARE(m_imageData->supportsComment(), false);

    // TOOD: Add more images for testing e.g TIFF
}

void ut_imagedata::testCopyAndClear()
{
    QCOMPARE(m_imageData->copyMetadataTo(m_copiedImagePath), true);
    QCOMPARE(QFile::exists(m_copiedImagePath), true);

    ImageData ci;
    ci.setSource(QUrl::fromLocalFile(m_copiedImagePath));
    ci.componentComplete();
    QCOMPARE(ci.valid(), true);
    QCOMPARE(ci.status(), ImageData::Ready);

    ExifData *oExif = m_imageData->exifData();
    ExifData *cExif = ci.exifData();
    QCOMPARE(oExif->rowCount(), cExif->rowCount());
    ci.clearExifData();
    cExif = ci.exifData();
    QCOMPARE(cExif->rowCount(), 0);

    // TODO Add test to iptc and xmp data too
}

