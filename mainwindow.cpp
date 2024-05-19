#include "mainwindow.h"
#include "ui_mainwindow.h"
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

QString mac(QString str, QFile* file)
{
    if(!file->open(QIODevice::ReadOnly)) return "Проблема с файлом";
    QByteArray msgMAC =file->readAll();


    QString pwd = "C:\\Users\\user\\Desktop\\bee2\\bee2-master\\bee2-master\\win\\vs15\\distrib\\Debug64\\bee2.dll";
    QLibrary lib(pwd);
    lib.load();
    if (!lib.isLoaded()) {
        qDebug() << "Выполнение невозможно: библиотека bee2.dll не была "
                    "загружена!\n Причина ошибки: " +
                        lib.errorString();
        exit(1);
    }

    typedef err_t (*beltMACType)(octet *, const void *, size_t, const void *, size_t);

    beltMACType beltMAC = reinterpret_cast<beltMACType>(lib.resolve("beltMAC"));

    msgMAC = QByteArray::fromHex(msgMAC);
    //qDebug()<<"msgMAC="<<msgMAC;
    QByteArray key = QByteArray::fromHex("E9DEE72C 8F0C0FA6 2DDB49F4 6F739647 06075316 ED247A37 39CBA383 03A98BF6");

    octet mac[8];

    err_t v = beltMAC(mac, msgMAC, msgMAC.size(), key, 32);

    if (v == ERR_OK) {
        qDebug() << QByteArray::fromRawData((const char *)mac, 8).toHex();
        QByteArray hexData = QByteArray::fromRawData((const char *)mac, 8).toHex();
        QString str = QString::fromLocal8Bit(hexData);
        qDebug() <<"mac"<< str;
        QFile macfile("C:/Users/user/Desktop/файлы для теста проги/MAC1.txt");
        macfile.open(QIODevice::ReadWrite | QIODevice::Text);
        macfile.write(QByteArray::fromRawData((const char *)mac, 8).toHex());
        macfile.close();
        return str;
    }else {
        str = "Произошла ошибка";
        return str;
    }//oxfghfbipgjtp
    //hnij'ghigfkopdxgj
}


QString macchck(QString str, QFile* file, QString str1)
{
    if(!file->open(QIODevice::ReadOnly)) return "Проблема с файлом для проверки";
    QByteArray msgMAC =file->readAll();


    QString pwd = "C:\\Users\\user\\Desktop\\bee2\\bee2-master\\bee2-master\\win\\vs15\\distrib\\Debug64\\bee2.dll";
    QLibrary lib(pwd);
    lib.load();
    if (!lib.isLoaded()) {
        qDebug() << "Выполнение невозможно: библиотека bee2.dll не была "
                    "загружена!\n Причина ошибки: " +
                        lib.errorString();
        exit(1);
    }

    typedef err_t (*beltMACType)(octet *, const void *, size_t, const void *, size_t);

    beltMACType beltMAC = reinterpret_cast<beltMACType>(lib.resolve("beltMAC"));

    msgMAC = QByteArray::fromHex(msgMAC);
    //qDebug()<<"msgMAC="<<msgMAC;
    QByteArray key = QByteArray::fromHex("E9DEE72C 8F0C0FA6 2DDB49F4 6F739647 06075316 ED247A37 39CBA383 03A98BF6");

    octet mac[8];

    err_t v = beltMAC(mac, msgMAC, msgMAC.size(), key, 32);
    QByteArray macforcheck;
    if (v == ERR_OK) {
        qDebug() << QByteArray::fromRawData((const char *)mac, 8).toHex();
        macforcheck = QByteArray::fromRawData((const char *)mac, 8).toHex();
    }else {
        qDebug() << "Функция не найдена в библиотеке";
    }

    QFile checkfile(str1);
    if(!checkfile.open(QIODevice::ReadOnly)) return "Проблема с файлом контроля целостности";

    checkfile.open(QIODevice::ReadOnly);
    QByteArray maccheck;
    maccheck =checkfile.readAll();
    qDebug() << maccheck.size();
    qDebug() << maccheck;
    checkfile.close();
    file->close();

    if (macforcheck==maccheck)
    {
        str = "MAC is OK";
        return str;
    }
    else
    {
        str = "MAC is BAD";
        return str;
    }
}


QString hashing(QString str, QFile* file, octet hash[32])
{
    QString pwd = "C:\\Users\\user\\Desktop\\bee2\\bee2-master\\bee2-master\\win\\vs15\\distrib\\Debug64\\bee2.dll";
    QLibrary lib(pwd);
    lib.load();
    if (!lib.isLoaded()) {
        qDebug() << "Выполнение невозможно: библиотека bee2.dll не была "
                    "загружена!\n Причина ошибки: " +
                        lib.errorString();
        exit(1);
    }
    else{
        qDebug() << "Библиотека была удачно загружена!";
    }


    typedef err_t (*beltHashType)(octet *, const void *, size_t);

    beltHashType beltHash = reinterpret_cast<beltHashType>(lib.resolve("beltHash"));

    //  msg = QByteArray::fromHex( "B194BAC8 0A08F53B 366D008E 58 ");
    if(!file->open(QIODevice::ReadOnly)) return "Проблема с файлом";
    QByteArray msg =file->readAll();
    msg = QByteArray::fromHex(msg);

    qDebug() << "msg.size="<< msg.size();
    if (beltHash) {

        err_t t = beltHash(hash, msg, msg.size());

        if (t == ERR_OK) {
            QByteArray hexData = QByteArray::fromRawData((const char *)hash, 32).toHex();
            QString str = QString::fromLocal8Bit(hexData);
            qDebug() << str;
            return str;
        }
    } else {
        str = "Произошла ошибка";
        return str;
    }
    file->close();
}


















extern void utilAssert(int e, const char* file, int line);

#define ASSERT(e) utilAssert(!!(e), __FILE__, __LINE__)

typedef struct
{
    const octet* X;		/*< дополнительное слово */
    size_t count;		/*< размер X в октетах */
    size_t offset;		/*< текущее смещение в X */
    octet state_ex[];	/*< состояние brngCTR */
} brng_ctrx_st;

bool_t memIsValid(const void* buf, size_t count)
{
    return count == 0 || buf != 0;
}
void memCopy(void* dest, const void* src, size_t count)
{
    ASSERT(memIsDisjoint(src, dest, count));
    if (count)
        memcpy(dest, src, count);
}
void utilAssert(int b, const char* file, int line)
{
    if (!b)
    {
        fprintf(stderr, "Assertion in %s::%d\n", file, line);
        abort();
    }
}
bool_t memIsDisjoint(const void* buf1, const void* buf2, size_t count)
{
    ASSERT(memIsValid(buf1, count));
    ASSERT(memIsValid(buf2, count));
    return count == 0 || (const octet*)buf1 + count <= (const octet*)buf2 ||
           (const octet*)buf1 >= (const octet*)buf2 + count;
}

static void brngCTRXStart(const octet key[32], const octet iv[32],
                          const void* X, size_t count, void* state)
{
    QString pwd = "C:\\Users\\user\\Desktop\\bee2\\bee2-master\\bee2-master\\win\\vs15\\distrib\\Debug64\\bee2.dll";
    QLibrary lib(pwd);
    lib.load();
    if (!lib.isLoaded()) {
        qDebug() << "Выполнение невозможно: библиотека bee2.dll не была "
                    "загружена!\n Причина ошибки: " +
                        lib.errorString();
        exit(1);
    }

    typedef void (*brngCTRStartType)(
        void* state,			/*!< [out] состояние */
        const octet key[32],	/*!< [in] ключ */
        const octet iv[32]		/*!< [in] синхропосылка */
        );

    brngCTRStartType brngCTRStart = reinterpret_cast<brngCTRStartType>(lib.resolve("brngCTRStart"));
    qDebug()<<"brngCTRStart.resolve="<<lib.resolve("brngCTRStart");

    typedef size_t (*brngCTR_keepType)();
    brngCTR_keepType brngCTR_keep = reinterpret_cast<brngCTR_keepType>(lib.resolve("brngCTR_keep"));
    qDebug()<<"brngCTR_keep=" <<lib.resolve("brngCTR_keep");

    brng_ctrx_st* s = (brng_ctrx_st*)state;
    ASSERT(memIsValid(s, sizeof(brng_ctrx_st)));
    ASSERT(count > 0);
    ASSERT(memIsValid(s->state_ex, brngCTR_keep()));
    brngCTRStart(s->state_ex, key, iv);
    s->X = (const octet*)X;
    s->count = count;
    s->offset = 0;
}


static void brngCTRXStepR(void* buf, size_t count, void* stack)
{
    QString pwd = "C:\\Users\\user\\Desktop\\bee2\\bee2-master\\bee2-master\\win\\vs15\\distrib\\Debug64\\bee2.dll";
    QLibrary lib(pwd);
    lib.load();
    if (!lib.isLoaded()) {
        qDebug() << "Выполнение невозможно: библиотека bee2.dll не была "
                    "загружена!\n Причина ошибки: " +
                        lib.errorString();
        exit(1);
    }

    typedef void (*brngCTRStepRType)(
        void* buf,			/*!< [in,out] дополн. / псевдослучайные данные */
        size_t count,		/*!< [in] число октетов buf */
        void* state			/*!< [in,out] состояние */
        );

    brngCTRStepRType brngCTRStepR = reinterpret_cast<brngCTRStepRType>(lib.resolve("brngCTRStepR"));
    qDebug()<<"brngCTRStepR.resolve="<<lib.resolve("brngCTRStepR");

    brng_ctrx_st* s = (brng_ctrx_st*)stack;
    octet* buf1 = (octet*)buf;
    size_t count1 = count;
    ASSERT(memIsValid(s, sizeof(brng_ctrx_st)));
    // заполнить buf
    while (count1)
        if (count1 < s->count - s->offset)
        {
            memCopy(buf1, s->X + s->offset, count1);
            s->offset += count1;
            count1 = 0;
        }
        else
        {
            memCopy(buf1, s->X + s->offset, s->count - s->offset);
            buf1 += s->count - s->offset;
            count1 -= s->count - s->offset;
            s->offset = 0;
        }
    // сгенерировать
    // qDebug()<<count;
    count = 0;
    brngCTRStepR(buf, count, s->state_ex);
}

bign_params params ()
{
    QString pwd = "C:\\Users\\user\\Desktop\\bee2\\bee2-master\\bee2-master\\win\\vs15\\distrib\\Debug64\\bee2.dll";
    QLibrary lib(pwd);
    lib.load();
    if (!lib.isLoaded()) {
        qDebug() << "Выполнение невозможно: библиотека bee2.dll не была "
                    "загружена!\n Причина ошибки: " +
                        lib.errorString();
        exit(1);
    }

    typedef err_t (*bignParamsType)(bign_params* params,	/*!< [out] стандартные параметры */
                                    const char* name		/*!< [in] имя параметров */);

    bignParamsType bignParamsStd = reinterpret_cast<bignParamsType>(lib.resolve("bignParamsStd"));
    qDebug()<<lib.resolve("bignParamsStd");

    const char* name = "1.2.112.0.2.0.34.101.45.3.1";

    bign_params a;
    bign_params *params = &a;

    err_t t = bignParamsStd(params, name);

    // qDebug()<<"params not params:";
    // qDebug() << params->l;
    // qDebug() << params->seed;
    // qDebug() <<QByteArray::fromRawData((const char *)params->seed, 8).toHex();
    // qDebug() <<QByteArray::fromRawData((const char *)params->p, 32).toHex();
    // qDebug() <<QByteArray::fromRawData((const char *)params->b, 32).toHex();
    // qDebug() <<QByteArray::fromRawData((const char *)a.seed, 8).toHex();

    return a;
}



int key_gen (const bign_params* paramets, octet privkey[32], octet pubkey[64], octet brng_state[1024], int k)
{
    QString pwds = "C:\\Users\\user\\Desktop\\bee2\\bee2-master\\bee2-master\\win\\vs15\\distrib\\Debug64\\bee2.dll";
    QLibrary lib(pwds);
    lib.load();
    if (!lib.isLoaded()) {
        qDebug() << "Выполнение невозможно: библиотека bee2.dll не была "
                    "загружена!\n Причина ошибки: " +
                        lib.errorString();
        exit(1);
    }

    typedef err_t (*bignKeypairGenType)(
        octet privkey[],			/*!< [out] личный ключ */
        octet pubkey[],				/*!< [out] открытый ключ */
        const bign_params* params,	/*!< [in] долговременные параметры */
        gen_i rng,				/*!< [in] генератор случайных чисел */
        void* rng_state				/*!< [in,out] состояние генератора */
        );

    typedef void (*brngCTRStepRType)(
        void* buf,			/*!< [in,out] дополн. / псевдослучайные данные */
        size_t count,		/*!< [in] число октетов buf */
        void* state			/*!< [in,out] состояние */
        );

    typedef err_t (*bignKeypairValType)(
        const bign_params* params,	/*!< [in] долговременные параметры */
        const octet privkey[],		/*!< [in] личный ключ */
        const octet pubkey[]		/*!< [in] открытый ключ */
        );

    typedef const octet* (*beltHType)();

    brngCTRStepRType brngCTRStepR = reinterpret_cast<brngCTRStepRType>(lib.resolve("brngCTRStepR"));
    qDebug()<<"brngCTRStepR.resolve="<<lib.resolve("brngCTRStepR");

    bignKeypairGenType bignKeypairGen = reinterpret_cast<bignKeypairGenType>(lib.resolve("bignKeypairGen"));
    qDebug()<<"bignKeypairGen.resolve=" <<lib.resolve("bignKeypairGen");

    beltHType beltH = reinterpret_cast<beltHType>(lib.resolve("beltH"));
    qDebug()<<"beltH.resolve=" <<lib.resolve("beltH");

    bignKeypairValType bignKeypairVal = reinterpret_cast<bignKeypairValType>(lib.resolve("bignKeypairVal"));
    qDebug()<<"bignKeypairVal.resolve=" <<lib.resolve("bignKeypairVal");

    qDebug()<<"state before start  " << brng_state;

    qDebug()<<k;

    if(k==0)
    {
    brngCTRXStart(beltH() + 128, beltH() + 128 + 64, beltH(), 8 * 32, brng_state);
        k=1;
    }
    // qDebug() <<"До функции:"<< QByteArray::fromRawData((const char *)privkey, 32).toHex();
    // qDebug() << QByteArray::fromRawData((const char *)pubkey, 32).toHex();    brngCTRXStart(beltH() + 128, beltH() + 128 + 64, beltH(), 8 * 32, brng_state);

    qDebug()<<"state before step   " << brng_state;

    err_t v = bignKeypairGen (privkey, pubkey, paramets,  brngCTRXStepR, brng_state);

    if (v==ERR_OK)
    {
        qDebug()<<"key pair gen OK";
    }
    else
        qDebug()<<"v="<<v;

    qDebug()<<"state = " << brng_state;

    qDebug() <<"после функции:"<< QByteArray::fromRawData((const char *)privkey, 32).toHex();
    qDebug() << QByteArray::fromRawData((const char *)pubkey, 64).toHex();

    err_t t = bignKeypairVal(paramets,privkey,pubkey);

    if (t==ERR_OK)
    {
        qDebug()<<"key pair is valid";
    }
    else
        qDebug()<<"t="<<t;
}


QString Sign (octet sig[], const bign_params* paramets, QFile* file1, QFile* file)
{
    QString pwd = "C:\\Users\\user\\Desktop\\bee2\\bee2-master\\bee2-master\\win\\vs15\\distrib\\Debug64\\bee2.dll";
    QLibrary lib(pwd);
    lib.load();
    if (!lib.isLoaded()) {
        qDebug() << "Выполнение невозможно: библиотека bee2.dll не была "
                    "загружена!\n Причина ошибки: " +
                        lib.errorString();
        exit(1);
    }
    else{
        qDebug() << "Библиотека была удачно загружена!";
    }


    typedef err_t  (*bignSignType)(
        octet sig[],				/*!< [out] подпись */
        const bign_params* params,	/*!< [in] долговременные параметры */
        const octet oid_der[],		/*!< [in] идентификатор хэш-алгоритма */
        size_t oid_len,				/*!< [in] длина oid_der в октетах */
        const octet hash[],			/*!< [in] хэш-значение */
        const octet privkey[],		/*!< [in] личный ключ */
        gen_i rng,					/*!< [in] генератор случайных чисел */
        void* rng_state				/*!< [in,out] состояние генератора */
        );

    bignSignType bignSign = reinterpret_cast<bignSignType>(lib.resolve("bignSign"));

    qDebug() << "bignSign.resolve="<<lib.resolve("bignSign");

    typedef const octet* (*beltHType)();

    beltHType beltH = reinterpret_cast<beltHType>(lib.resolve("beltH"));
    qDebug()<<"beltH.resolve=" <<lib.resolve("beltH");

    typedef err_t (*bignOidToDERType)(
        octet der[],		/*!< [out] DER-код идентификатора */
        size_t* count,		/*!< [in,out] длина буфера der / длина DER-кода */
        const char* oid		/*!< [in] строковое представление идентификатора */
        );

    bignOidToDERType bignOidToDER = reinterpret_cast<bignOidToDERType>(lib.resolve("bignOidToDER"));

    qDebug() << "bignOidToDER.resolve="<<lib.resolve("bignOidToDER");
    octet der[11];
    size_t* count;
    size_t count1 = 32;
    count = &count1;
    qDebug()<<"der="<< QByteArray::fromRawData((const char *)der, 11).toHex();;
    const char* oid = "1.2.112.0.2.0.34.101.31.81";

    err_t v = bignOidToDER (der, count, oid);
    if (v==ERR_OK)
    {
        qDebug()<<"oid is ok";
    }
    else
        qDebug()<<"v="<<v;

    qDebug()<<"der="<< QByteArray::fromRawData((const char *)der, 11).toHex();;

    octet brng_state[1024];

    bign_params parametrs= params();
    paramets=&parametrs;
    octet privkey[32];
    octet pubkey[64];

    int k =key_gen(paramets, privkey, pubkey,brng_state, k);


    QString str;
    octet hash[32];

    hashing(str, file, hash);

    qDebug() << str;

    //brngCTRXStart(beltH() + 128, beltH() + 128 + 64, beltH(), 8 * 32, brng_state);

    err_t t = bignSign(sig, paramets, der, 11, hash, privkey, brngCTRXStepR, brng_state);

    qDebug() << t;

        QByteArray hexData = QByteArray::fromRawData((const char *)sig, 32).toHex();
        str = QString::fromLocal8Bit(hexData);
        qDebug() << str;
        return str;

}







































MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->label_file_path->setVisible(false);
    ui->file_path->setVisible(false);
    ui->file_path_complete->setVisible(false);
    ui->textEdit_hash->setVisible(false);
    ui->label_hash_result->setVisible(false);
    ui->textEdit_hashfile_was_complete->setVisible(false);

ui->label_mac->setVisible(false);
ui->lineEdit_mac->setVisible(false);
ui->pushButton_mac->setVisible(false);
ui->textEdit_mac->setVisible(false);
ui->textEdit_mac_file_complete->setVisible(false);
ui->label_mac_result->setVisible(false);
ui->textEdit_mac_file_complete->setVisible(false);

ui->label_mac_check->setVisible(false);
ui->lineEdit_mac_check->setVisible(false);
ui->pushButton_mac_check->setVisible(false);
ui->textEdit_mac_check->setVisible(false);
ui->textEdit_mac_for_check->setVisible(false);
ui->label_mac_file_for_check->setVisible(false);

ui->label_path_to_file_ecp->setVisible(false);
ui->textEdit_path_to_file_ecp->setVisible(false);
ui->label_key_for_ecp->setVisible(false);
ui->textEdit_key_for_ecp->setVisible(false);
ui->pushButton_ok_ecp->setVisible(false);
ui->textEdit_ecp->setVisible(false);
ui->textEdit_ecp_result->setVisible(false);
ui->label_ecp_result->setVisible(false);

ui->textEdit_keys_ok->setVisible(false);
ui->textEdit_privkey->setVisible(false);
ui->textEdit_pubkey->setVisible(false);
ui->label_pubkey->setVisible(false);
ui->label_privkey->setVisible(false);
ui->pushButton_key_gen->setVisible(false);
ui->label_key_gen->setVisible(false);
ui->pushButton_clear->setVisible(false);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    ui->textEdit_hashfile_was_complete->setVisible(false);
    ui->textEdit_hash->setVisible(false);
    ui->file_path->setVisible(false);
    ui->file_path_complete->setVisible(false);
    ui->label_file_path->setVisible(false);
    ui->label_hash_result->setVisible(false);

    ui->label_mac->setVisible(false);
    ui->lineEdit_mac->setVisible(false);
    ui->pushButton_mac->setVisible(false);
    ui->textEdit_mac->setVisible(false);
    ui->textEdit_mac_file_complete->setVisible(false);
    ui->label_mac_result->setVisible(false);
    ui->textEdit_mac_file_complete->setVisible(false);

    ui->label_mac_check->setVisible(false);
    ui->lineEdit_mac_check->setVisible(false);
    ui->pushButton_mac_check->setVisible(false);
    ui->textEdit_mac_check->setVisible(false);
    ui->textEdit_mac_for_check->setVisible(false);
    ui->label_mac_file_for_check->setVisible(false);

    ui->label_path_to_file_ecp->setVisible(false);
    ui->textEdit_path_to_file_ecp->setVisible(false);
    ui->label_key_for_ecp->setVisible(false);
    ui->textEdit_key_for_ecp->setVisible(false);
    ui->pushButton_ok_ecp->setVisible(false);

    ui->textEdit_keys_ok->setVisible(false);
    ui->textEdit_privkey->setVisible(false);
    ui->textEdit_pubkey->setVisible(false);
    ui->label_pubkey->setVisible(false);
    ui->label_privkey->setVisible(false);
    ui->pushButton_key_gen->setVisible(false);
    ui->label_key_gen->setVisible(false);

    if (item->text(0)=="хэширование")
    {
       ui->file_path->setVisible(true);
       ui->file_path_complete->setVisible(true);
       ui->label_file_path->setVisible(true);
    }
    if(item->text(0)=="формирование файла")
    {
        ui->label_mac->setVisible(true);
        ui->lineEdit_mac->setVisible(true);
        ui->pushButton_mac->setVisible(true);
    }
    if(item->text(0)=="проверка файла")
    {
        ui->label_mac_check->setVisible(true);
        ui->lineEdit_mac_check->setVisible(true);
        ui->pushButton_mac_check->setVisible(true);
        ui->textEdit_mac_for_check->setVisible(true);
        ui->label_mac_file_for_check->setVisible(true);
    }

    if(item->text(0)=="выработка значения ЭЦП")
    {
        ui->label_path_to_file_ecp->setVisible(true);
        ui->textEdit_path_to_file_ecp->setVisible(true);
        ui->label_key_for_ecp->setVisible(true);
        ui->textEdit_key_for_ecp->setVisible(true);
        ui->pushButton_ok_ecp->setVisible(true);
        ui->pushButton_clear->setVisible(true);
    }
    if(item->text(0)=="проверка значения ЭЦП")
    {

    }
    if(item->text(0)=="генерация ключевой пары")
    {
        ui->pushButton_key_gen->setVisible(true);
        ui->label_key_gen->setVisible(true);
    }
}


void MainWindow::on_file_path_cursorPositionChanged(int arg1, int arg2)
{
   //ui->file_path->setVisible(true);
}


void MainWindow::on_file_path_complete_clicked()
{
    ui->label_hash_result->setVisible(false);
    ui->textEdit_hash->setVisible(false);
    ui->textEdit_hashfile_was_complete->setVisible(false);
    QString str = ui->file_path->text();
    QFile file(str);
    QFile *fille =&file;
    octet hash[32];
    str=hashing(str, fille, hash);
    if (str == "Проблема с файлом")
    {
        ui->textEdit_hashfile_was_complete->setVisible(true);
        ui->textEdit_hashfile_was_complete->setText(str);
    }
    else
        if (str == "Произошла ошибка")
    {
        ui->textEdit_hashfile_was_complete->setVisible(true);
        ui->textEdit_hashfile_was_complete->setText(str);
    }
    else
    {
        ui->textEdit_hash->setVisible(true);
        ui->textEdit_hash->setText(str);
        ui->textEdit_hashfile_was_complete->setVisible(true);
        ui->label_hash_result->setVisible(true);
        ui->textEdit_hashfile_was_complete->setText("Файл с хэш-значением был создан");
    }
}


void MainWindow::on_pushButton_mac_clicked()
{
    ui->textEdit_mac_file_complete->setVisible(false);
    ui->label_mac_result->setVisible(false);
    ui->textEdit_mac->setVisible(false);
    QString str = ui->lineEdit_mac->text();
    QFile file(str);
    QFile *fille =&file;
    str=mac(str, fille);
    if (str == "Проблема с файлом")
    {
        ui->textEdit_mac_file_complete->setVisible(true);
        ui->textEdit_mac_file_complete->setText(str);
    }
    else
        if (str == "Произошла ошибка")
        {
            ui->textEdit_mac_file_complete->setVisible(true);
            ui->textEdit_mac_file_complete->setText(str);
        }
        else
        {
            ui->textEdit_mac->setVisible(true);
            ui->textEdit_mac->setText(str);
            ui->textEdit_mac_file_complete->setVisible(true);
            ui->label_mac_result->setVisible(true);
            ui->textEdit_mac_file_complete->setText("Файл контроля целостности был создан");
        }

}


void MainWindow::on_pushButton_mac_check_clicked()
{
    ui->textEdit_mac_check->setVisible(false);
    QString str = ui->lineEdit_mac_check->text();
    QString str1 = ui->textEdit_mac_for_check->toPlainText();
    QFile file(str);
    QFile *fille =&file;
    str=macchck(str, fille, str1);
    if (str == "Проблема с файлом")
    {
        ui->textEdit_mac_check->setVisible(true);
        ui->textEdit_mac_check->setText(str);
    }
    else
        if (str == "Произошла ошибка")
        {
        ui->textEdit_mac_check->setVisible(true);
        ui->textEdit_mac_check->setText(str);
        }
        else
        {
        ui->textEdit_mac_check->setVisible(true);
        ui->textEdit_mac_check->setText(str);
        }

}


//"C:\\Users\\user\\Desktop\\файлы для теста проги\\MAC.docx"


void MainWindow::on_pushButton_ok_ecp_clicked()
{
    ui->textEdit_ecp->setVisible(false);
    ui->textEdit_ecp_result->setVisible(false);
    ui->label_ecp_result->setVisible(false);

    QString str = ui->textEdit_path_to_file_ecp->toPlainText();
    QString str1 = ui->textEdit_key_for_ecp->toPlainText();
    QFile file(str);
    QFile *fille =&file;
    QFile file1(str1);
    QFile *fille1 =&file1;
    octet sig[32];
    const bign_params* paramets;

    str = Sign (sig, paramets, fille1,fille);


    if (str == "Произошла ошибка")
    {
        ui->textEdit_ecp->setVisible(true);
        ui->textEdit_ecp->setText(str);
    }
    else
    {
        ui->textEdit_ecp->setVisible(true);
        ui->textEdit_ecp->setText(str);
    }

    ui->textEdit_path_to_file_ecp->clear();
    ui->textEdit_key_for_ecp->clear();
}


void MainWindow::on_pushButton_key_gen_clicked()
{
    ui->textEdit_keys_ok->setVisible(false);
    ui->textEdit_privkey->setVisible(false);
    ui->textEdit_pubkey->setVisible(false);
    ui->label_pubkey->setVisible(false);
    ui->label_privkey->setVisible(false);

    bign_params parametrs= params();
    const bign_params* paramets=&parametrs;
    octet privkey[32];
    octet pubkey[64];
    octet brng_state[1024];

    int k = key_gen(paramets, privkey, pubkey, brng_state, k);

    QByteArray privData = QByteArray::fromRawData((const char *)privkey, 32).toHex();
    QString priv = QString::fromLocal8Bit(privData);

    QByteArray pubData = QByteArray::fromRawData((const char *)pubkey, 64).toHex();
    QString pub = QString::fromLocal8Bit(pubData);   

        ui->textEdit_privkey->setVisible(true);
    ui->textEdit_privkey->setText(priv);
            ui->textEdit_pubkey->setVisible(true);
    ui->textEdit_pubkey->setText(pub);
                ui->textEdit_keys_ok->setVisible(true);
    ui->textEdit_keys_ok->setText("Ключи сгенерированы");
}

//C:\Users\user\Desktop\файлы для теста проги\MAC.docx

void MainWindow::on_pushButton_clear_clicked()
{
    ui->textEdit_path_to_file_ecp->clear();
    ui->textEdit_key_for_ecp->clear();
    ui->textEdit_ecp->clear();
}

