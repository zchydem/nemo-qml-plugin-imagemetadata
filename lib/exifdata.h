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

#ifndef EXIFDATA_H
#define EXIFDATA_H

#include <QObject>
#include <QQmlParserStatus>
#include <QAbstractListModel>
#include <QStringList>

#include <QQmlListProperty>

#include <exiv2/exiv2.hpp>


class ExifDataPrivate;
class Q_DECL_EXPORT ExifData: public QAbstractListModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(QStringList tags READ tags WRITE setTags NOTIFY tagsChanged)

    Q_ENUMS(Role)
public:

    enum Role {
        Key = 0,
        FamilyName,
        GroupName,
        TagName,
        TagLabel,
        Tag,
        IfdName,
        Ifd,
        TypeName,
        ValueComponentCount,
        Value,
        LongValue,
        FloatValue,
        RationalValue
    };


    ExifData(QObject *parent = 0);
    virtual ~ExifData();

    void classBegin();
    void componentComplete();

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

    QStringList tags() const;
    void setTags(const QStringList &tags);

    Q_INVOKABLE QVariant value(const QString &key);
    Q_INVOKABLE void setValue(const QString &key, const QVariant &value);

    Q_INVOKABLE bool removeValue(const QString &key);

Q_SIGNALS:
    void countChanged();
    void tagsChanged();

protected:
    virtual QHash<int, QByteArray> roleNames() const;

private:
    ExifData(Exiv2::ExifData &exifData, QObject * parent = 0);
    ExifDataPrivate *d_ptr;
    Q_DECLARE_PRIVATE(ExifData)


    friend class ImageData;
};

#endif
