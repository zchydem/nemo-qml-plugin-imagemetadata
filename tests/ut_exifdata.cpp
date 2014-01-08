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

#include "ut_exifdata.h"
#include "imagedata.h"
#include "exifdata.h"

#include <QtDebug>
#include <QVariant>

ut_exifdata::ut_exifdata(QObject * parent)
    : QObject(parent)
    , m_imageData(0)
    , m_exif(0)
{
    qDebug() << Q_FUNC_INFO;
}

void ut_exifdata::init()
{
    qDebug() << Q_FUNC_INFO;
    QVERIFY(m_exif == 0);
    m_imageData = new ImageData;
    m_imageData->setSource(QUrl::fromLocalFile("images/android-test-image.jpg"));
    m_imageData->componentComplete();
    QCOMPARE(m_imageData->status(), ImageData::Ready);
    QCOMPARE(m_imageData->valid(), true);

    m_exif = m_imageData->exifData();
    QVERIFY(m_exif);
}

void ut_exifdata::cleanup()
{
    qDebug() << Q_FUNC_INFO;

    if (m_exif) {
        delete m_exif;
        m_exif = 0;
    }
    if (m_imageData) {
        delete m_imageData;
        m_imageData = 0;
    }
}


void ut_exifdata::testMetadataCount()
{
    QVERIFY(m_exif);
    QCOMPARE(m_exif->rowCount(), 64);

    // Test couple of random keys from the exif
    QCOMPARE(m_exif->tags().contains("Exif.Image.Make"), true);
    QCOMPARE(m_exif->tags().contains("Exif.Thumbnail.ImageWidth"), true);
    QCOMPARE(m_exif->tags().contains("Exif.Photo.Saturation"), true);
}

void ut_exifdata::testIndex()
{
    QVERIFY(m_exif);
    // Test with bad input
    QModelIndex index = m_exif->index(0, 1, QModelIndex());
    QCOMPARE(index.isValid(), false);
    index = m_exif->index(100, 0, QModelIndex());
    QCOMPARE(index.isValid(), false);
    index = m_exif->index(-100, 0, QModelIndex());
    QCOMPARE(index.isValid(), false);
    index = m_exif->index(1, -1, QModelIndex());
    QCOMPARE(index.isValid(), false);

    // Test with good input
    index = m_exif->index(0, 0, QModelIndex());
    QCOMPARE(index.isValid(), true);

}

void ut_exifdata::testModelData_data()
{
    // Create test data with row, column, role and expected result
    QTest::addColumn<int>("row");
    QTest::addColumn<int>("column");
    QTest::addColumn<int>("role");
    QTest::addColumn<QVariant>("expected");

    // These values are from the actual file. See images/android-test-image.jpg
    QTest::newRow("Key")                << 0    << 0 << static_cast<int>(ExifData::Key)             <<  QVariant("Exif.Image.Make");
    QTest::newRow("Key")                << 63   << 0 << static_cast<int>(ExifData::Key)             <<  QVariant("Exif.Thumbnail.JPEGInterchangeFormatLength");
    QTest::newRow("FamilyName")         << 2    << 0 << static_cast<int>(ExifData::FamilyName)      <<  QVariant("Exif");
    QTest::newRow("GroupName")          << 2    << 0 << static_cast<int>(ExifData::GroupName)       <<  QVariant("Image");
    QTest::newRow("GroupName")          << 11   << 0 << static_cast<int>(ExifData::GroupName)       <<  QVariant("Photo");
    QTest::newRow("TagName")            << 12   << 0 << static_cast<int>(ExifData::TagName)         <<  QVariant("ISOSpeedRatings");
    QTest::newRow("TagLabel")           << 18   << 0 << static_cast<int>(ExifData::TagLabel)        <<  QVariant("Shutter speed");
    QTest::newRow("Tag")                << 13   << 0 << static_cast<int>(ExifData::Tag)             <<  QVariant(36864);
    QTest::newRow("Ifd")                << 55   << 0 << static_cast<int>(ExifData::Ifd)             <<  QVariant(6);
    QTest::newRow("IfdName")            << 55   << 0 << static_cast<int>(ExifData::IfdName)         <<  QVariant("GPSInfo");
    QTest::newRow("TypeName")           << 57   << 0 << static_cast<int>(ExifData::TypeName)        <<  QVariant("Long");
    QTest::newRow("TypeName")           << 60   << 0 << static_cast<int>(ExifData::TypeName)        <<  QVariant("Rational");
    QTest::newRow("TypeName")           << 12   << 0 << static_cast<int>(ExifData::TypeName)        <<  QVariant("Short");
    QTest::newRow("Value")              << 12   << 0 << static_cast<int>(ExifData::Value)           <<  QVariant("800");
    QTest::newRow("Value")              << 0    << 0 << static_cast<int>(ExifData::Value)           <<  QVariant("HTC");
    QTest::newRow("RationalValue ")     << 60   << 0 << static_cast<int>(ExifData::Value)           <<  QVariant("72/1");
    QTest::newRow("RationalC1Value ")   << 60   << 0 << static_cast<int>(ExifData::RationalC1Value) <<  QVariant(72);
    QTest::newRow("RationalC2Value ")   << 60   << 0 << static_cast<int>(ExifData::RationalC2Value) <<  QVariant(1);
    QTest::newRow("LongValue")          << 62   << 0 << static_cast<int>(ExifData::LongValue)       <<  QVariant(1798);
    QTest::newRow("RationalC1Value")    << 26   << 0 << static_cast<int>(ExifData::RationalC1Value) <<  QVariant(382);
    QTest::newRow("RationalC2Value")    << 26   << 0 << static_cast<int>(ExifData::RationalC2Value) <<  QVariant(100);
    QTest::newRow("LongValue")          << 12   << 0 << static_cast<int>(ExifData::LongValue)       <<  QVariant(800);
}

void ut_exifdata::testModelData()
{
    QFETCH(int, row);
    QFETCH(int, column);
    QFETCH(int, role);
    QFETCH(QVariant, expected);

    const QModelIndex index = m_exif->index(row, column, QModelIndex());
    const QVariant res = m_exif->data(index, role);
    QCOMPARE(res, expected);
}

void ut_exifdata::testModifyMetadata()
{

}


