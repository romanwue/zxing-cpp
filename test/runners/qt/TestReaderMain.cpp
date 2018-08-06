#include "BarcodeReader.h"
#include "../../core/src/TextDecoder.h"
#include "../../core/src/TextUtfEncoding.h"
#include <QFile>
#include <QImage>
#include <iostream>

int main(int argc, char** argv)
{
    if (argc <= 1)
    {
        std::cout << "Usage: " << argv[0] << " <test_path_prefix>" << std::endl;
        return 0;
    }

    QFile file(argv[1]);
    file.open(QIODevice::ReadOnly);
    if (!file.exists()) {
        std::cerr << qPrintable(file.errorString());
        file.close();
        return 1;
    }
    file.close();

    QImage bitmap(file.fileName());
    ZXing::BarcodeReader reader(true, true);
    auto r = reader.scan(bitmap);
    std::cout << "Format: " << qPrintable(r.format) << std::endl;
    std::cout << "  Text: " << qPrintable(r.text) << std::endl;
    return 0;
}
