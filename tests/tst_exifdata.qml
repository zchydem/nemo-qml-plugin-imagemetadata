import QtTest 1.0
import QtQuick 2.0
import org.nemomobile.imagemetadata 1.0
import "scripts/Util.js" as Util

TestCase {
    name: "tst_exifdata"

    ImageData {
        id: imageData
        source: "images/testimage-with-location.jpg"
    }

    ListView {
        id: exifList
        objectName: "exifList"
        model: imageData.status == ImageData.Ready
               ? imageData.exifData
               : null
        delegate: Item {
            property string _key: key
            property string _familyName: familyName
            property string _tagName

            Component.onCompleted: console.log("Exif item: ", index, key)
        }
    }

    function test_dah()
    {
        compare(exifList.count, 41)

        var item = Util.findItem(exifList.contentItem, function(item){
            return item._key == "Exif.Image.Make"
        })
        verify(item)
    }

    /*
    function test_initial_status()
    {
        return

        compare(imageData.status, ImageData.Ready)
        compare(imageData.valid, true)
        verify(imageData.mimeType == "image/jpeg")
        verify(imageData.comment == "This is test comment")
        compare(imageData.width, 3552)
        compare(imageData.height, 2000)
        compare(imageData.byteOrder, ImageData.LittleEndian)
        compare(imageData.exifAccess(), ImageData.ReadAndWrite)
    }

    function test_metadata_removal()
    {
        return
        var copiedFile = "/tmp/copied-test-image.jpg"
        compare(imageData.copyMetadataTo(copiedFile), true)
        copiedData.source = copiedFile;
        compare(copiedData.status, ImageData.Ready)


        compare(copiedData.exifData.count, 41)
        copiedData.clearExifData(true)
        compare(copiedData.exifData.count, 0)

        // TODO Add xmp and iptc too
    }
    */
}
