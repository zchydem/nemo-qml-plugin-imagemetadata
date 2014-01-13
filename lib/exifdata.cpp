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
#include "exifdata_p.h"
#include <QtDebug>

ExifDataPrivate::ExifDataPrivate(ExifData *parent)
    : q_ptr(parent)
{

}

ExifDataPrivate::ExifDataPrivate(ExifData *parent, Exiv2::ExifData &exifData)
    : exif(exifData)
    , q_ptr(parent)
{
    loadKeys();
}

void ExifDataPrivate::loadKeys()
{
    Q_Q(ExifData);
    const int oldCount = keys.count();
    if (exif.empty()) {
        return;
    }

    QStringList tmpKeys;
    Exiv2::ExifData::const_iterator end = exif.end();
    for (Exiv2::ExifData::const_iterator i = exif.begin(); i != end; ++i) {
        tmpKeys  << QString::fromStdString(i->key());
    }

    keys = tmpKeys;
    if (oldCount != keys.count()) {
        emit q->countChanged();
    }
}

// This is ugly as hell, but the idea is that value can have more
// than 1 component. If there's one component QVariant(value) is
// returned. If there are more componets or the value type is rational
// then this returns QList<QVariant> list, which contains all the
// components or rational number.
QVariant ExifDataPrivate::valueToVariant(Exiv2::Value::AutoPtr value, int role) const
{
    const int count = value->count();
    QList <QVariant> values;

    if (count == 0) {
        return QVariant();
    }
    // NOTE: For some reason count can be something like 90 for string.
    // Therefore need to handle it differently.
    if(role == ExifData::Value) {
        return  QString::fromStdString(value->toString());
    }

    for (int i=0; i < count; ++i) {
        switch(role) {
        case ExifData::LongValue:
            values << QVariant::fromValue(value->toLong(i));
            break;
        case ExifData::FloatValue:
            values << value->toFloat(i);
            break;
        case ExifData::RationalValue:
            values << QVariant(value->toRational(i).first);
            values << QVariant(value->toRational(i).second);
            break;
        default:
            qWarning() << Q_FUNC_INFO << "Unknown value!";
            break;
        }
    }

    return values.count() == 1
            ? values.at(0)
            : values;
}



ExifData::ExifData(QObject * parent)
    : QAbstractListModel(parent)
    , d_ptr(new ExifDataPrivate(this))
{
}

ExifData::ExifData(Exiv2::ExifData &exifData, QObject *parent)
    : QAbstractListModel(parent)
    , d_ptr(new ExifDataPrivate(this, exifData))
{

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
    case ExifData::RationalValue:
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


QVariant ExifData::value(const QString &key)
{
    Q_D(const ExifData);
    if (key.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "Empty key!";
        return QVariant();
    }

    Exiv2::ExifKey exifKey(key.toStdString());
    Exiv2::ExifData::const_iterator entry = d->exif.findKey(exifKey);
    if (entry == d->exif.end()) {
        return QVariant();
    }

    // Always return value as string
    Exiv2::Value::AutoPtr value = entry->getValue();
    return QString::fromStdString(value->toString());
}

void ExifData::setValue(const QString &key, const QVariant &value)
{
    /*
    Q_D(ExifData);
    if (key.isEmpty() || !value.isValid()) {
        qWarning() << Q_FUNC_INFO << "Invalid key or value!";
        return;
    }

    Exiv2::ExifKey exifKey(key.toStdString());
    Exiv2::ExifData::iterator entry = d->exif.findKey(exifKey);
    if (entry == d->exif.end()) {
        return;
    }

    // Always return value as string
    Exiv2::Value::AutoPtr value = entry->getValue();
    */
}

bool ExifData::removeValue(const QString &key)
{
    Q_D(ExifData);
    if (key.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "Invalid key or value!";
        return false;
    }

    Exiv2::ExifKey exifKey(key.toStdString());
    Exiv2::ExifData::iterator entry = d->exif.findKey(exifKey);
    if (entry == d->exif.end()) {
        return false;
    }

    const int index = d->keys.indexOf(key);
    beginRemoveRows(QModelIndex(), index, index);
    const int keyCount = d->keys.count();
    d->exif.erase(entry);
    d->loadKeys();
    endRemoveRows();
    if ((keyCount > d->keys.count())) {
        emit countChanged();
        return true;
    } else {
        return  false;
    }
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
    roles[++roleIndex] = "rationalValue";
    return roles;
}
