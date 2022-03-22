
#include <QApplication>
#include <QPushButton>
#include <QTime>
#include <QRandomGenerator>
#include <QDebug>

#include "MyWidget/mainwindow.h"
#include "MyWidget/MyPaint.h"
#include "MyWidget/AfterPlacement.h"

#include "PlaceAlgo/Placer.h"
#include "PlaceAlgo/ComputeAbsolutePos.h"
#include "PlaceAlgo/SchematicPlacement.h"
#include "PlaceAlgo/MyStruct.h"
#include "PlaceAlgo/Router.h"

#include "Test.h"
#include "Data.h"


/// 根据结构体的某一个属性排序

void getRandomConnectData();

QVector<QList<int>> getModuleConnectData(const QList<ConnectData> &connectData, int moduleCount) {
    QVector<QList<int>> moduleData = QVector<QList<int>>(moduleCount);
    for (int i = 0; i < connectData.size(); ++i) {
        int startModule = connectData[i].startModuleIndex;
        int endModule = connectData[i].endModuleIndex;

        if (endModule == -1 || startModule == -1) {
            // 输入为单独的port
            // 输入为单独的port
            continue;
        }
        moduleData[startModule].push_back(endModule);
    }
    return moduleData;
}

bool compareHeight(const HeightAndIndex &first, const HeightAndIndex &second) {
    // 从大到小排序
    if (first.height > second.height) {
        return true;
    }
    return false;
}

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(images);

    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    /// 计算模块大小 根据输入输出个数() 高度统一(参数)
    int moduleCount = graphData.size();
    QVector<QPoint> size = QVector<QPoint>(moduleCount);
    QVector<int> inDegree = QVector<int>(moduleCount);
    QVector<int> outDegree = QVector<int>(moduleCount);
    for (int i = 0; i < graphData.size(); ++i) {
        // 出度
        outDegree[i] = graphData[i].size();
        int end;
        for (int j = 0; j < graphData[i].size(); ++j) {
            end = graphData[i][j];
            inDegree[end]++;
        }
    }
    const int WIDTH = 30;
    int height = 10;// 一个port 20 两个port30 依次类推
    for (int i = 0; i < moduleCount; ++i) {
        size[i].setX(WIDTH);
        int temp = inDegree[i] > outDegree[i] ? inDegree[i] : outDegree[i];
        size[i].setY((temp + 1) * height);
    }
    SchematicPlacement sp(graphData, size);

    // 获得相对位置
    QVector<QPoint> moduleRelativePos = sp.getModuleRelativePos();
    // 获得绝对位置
    QVector<QPoint> modulePos = sp.getModulePos();
    //qDebug() << moduleRelativePos;

    // 连接数据
    QList<ConnectData> data = FAKE_ALL_CONNECT_DATA;
    QVector<QList<int>> m_moduleConnectData = QVector<QList<int>>(MODULE_COUNT);


    m_moduleConnectData = getModuleConnectData(data, MODULE_COUNT);
    //qDebug() << m_moduleConnectData;

    // todo add function convert data --> connectData easy

    QVector<QVector<PortInfo>> modulePort = QVector<QVector<PortInfo>>(MODULE_COUNT);
    QVector<PortInfo> leftInputPort = QVector<PortInfo>(LEFT_INPUT_PORT_COUNT);// 单独的输入port
    QVector<PortInfo> rightOutputPort = QVector<PortInfo>(RIGHT_OUTPUT_PORT_COUNT);// 单独的输出port
    QVector<QVector<int>> modulePortIndex = QVector<QVector<int>>(MODULE_COUNT);;// 排序后的modulePortIndex // 绘制的时候需要稍微计算下
    QVector<int> leftInputPortOrder = QVector<int>(LEFT_INPUT_PORT_COUNT);
    QVector<int> rightOutputPortOrder = QVector<int>(RIGHT_OUTPUT_PORT_COUNT);

    /// 初始化数据
    for (int i = 0; i < MODULE_COUNT; ++i) {
        modulePort[i] = QVector<PortInfo>(MODULE_PORT_LIST[i]);
        /// 排序后ports的index存放处
        modulePortIndex[i] = QVector<int>(MODULE_PORT_LIST[i]);
    }

    qDebug() << moduleRelativePos << "   - -- -  - - - - module relative pos";

    // 计算所有port的信息   1.计算单独的port的height  2.计算属于模块port的height
    for (int i = 0; i < data.size(); ++i) {
        int startModule = data[i].startModuleIndex;
        int endModule = data[i].endModuleIndex;
        int startPort = data[i].startPortIndex;
        int endPort = data[i].endPortIndex;

        /// 注意 单独的port计算的是绝对高度
        if (startModule == -1) {
            /// 输入为单独的port
            modulePort[endModule][endPort].height = 0;
            leftInputPort[startPort].height = modulePos[endModule].y();
            continue;
        }
        if (endModule == -1) {
            /// 输入为单独的port
            modulePort[startModule][startPort].height = 0;
            rightOutputPort[endPort].height = modulePos[startModule].y();
            continue;
        }
        /// 注意 模块上的port计算的是相对高度
        // 输入输出均为module上的port
        // Bug 数据没有成为闭环
        //modulePort[startModule][startPort].height = moduleRelativePos[endModule].y() + 1;
        //modulePort[endModule][endPort].height = moduleRelativePos[startModule].y() + 1;
    }

    /// module的Port排序
    // bug Fix
    for (int i = 0; i < modulePort.size(); ++i) {
        QList<HeightAndIndex> left, right;
        for (int j = 0; j < modulePort[i].size(); ++j) {
            HeightAndIndex temp;
            temp.height = modulePort[i][j].height;
            temp.index = j;// index
            if (modulePort[i][j].isLeftInput) {
                /// 左侧输入的Port input left
                left.push_back(temp);
            } else {
                /// 右侧输出的Port output right
                right.push_back(temp);
            }
        }

        /// 排序
        qSort(left.begin(), left.end(), compareHeight);
        qSort(right.begin(), right.end(), compareHeight);

        /// 数据存储在modulePortIndex中
        for (int j = 0; j < left.size(); ++j) {
            int index = left[j].index;
            modulePortIndex[i][index] = j;
        }
        for (int j = 0; j < right.size(); ++j) {
            int index = right[j].index;
            modulePortIndex[i][index] = j;
        }
    }
    // port位置的计算

    // 单独port的排序及位置计算
    QList<HeightAndIndex> left, right;
    for (int i = 0; i < leftInputPort.size(); ++i) {
        HeightAndIndex temp;
        temp.height = leftInputPort[i].height;
        temp.index = i;
        left.push_back(temp);
    }
    for (int i = 0; i < rightOutputPort.size(); ++i) {
        HeightAndIndex temp;
        temp.height = rightOutputPort[i].height;
        temp.index = i;
        right.push_back(temp);
    }
    qSort(left.begin(), left.end(), compareHeight);
    qSort(right.begin(), right.end(), compareHeight);
    /// 数据存储在modulePortIndex中
    for (int i = 0; i < left.size(); ++i) {
        int index = left[i].index;
        leftInputPortOrder[index] = i;
    }

    for (int i = 0; i < right.size(); ++i) {
        int index = right[i].index;
        rightOutputPortOrder[index] = i;
    }

    QHash<int, int> leftPortHash;
    QHash<int, int> rightPortHash;

    //qDebug() << size;

    MyPaint myPaint(sp.getModulePos(), size, inDegree, outDegree);
    myPaint.show();

    return app.exec();
}


void getRandomConnectData() {
    // 模块数 1-10
    int maxModuleCount = 10;

    int moduleCount = QRandomGenerator::global()->bounded(maxModuleCount);
    //moduleCount = 5;
    //qDebug() << moduleCount << " ModuleCount";

    // in the range between 0 (inclusive) and highest (exclusive)
    int connectLineCount = 5 * QRandomGenerator::global()->bounded(40);
    QVector<QPoint> connectList = QVector<QPoint>(connectLineCount);// 模拟连接数据
    QVector<int> inDegree = QVector<int>(moduleCount);
    QVector<int> outDegree = QVector<int>(moduleCount);
    for (int i = 0; i < connectLineCount; ++i) {
        int start = QRandomGenerator::global()->bounded(moduleCount + 1) - 1;
        int end = QRandomGenerator::global()->bounded(moduleCount + 1) - 1;
        //qDebug() << start << " " << end;
        if (start != end) {
            if (start == -1 && end == -1) {
                // 数据无效
                continue;
            }

            if (start != -1 && end != -1) {
                outDegree[start]++;
                inDegree[end]++;
                //qDebug() << start << " " << end;
            }


            if (start == -1) {
                // start = -1 表示单独的输入port
                if (end != -1) {

                }
            }

            if (end == -1) {
                // end = -1 表示单独的输出port
                if (start != -1) {

                }
            }
        }
    }

    //qDebug() << inDegree << " InDegree";
    //qDebug() << outDegree << " outDegree";

    //qDebug() << "-------------------------\n\n\n";
    int width = 100;
    int gap = 5;
    QVector<QPoint> size = QVector<QPoint>(moduleCount);
    for (int i = 0; i < moduleCount; ++i) {
        size[i].setX(width);
        int max = inDegree[i] > outDegree[i] ? inDegree[i] : outDegree[i];
        int height = (max + 1) * gap;

        size[i].setY(height);
    }
}

/// 演示
//    qDebug() << placement.getRelativePosition().sccSize();
//    qDebug() << placement.m_nameList.sccSize();
//    qDebug() << placement.m_nameListScc.sccSize();

//AfterPlacement a(placement.getRelativePosition(), graphData, placement.m_nameList);
//a.setWindowTitle("place all module");
//a.show();

//AfterPlacement b(placement.m_relativePositionScc, placement.m_connectionScc, placement.m_nameListScc);
//b.setWindowTitle("place all scc");
//b.show();

//AfterPlacement c(placement.relativePosition, placement.sccConnect, placement.sccList);
//c.setWindowTitle("a scc");
//c.show();

