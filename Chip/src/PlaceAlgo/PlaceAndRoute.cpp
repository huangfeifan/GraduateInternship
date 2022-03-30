//
// Created by Huangff on 2022/3/23.
//

#include "PlaceAndRoute.h"

PlaceAndRoute::PlaceAndRoute(QList<ConnectData> connectData, int leftPortNum,
                             int rightPortNum, QVector<QVector<int>> modulePortInfo) : m_connectData(connectData),
                                                                                       m_leftPortNum(leftPortNum),
                                                                                       m_rightPortNum(rightPortNum),
                                                                                       m_modulePortInfo(
                                                                                               modulePortInfo) {
    // 布局
    placing();

    // 布线
    routing();
}

void PlaceAndRoute::dataConvert() {

}

void PlaceAndRoute::placing() {

    //qDebug() << m_modulePortInfo << " MODULE_PORT_INFO";
    Placement p(m_connectData, m_modulePortInfo, m_leftPortNum, m_rightPortNum);
    m_modulePos = p.getModulePos();
    m_rightPortPos = p.getRightOutputPortPos();
    //qDebug() << m_rightPortPos.size() << "PLACEMENT_AND_ROUTE_RIGHT_PORT_POS";
    m_leftPortPos = p.getLeftInputPortPos();
    m_modulePortPos = p.getModulePortPos();
    m_graphColumnCount = p.getSchematicColumnCount();
    m_graphRowCount = p.getSchematicRowCount();
    m_moduleSize = p.getModuleSize();

    // 版图的行数和列数 todo modify 可以作为参数输入
    m_graphRowCount = p.getSchematicRowCount();
    m_graphColumnCount = p.getSchematicColumnCount();

    //qDebug() << m_moduleSize << " MODULE_SIZE " ;
    //qDebug() << m_graphRowCount << " " << m_graphColumnCount;
    // qDebug() << m_modulePos << " PLACE_AND_ROUTE_MODULE_POS";
    // qDebug() << m_moduleSize << " PLACE_AND_ROUTE_MODULE_SIZE";
}

void PlaceAndRoute::routing() {
    qDebug() << m_graphRowCount << " " << m_graphColumnCount << "PLACE_AND_ROUTE_ROW_COLUMN";
    Router router(m_graphRowCount, m_graphColumnCount, m_modulePos, m_moduleSize);
    m_paths = QVector<QList<QPoint >>(m_connectData.size());

    // 布线
    QList<WeightAndIndex> list;
    for (int i = 0; i < m_connectData.size(); ++i) {
        int startModule = m_connectData[i].startModuleIndex;
        int startPort = m_connectData[i].startPortIndex;

        int endModule = m_connectData[i].endModuleIndex;
        int endPort = m_connectData[i].endPortIndex;

        // todo improve  y相同比较x
        int startPortY = 0;
        int endPortY = 0;

        if (startModule == -1) {
            startPortY = m_leftPortPos[startPort].y();
        } else {
            startPortY = m_modulePortPos[startModule][startPort].y();
        }
        if (endModule == -1) {
            endPortY = m_rightPortPos[endPort].y();
        } else {
            endPortY = m_modulePortPos[endModule][endPort].y();
        }

        startPortY = startPortY > endPortY ? startPortY : endPortY;

        WeightAndIndex w;
        w.weight = startPortY;
        w.index = i;

        list.push_back(w);
    }

    qSort(list.begin(), list.end(), compareWeight);

    /**/

    for (int i = 0; i < list.size(); ++i) {
        // 第index个连接关系
        int index = list[i].index;

        int startModule = m_connectData[index].startModuleIndex;
        int startPort = m_connectData[index].startPortIndex;

        int endModule = m_connectData[index].endModuleIndex;
        int endPort = m_connectData[index].endPortIndex;

        QPoint startPoint, endPoint;
        const int fanoutDistance = 2;
        if (startModule == -1) {
            startPoint = m_leftPortPos[startPort];
        } else {
            startPoint = m_modulePortPos[startModule][startPort];
            QPoint point = m_modulePos[startModule];
            if (startPoint.x() == point.x()) {
                // 左侧输入port 向左扇出
                startPoint.setX(startPoint.x() - fanoutDistance);
            } else {
                // 右侧输出port 向右扇出
                startPoint.setX(startPoint.x() + fanoutDistance);
            }
        }
        if (endModule == -1) {
            endPoint = m_rightPortPos[endPort];
        } else {
            endPoint = m_modulePortPos[endModule][endPort];
            QPoint point = m_modulePos[endModule];
            if (endPoint.x() == point.x()) {
                // 左侧输入port 向左扇出
                endPoint.setX(endPoint.x() - fanoutDistance);
            } else {
                // 右侧输出port 向右扇出
                endPoint.setX(endPoint.x() + fanoutDistance);
            }
        }

        Pair src = Pair(startPoint.y(), startPoint.x());
        Pair dest = Pair(endPoint.y(), endPoint.x());

        // from startPoint --to--> endPoint
        router.aStarSearch(src, dest);
        if (router.isSuccessFindPaths()) {
            m_paths[i] = router.getPaths();// (y,x)
            // 及时更新被阻挡区域
            qDebug() << m_paths[i] << "PLACE_AND_ROUTE_PATH_I";

            // list(y,x) --> list(x,y)
            for (int j = 0; j < m_paths[i].size(); ++j) {
                QPoint point(m_paths[i][j].y(), m_paths[i][j].x());
                m_paths[i][j] = point;
            }
            router.addPath(m_paths[i]);
        }
    }

    /**/
}

QVector<QPoint> PlaceAndRoute::getModulePos() {
    return m_modulePos;
}

QVector<QVector<QPoint>> PlaceAndRoute::getPortPos() {
    return m_modulePortPos;
}

QVector<QPoint> PlaceAndRoute::getLeftPortPos() {
    return m_leftPortPos;
}

QVector<QPoint> PlaceAndRoute::getRightPortPos() {
    return m_rightPortPos;
}

QVector<QList<QPoint>> PlaceAndRoute::getPaths() {
    return m_paths;
}

QList<QPoint> PlaceAndRoute::getARoutingResult(const Pair &src, const Pair &dest) {
    return QList<QPoint>();
}

QVector<QPoint> PlaceAndRoute::getModuleSize() {
    return m_moduleSize;
}
