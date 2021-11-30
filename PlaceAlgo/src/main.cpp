#include <QApplication>
#include <QPushButton>
#include <QString>
#include <QVector>
#include <QStack>
#include <QDebug>
#include <iostream>

#include "CompCompute.h"
#include "GetLayout.h"



int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    //qDebug() << FakeData.size();

    //CompCompute compCompute(FakeData);

    GetLayout getLayout;


/*    QMultiHash<QString, int> hash1, hash2, hash3;

    hash1.insert("plenty", 100);
    hash1.insert("plenty", 2000);
    hash1.insert("plenty", 100);
    // hash1.size() == 2

    hash2.insert("plenty", 5000);
    // hash2.size() == 1

    hash3 = hash1 + hash2;
    // hash3.size() == 3

    for (int i = 0; i < hash3.count(); ++i) {
        qDebug() << "?? ";
        qDebug() << hash3.values("plenty");
        //qDebug() << hash3.values("plenty");
    }*/

    //QPushButton button("Hello world!", nullptr);
    // button.resize(200, 100);
    //button.show();
    //button.close();



    qDebug() << " --------return main function----------";

    return QApplication::exec();
}
