#ifndef EXIFDATA_P_H
#define EXIFDATA_P_H

#include <QObject>
#include <exiv2/exiv2.hpp>

class ExifData;
class ExifDataPrivate
{
public:
    ExifDataPrivate(ExifData *parent);
    ExifDataPrivate(ExifData *parent, Exiv2::ExifData &exifData);

    void loadKeys();
    QVariant valueToVariant(Exiv2::Value::AutoPtr value, int role) const;


    Exiv2::ExifData exif;
    QStringList keys;
    QString removedKey;

    ExifData *q_ptr;
    Q_DECLARE_PUBLIC(ExifData)
};


#endif // EXIFDATA_P_H
