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

#include "imagedata.h"
#include "exifdata_p.h"
#include <QtDebug>
#include <QFile>
#include <QDir>

class ImageDataPrivate {
public:
    ImageDataPrivate(ImageData *parent)
        : q_ptr(parent)
        , source()
        , status(ImageData::Null)
        , image()
        , initDone(false)
    {

    }

    void reload()
    {
        Q_Q(ImageData);
        if (status != ImageData::Loading) {
            status = ImageData::Loading;
            emit q->statusChanged();
        }

        if (!source.isValid() || !QFile::exists(source.toLocalFile())) {
            status = ImageData::Error;
            emit q->statusChanged();
            qWarning() << Q_FUNC_INFO << "Invalid image source: "
                       << source
                       << source.toLocalFile()
                       << source.isValid();
            return;
        }

        image = Exiv2::ImageFactory::open(source.toLocalFile().toStdString());
        image->readMetadata();

        if (image->good()) {
            status = ImageData::Ready;
            emit q->statusChanged();
        }
    }

    ImageData *q_ptr;
    Q_DECLARE_PUBLIC(ImageData)

    QUrl source;
    ImageData::Status status;
    Exiv2::Image::AutoPtr image;
    bool initDone;
};


ImageData::ImageData(QObject * parent)
    : QObject(parent)
    , d_ptr(new ImageDataPrivate(this))
{

}

ImageData::~ImageData()
{
    delete d_ptr;
}

void ImageData::classBegin()
{

}

void ImageData::componentComplete()
{
    Q_D(ImageData);
    d->initDone = true;
    d->reload();

}

QUrl ImageData::source() const
{
    Q_D(const ImageData);
    return d->source;
}

void ImageData::setSource(const QUrl &url)
{
    Q_D(ImageData);
    if (url != d->source && url.isValid()) {
        d->source = url;

        if (d->source.scheme().isEmpty()) {
            d->source.setScheme(QStringLiteral("file"));
        }

        emit sourceChanged();
        if (d->initDone) {
            d->reload();
        }
    } else {
        qWarning() << Q_FUNC_INFO << "Invalid source: " << url;
    }
}

ImageData::Status ImageData::status() const
{
    Q_D(const ImageData);
    return d->status;
}

ImageData::ByteOrder ImageData::byteOrder() const
{
    Q_D(const ImageData);
    return static_cast<ImageData::ByteOrder>(d->image->byteOrder());
}

void ImageData::setByteOrder(const ImageData::ByteOrder &order)
{
    Q_D(ImageData);
    d->image->setByteOrder(static_cast<Exiv2::ByteOrder>(order));
}

bool ImageData::valid() const
{
    Q_D(const ImageData);
    return (d->status == ImageData::Ready && d->image->good());
}

QString ImageData::mimeType() const
{
    Q_D(const ImageData);
    return QString::fromStdString(d->image->mimeType());
}

QString ImageData::comment() const
{
    Q_D(const ImageData);
    return QString::fromStdString(d->image->comment());
}

int ImageData::width() const
{
    Q_D(const ImageData);
    return d->image->pixelWidth();
}

int ImageData::height() const
{
    Q_D(const ImageData);
    return d->image->pixelHeight();
}

ExifData * ImageData::exifData()
{
    Q_D(const ImageData);
    return d->status == ImageData::Ready
            ? new ExifData(d->image->exifData(), this)
            : 0;
}

void ImageData::setExifData(ExifData *exifData)
{
    Q_D(ImageData);
    if (exifData == 0) {
        qWarning() << Q_FUNC_INFO << "Null ExifData object";
        return;
    }
    if (d->status == ImageData::Ready) {
        d->image->setExifData(exifData->d_func()->exif);
    } else {
        qWarning() << Q_FUNC_INFO << "Can't set exif data while status is not Ready";
    }
}


bool ImageData::supportsExif() const
{
    return exifAccess() == ImageData::ReadAndWrite;
}

bool ImageData::supportsIptc() const
{
    return iptcAccess() == ImageData::ReadAndWrite;
}

bool ImageData::supportsXmp() const
{
    return xmpAccess() == ImageData::ReadAndWrite;
}

bool ImageData::supportsComment() const
{
    return commentAccess() == ImageData::ReadAndWrite;
}

ImageData::AccessMode ImageData::exifAccess() const
{
    Q_D(const ImageData);
    return static_cast<ImageData::AccessMode>(d->image->checkMode(Exiv2:: mdExif));
}

ImageData::AccessMode ImageData::iptcAccess() const
{
    Q_D(const ImageData);
    return static_cast<ImageData::AccessMode>(d->image->checkMode(Exiv2:: mdIptc));
}

ImageData::AccessMode ImageData::xmpAccess() const
{
    Q_D(const ImageData);
    return static_cast<ImageData::AccessMode>(d->image->checkMode(Exiv2:: mdXmp));
}

ImageData::AccessMode ImageData::commentAccess() const
{
    Q_D(const ImageData);
    return static_cast<ImageData::AccessMode>(d->image->checkMode(Exiv2:: mdComment));
}

void ImageData::clearExifData(bool flush)
{
    Q_D(ImageData);
    d->image->clearExifData();
    if (flush) {
        d->image->writeMetadata();
    }
}

void ImageData::clearXmpData(bool flush)
{
    Q_D(ImageData);
    d->image->clearXmpData();
    if (flush) {
        d->image->writeMetadata();
    }
}

void ImageData::clearIptcData(bool flush)
{
    Q_D(ImageData);
    d->image->clearIptcData();
    if (flush) {
        d->image->writeMetadata();
    }
}

void ImageData::clearComment(bool flush)
{
    Q_D(ImageData);
    d->image->clearComment();
    if (flush) {
        d->image->writeMetadata();
    }
}

void ImageData::clearMetadata(bool flush)
{
    Q_D(ImageData);
    d->image->clearMetadata();
    if (flush) {
        d->image->writeMetadata();
    }
}

/*!
    Copy metadata from this object to the target file defined by \a targetFilePath.
    By default if the \a targetFilePath doesn't exist, this method will create a new
    file. Argument \a create is set true by default.

    NOTE: Application must have write access to the target location, or otherwise
          libexiv2 throws an exception.

    Returns true on success false on failure..
*/
bool ImageData::copyMetadataTo(const QString &targetFilePath, bool create)
{
    Q_D(ImageData);
    QString basePath = targetFilePath.left(targetFilePath.lastIndexOf("/"));

    if (basePath.size() <= 0) {
        qWarning() << Q_FUNC_INFO << "Invalid target file path!";
        return false;
    }

    QDir path;
    if (!path.exists(basePath) && create) {
        if (!path.mkpath(basePath)) {
            qWarning() << Q_FUNC_INFO << "Failed to create path!";
            return false;
        }
    }

    const int type = Exiv2::ImageFactory::getType(d->source.toLocalFile().toStdString());
    if (type == 0) {
        qWarning() << Q_FUNC_INFO << "Couldn't detect image type info."
                   << d->source.toLocalFile();
        return false;
    }

    // Create a new file or overwrite the existing one
    Exiv2::Image::AutoPtr output = Exiv2::ImageFactory::create(type, targetFilePath.toStdString());
    output->setMetadata(*d->image.get());
    output->writeMetadata();

    // TODO We could check that metadata is really copied. libexiv2 only throws
    // exceptions on errors so tracking those is bit difficult in Qt.

    return QFile::exists(targetFilePath);
}

void ImageData::save()
{
    Q_D(ImageData);
    d->image->writeMetadata();
}
