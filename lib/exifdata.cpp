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

#include "exifdata.h"

#include <QtDebug>


class ExifDataPrivate
{
public:

    QVariant valueToVariant(Exiv2::Value::AutoPtr value, int role) const
    {
        if (value->count() == 0) {
            return QVariant();
        }
        else
        if (role == ExifData::Value) {
            return QString::fromStdString(value->toString());
        }
        else
        if (role == ExifData::LongValue && value->count() == 1) {
            return QVariant::fromValue(value->toLong(0));
        }
        else
        if (role == ExifData::FloatValue && value->count() == 1) {
            return value->toFloat(0);
        }
        else
        if (role == ExifData::RationalC1Value && value->count() == 1) {
            return static_cast<std::pair<uint32_t, uint32_t> >(value->toRational(0)).first;
        }
        else
        if (role == ExifData::RationalC2Value && value->count() == 1) {
            return static_cast<std::pair<uint32_t, uint32_t> >(value->toRational(0)).second;
        }

        qWarning() << Q_FUNC_INFO << "Failed to convert value to QVariant"
                   << QString::fromStdString(value->toString());
        return QVariant();
    }

    Exiv2::ExifData exif;
    QStringList keys;
};

ExifData::ExifData(QObject * parent)
    : QAbstractListModel(parent)
    , d_ptr(new ExifDataPrivate)
{
}

ExifData::ExifData(Exiv2::ExifData &exifData, QObject *parent)
    : QAbstractListModel(parent)
    , d_ptr(new ExifDataPrivate)
{
    Q_D(ExifData);
    d->exif = exifData;
    if (d->exif.empty()) {
        return;
    }
    int index = 0;
    Exiv2::ExifData::const_iterator end = d->exif.end();
    for (Exiv2::ExifData::const_iterator i = d->exif.begin(); i != end; ++i) {

        QString key = QString::fromStdString(i->key());
        qDebug() << index++ << "Key: " << key
                 << QString::fromStdString(i->tagName())
                 << QString::fromStdString(i->familyName())
                 << QString::fromStdString(i->tagLabel())
                 << QString::fromStdString(i->groupName())
                 << i->tag()
                 << QString::fromStdString(i->tagName())
                 << QString::fromStdString(i->ifdName())
                 << QString::fromStdString(i->typeName());

        d->keys  << QString::fromStdString(i->key());
    }

    if (d->exif.count() > 0) {
        emit countChanged();
    }
}

ExifData::~ExifData()
{
    delete d_ptr;
    d_ptr = 0;
}

void ExifData::classBegin()
{

}

void ExifData::componentComplete()
{
    // TODO: Check this
    Q_D(ExifData);
}

QModelIndex ExifData::index(int row, int column, const QModelIndex &parent) const
{
    Q_D(const ExifData);
    return row < d->exif.count() && column == 0
            ? createIndex(row, 0)
            : QModelIndex();
}

int ExifData::rowCount(const QModelIndex & parent) const
{
    Q_D(const ExifData);
    return !parent.isValid()
            ? d->exif.count()
            : 0;
}

QVariant ExifData::data(const QModelIndex & index, int role) const
{
    Q_D(const ExifData);
    if (!index.isValid() || index.row() >= d->exif.count() || index.column() != 0) {
        qWarning() << Q_FUNC_INFO << "Invalid index!";
        return QVariant();
    }

    const QString key = d->keys.value(index.row());
    Exiv2::ExifKey exifKey(key.toStdString());
    Exiv2::ExifData::const_iterator entry = d->exif.findKey(exifKey);
    if (entry == d->exif.end()) {
        return QVariant();
    }

    switch (role) {
    case ExifData::Key:
        return key;
    case ExifData::FamilyName:
        return entry->familyName();
    case ExifData::GroupName:
        return QString::fromStdString(entry->groupName());
    case ExifData::TagName:
        return QString::fromStdString(entry->tagName());
    case ExifData::TagLabel:
        return QString::fromStdString(entry->tagLabel());
    case ExifData::IfdName:
        return entry->ifdName();
    case ExifData::Ifd:
        return entry->ifdId();
    case ExifData::Tag:
        return entry->tag();
    case ExifData::TypeName:
        return entry->typeName();
    case ExifData::Value:
    case ExifData::LongValue:
    case ExifData::FloatValue:
    case ExifData::RationalC1Value:
    case ExifData::RationalC2Value:
        return d->valueToVariant(entry->getValue(), role);
    default:
        qWarning() << Q_FUNC_INFO << "Unknown role!";
        return QVariant();
    }
    return QVariant();
}

QStringList ExifData::tags() const
{
    Q_D(const ExifData);
    return d->keys;
}

void ExifData::setTags(const QStringList &tags)
{

}


QVariant ExifData::tag(const QString &key)
{
    return QVariant();
}

void ExifData::setTag(const QString &key, const QVariant &value)
{

}

QHash<int, QByteArray> ExifData::roleNames() const
{
    Q_D(const ExifData);
    QHash<int, QByteArray> roles;
    int roleIndex = ExifData::Key;
    roles[roleIndex]   = "key";
    roles[++roleIndex] = "familyName";
    roles[++roleIndex] = "groupName";
    roles[++roleIndex] = "tagName";
    roles[++roleIndex] = "tagLabel";
    roles[++roleIndex] = "tag";
    roles[++roleIndex] = "ifdName";
    roles[++roleIndex] = "ifd";
    roles[++roleIndex] = "typeName";
    roles[++roleIndex] = "value";
    roles[++roleIndex] = "longValue";
    roles[++roleIndex] = "floatValue";
    roles[++roleIndex] = "rationalC1Value";
    roles[++roleIndex] = "rationalC2value";
    return roles;
}
