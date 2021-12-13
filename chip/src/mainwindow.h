
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QSplitter;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

private:
    void setupMatrix();
    void populateScene();

    QGraphicsScene *scene;
    QSplitter *h1Splitter;
    QSplitter *h2Splitter;
};

#endif // MAINWINDOW_H
