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

#include <QQmlExtensionPlugin>
#include <QQmlEngine>
#include <QtQml>

#include "imagedata.h"
#include "exifdata.h"
#include "xmpdata.h"
#include "iptcdata.h"

class SailfishAccountsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.nemomobile.imagemetadata")

public:

    virtual void registerTypes(const char *uri)
    {
        Q_UNUSED(uri)
        Q_ASSERT(QLatin1String(uri) == QLatin1String("org.nemomobile.imagemetadata"));

        qmlRegisterType<ImageData>("org.nemomobile.imagemetadata", 1, 0, "ImageData");
        qmlRegisterType<ExifData>("org.nemomobile.imagemetadata", 1, 0, "ExifData");
        qmlRegisterType<XmpData>("org.nemomobile.imagemetadata", 1, 0, "XmpData");
        qmlRegisterType<IptcData>("org.nemomobile.imagemetadata", 1, 0, "IptcData");
    }
};

#include "plugin.moc"
