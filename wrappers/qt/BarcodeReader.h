#pragma once

#include <QImage>
#include <QString>
#include <memory>

namespace ZXing {

class MultiFormatReader;

class BarcodeReader
{
public:
	struct ScanResult {
		QString format;
		QString text;    // in UTF-8
	};

	enum Rotation {
		Rotation0 = 1,
		RotationCW90 = 2,
		Rotation180 = 4,
		RotationCCW90 = 8,
	};

	/**
	 Supported formats are:
	 "AZTEC",
	 "CODABAR",
	 "CODE_39",
	 "CODE_93",
	 "CODE_128",
	 "DATA_MATRIX",
	 "EAN_8",
	 "EAN_13",
	 "ITF",
	 "MAXICODE",
	 "PDF_417",
	 "QR_CODE",
	 "RSS_14",
	 "RSS_EXPANDED",
	 "UPC_A",
	 "UPC_E",
	 "UPC_EAN_EXTENSION",
	*/
	explicit BarcodeReader(bool tryHarder = false, bool tryRotate = true, const std::string& format = std::string());

	ScanResult scan(QImage& bitmap, int rotations = Rotation0);

private:
	std::shared_ptr<MultiFormatReader> _reader;
};

} // ZXing
