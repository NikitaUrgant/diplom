#include "mainwindow.h"
#include <QLibrary>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "bee2/defs.h"
#include <QFile>

#include <bee2/core/err.h>
#include <bee2/core/mem.h>
#include <bee2/core/hex.h>
#include <bee2/core/str.h>
#include <bee2/core/util.h>
#include <bee2/crypto/bign.h>
#include <bee2/crypto/belt.h>
#include <bee2/crypto/brng.h>






int main(int argc, char *argv[])
{

    //  bign_params parametrs = params();
    //  bign_params* paramets = &parametrs;

    // octet privkey[64];
    // octet pubkey[128];

    //  key_gen(paramets, privkey, pubkey);

    // qDebug()<<"params in main:";
    // qDebug() <<QByteArray::fromRawData((const char *)paramets->seed, 8).toHex();
    // qDebug() <<QByteArray::fromRawData((const char *)paramets->p, 32).toHex();

    // qDebug()<<"struct:";
    // qDebug() << QByteArray::fromRawData((const char *)privkey, 32).toHex();
    // qDebug() << QByteArray::fromRawData((const char *)pubkey, 32).toHex();

    octet sig[128];

   // Sign(sig, paramets, privkey, file);

    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "hash_for_file_RU_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}




