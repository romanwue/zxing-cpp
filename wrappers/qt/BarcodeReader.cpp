#include "BarcodeReader.h"
#include "TextUtfEncoding.h"
#include "GenericLuminanceSource.h"
#include "HybridBinarizer.h"
#include "BinaryBitmap.h"
#include "MultiFormatReader.h"
#include "Result.h"
#include "DecodeHints.h"

#include <type_traits>
#include <cassert>
#include <QDebug>

namespace ZXing {

BarcodeReader::BarcodeReader(bool tryHarder, bool tryRotate, const std::string& format)
{
	DecodeHints hints;
	hints.setShouldTryHarder(tryHarder);
	hints.setShouldTryRotate(tryRotate);
	if (!format.empty()) {
		BarcodeFormat f = BarcodeFormatFromString(format.c_str());
		if (f != BarcodeFormat::FORMAT_COUNT) {
			hints.setPossibleFormats({ f });
		}
	}
	_reader = std::make_shared<MultiFormatReader>(hints);
}

static std::shared_ptr<LuminanceSource>
CreateLuminanceSource(QImage& bitmap)
{
    // Convert the image to grayscale
    QImage img(bitmap.convertToFormat(QImage::Format_Grayscale8));

	switch (img.format())
	{
    case QImage::Format_Grayscale8:
        return std::make_shared<GenericLuminanceSource>(0, 0, img.width(), img.height(), img.constBits(), img.width());
    default:
        qDebug() << bitmap.format();
        assert(false && "Format not handled");
	}
	throw std::invalid_argument("Unsupported format");
}

static std::shared_ptr<BinaryBitmap>
CreateBinaryBitmap(QImage& bitmap)
{
	return std::make_shared<HybridBinarizer>(CreateLuminanceSource(bitmap));
}

BarcodeReader::ScanResult
BarcodeReader::scan(QImage& bitmap, int rotations)
{
	Result result(DecodeStatus::NotFound);
	auto binImg = CreateBinaryBitmap(bitmap);
	
	if ((rotations & Rotation0) != 0) {
		result = _reader->read(*binImg);
	}
	if (!result.isValid() && (rotations & Rotation180) != 0) {
		result = _reader->read(*binImg->rotated(180));
	}
	if (!result.isValid() && (rotations & RotationCW90) != 0) {
		result = _reader->read(*binImg->rotated(90));
	}
	if (!result.isValid() && (rotations & RotationCCW90) != 0) {
		result = _reader->read(*binImg->rotated(270));
	}
	if (result.isValid()) {
		std::string text;
		TextUtfEncoding::ToUtf8(result.text(), text);

		return { ToString(result.format()), QString::fromStdString(text) };
	}
	return ScanResult();
}

} // ZXing
