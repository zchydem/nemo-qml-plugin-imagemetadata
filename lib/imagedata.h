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

#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <QObject>
#include <QQmlParserStatus>
#include <QUrl>

#include <exiv2/exiv2.hpp>
#include "exifdata.h"

class ImageDataPrivate;
class Q_DECL_EXPORT ImageData: public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(ByteOrder byteOrder READ byteOrder WRITE setByteOrder NOTIFY byteOrderChanged)

    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)
    Q_PROPERTY(QString mimeType READ mimeType NOTIFY mimeTypeChanged)
    Q_PROPERTY(QString comment READ comment NOTIFY commentChanged)

    Q_PROPERTY(int width READ width NOTIFY widthChanged)
    Q_PROPERTY(int height READ height NOTIFY heightChanged)

    Q_PROPERTY(ExifData* exifData READ exifData WRITE setExifData NOTIFY exifDataChanged)

    Q_PROPERTY(bool supportsExif READ supportsExif())


    Q_ENUMS(Status)
    Q_ENUMS(ByteOrder)
    Q_ENUMS(AccessMode)

public:
    enum Status {
        Null,
        Loading,
        Ready,
        Error
    };

    enum ByteOrder {
        InvalidByteOrder= Exiv2::invalidByteOrder,
        LittleEndian    = Exiv2::littleEndian,
        BigEndian       = Exiv2::bigEndian
    };

    enum AccessMode {
        None            = Exiv2::amNone,
        ReadOnly        = Exiv2::amRead,
        WriteOnly       = Exiv2::amWrite,
        ReadAndWrite    = Exiv2::amReadWrite
    };

    ImageData(QObject * parent = 0);
    virtual ~ImageData();

    void classBegin();
    void componentComplete();

    QUrl source() const;
    void setSource(const QUrl &url);

    Status status() const;

    ByteOrder byteOrder() const;
    void setByteOrder(const ByteOrder &order);

    bool valid() const;
    QString mimeType() const;
    QString comment() const;

    int width() const;
    int height() const;

    ExifData * exifData();
    void setExifData(ExifData *exifData);

    Q_INVOKABLE bool supportsExif() const;
    Q_INVOKABLE bool supportsIptc() const;
    Q_INVOKABLE bool supportsXmp() const;
    Q_INVOKABLE bool supportsComment() const;

    Q_INVOKABLE AccessMode exifAccess() const;
    Q_INVOKABLE AccessMode iptcAccess() const;
    Q_INVOKABLE AccessMode xmpAccess() const;
    Q_INVOKABLE AccessMode commentAccess() const;

    Q_INVOKABLE void clearExifData(bool flush = true);
    Q_INVOKABLE void clearXmpData(bool flush = true);
    Q_INVOKABLE void clearIptcData(bool flush = true);
    Q_INVOKABLE void clearComment(bool flush = true);
    Q_INVOKABLE void clearMetadata(bool flush = true);

    Q_INVOKABLE bool copyMetadataTo(const QString &targetFilePath, bool create = true);


Q_SIGNALS:
    // TODO: Check if all the are really needed
    void sourceChanged();
    void statusChanged();
    void byteOrderChanged();
    void validChanged();
    void mimeTypeChanged();
    void commentChanged();
    void widthChanged();
    void heightChanged();
    void exifDataChanged();

private:
    ImageDataPrivate *d_ptr;
    Q_DECLARE_PRIVATE(ImageData)
};

#endif
