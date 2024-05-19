#include "hash.h"
#include <QApplication>
#include <QLibrary>
#include "bee2/defs.h"

hash::hash()
{
    typedef err_t (*beltHashType)(octet *, const void *, size_t);

    beltHashType beltHash =

        reinterpret_cast<beltHashType>(lib.resolve("beltHash"));

    QByteArray msg = QByteArray::fromHex(
        "B194BAC8 0A08F53B 366D008E 584A5DE4 8504FA9D 1BB6C7AC 252E72C2 02FDCE0D "
        "5BE3D612 17B96181 FE6786AD 716B890B");
    octet hash[32];
    if (beltHash) {
        err_t t = beltHash(hash, msg, msg.size());
        if (t == ERR_OK) {
            qDebug() << QByteArray::fromRawData((const char *)hash, 32).toHex();
        }
    } else {
        qDebug() << "Функция не найдена в библиотеке";
    }

}
