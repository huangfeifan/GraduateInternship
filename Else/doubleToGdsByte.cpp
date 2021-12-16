#include <QApplication>
#include <QPushButton>
#include <QList>
#include <QDebug>
#include <QFile>


void convertToGDSDouble(double data, unsigned char memory[8]);

void convertToGDSDouble(double data, unsigned char memory[8]) {

    for (int i = 0; i < 8; ++i) {
        memory[i] = 0;
    }
    if (data == 0) {
        return;
    }

    // S
    int signal = 64;
    if (data < 0) {
        data = -data;
        signal += 128;
    }

    // Exponent
    // mantissa is always a positive fraction >=1/16 and <1
    int exp = 0;
    if (data < 0.0625) {//  1/16=0.0625
        while (data < 0.0625) {
            data *= 16;
            exp--;
        }
    } else if (data >= 1) {
        while (data >= 1) {
            data /= 16;
            exp++;
            //qDebug() << "  iii";
        }
    }


    memory[0] = signal + exp;


    QList<int> ttemp;
    for (int i = 0; i < 56; ++i) {
        ttemp.push_back(0);
    }
/*    qDebug() << data;
    qDebug() << "  ";*/

    int count = 0;
    while (data && count < 56) {
        data *= 2;
        if (data >= 1) {
            data -= 1;
            ttemp[count] = 1;
        }
        count++;
    }

/*    QString str;
    for (int i = 0; i < 56; ++i) {
        str += QString::number(ttemp[i]);
    }
    qDebug() << str;*/

    int index = 0;
    for (int i = 0; i < 56; ++i) {
        index = i / 8;// 0 1 2 3 4 5 6
        if (ttemp[i] == 1) {
            memory[index + 1] *= 2;
            memory[index + 1] += 1;
        } else {
            memory[index + 1] *= 2;
        }
    }

    return;

/*    while (data || count < 56) {
        int index = count / 8 + 1;
        //memory[index] = 0;
        data *= 2;
        if (data >= 1) {
            data -= 1;
            // 1
            *//*memory[index] *= 2;
            memory[index] += 1;*//*
        } else {
            // 0
            //memory[index] *= 2;
        }
        count++;
    }*/


}

QByteArray decimalToByte(double decimal) {
    // 小数转换到二进制
    QByteArray byteArray;
    while (decimal) {
        decimal *= 2;
        if (decimal >= 1) {
            decimal -= 1;
            byteArray.append('1');
        } else {
            byteArray.append('0');
        }
        QString string = QString(byteArray);
        qDebug() << string;
    }

    return byteArray;
}


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    double data = 0.0001;//= 0.09999996423721313;
    data = 0.001;
    unsigned char memory[8];

    convertToGDSDouble(data, memory);

    char temp[100];
    for (int i = 0; i < 8; ++i) {
        itoa(int(memory[i]), temp, 2);
        qDebug() << temp;
        //out << memory[i];

        //qDebug() << memory[i];
    }


    return QApplication::exec();
}
