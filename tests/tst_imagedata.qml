import QtTest 1.0
import QtQuick 2.0
import org.nemomobile.imagemetadata 1.0

TestCase {
    name: "tst_imagedata"

    ImageData {
        id: imageData
        source: "images/testimage-with-location.jpg"
    }

    ImageData {
        id: copiedData
    }

    function test_initial_status()
    {
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
        var copiedFile = "/tmp/copied-test-image.jpg"
        compare(imageData.copyMetadataTo(copiedFile), true)
        copiedData.source = copiedFile;
        compare(copiedData.status, ImageData.Ready)


        compare(copiedData.exifData.count, 41)
        copiedData.clearExifData(true)
        compare(copiedData.exifData.count, 0)

        // TODO Add xmp and iptc too
    }

}
