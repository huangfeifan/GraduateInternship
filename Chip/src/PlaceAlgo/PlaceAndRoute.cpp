//
// Created by Huangff on 2022/3/23.
//

#include "PlaceAndRoute.h"

PlaceAndRoute::PlaceAndRoute(QList<ConnectData> connectData, int leftPortNum,
                             int rightPortNum, QVector<QVector<int>> modulePortInfo) : m_connectData(connectData),
                                                                                       m_leftPortNum(leftPortNum),
                                                                                       m_rightPortNum(rightPortNum),
                                                                                       m_modulePortDirection(
                                                                                               modulePortInfo) {
    // 布局
    placing();

    // 布线
    routing();
}

void PlaceAndRoute::dataConvert() {

}

void PlaceAndRoute::placing() {

    Placement p(m_connectData, m_modulePortDirection, m_leftPortNum, m_rightPortNum);
    m_modulePos = p.getModulePos();
    m_moduleSize = p.getModuleSize();
    m_modulePortPos = p.getModulePortPos();
    m_leftPortPos = p.getLeftInputPortPos();
    m_rightPortPos = p.getRightOutputPortPos();
    m_moduleDirectionAndOrder = p.getModulePortDirectionAndOrder();

    m_moduleLeftPortNum = p.getModuleLeftPortNum();// 模块i左侧port个数
    m_moduleRightPortNum = p.getModuleRightPortNum();
    m_moduleTopPortNum = p.getModuleTopPortNum();
    m_moduleBottomPortNum = p.getModuleBottomPortNum();

    // 版图的行数和列数 todo modify 可以作为参数输入
    m_graphRowCount = p.getSchematicRowCount();
    m_graphColumnCount = p.getSchematicColumnCount();

}

void PlaceAndRoute::routing() {
    //qDebug() << m_graphRowCount << " " << m_graphColumnCount << "PLACE_AND_ROUTE_ROW_COLUMN";
    Router router(m_graphRowCount, m_graphColumnCount, m_modulePos, m_moduleSize);
    m_paths = QVector<QList<QPoint >>(m_connectData.size());

    // 确定布线的顺序
    QList<WeightAndIndex> list = getRoutingOrder();

    /**/
    for (int i = 0; i < list.size(); ++i) {
        int index = list[i].index;        // 第index个连接关系
        int startModule = m_connectData[index].startModuleIndex;
        int startPort = m_connectData[index].startPortIndex;
        int endModule = m_connectData[index].endModuleIndex;
        int endPort = m_connectData[index].endPortIndex;

        QPoint startPoint, endPoint;        // 计算startPoint和endPoint
        if (startModule == -1) {
            startPoint = m_leftPortPos[startPort];
        } else {
            startPoint = m_modulePortPos[startModule][startPort];
        }
        if (endModule == -1) {
            endPoint = m_rightPortPos[endPort];
        } else {
            endPoint = m_modulePortPos[endModule][endPort];
        }

        //qDebug() << startPoint << " Start " << endPoint;

        // port的上下关系
        bool startHigher = startPoint.y() < endPoint.y();// 起点在终点上方
        bool startLefter = startPoint.x() < endPoint.x();// 起点在终点左侧

        // 计算fanoutPoint
        QPoint startFanoutPoint, endFanoutPoint;
        const int fanoutDistance = 2;//
        if (startModule == -1) {// 左侧的port往右扇出一段
            startFanoutPoint = startPoint + QPoint(fanoutDistance, 0);
        } else {
            QPoint point = m_moduleDirectionAndOrder[startModule][startPort];
            // 左右相同处理 上下相同处理
            int direction = point.x();// 0,1,2,3 左下右上
            int orderIndex = point.y();

            switch (direction) {
                case 0:     // left
                    direction = startHigher ? (fanoutDistance + m_moduleLeftPortNum[startModule] -
                                               orderIndex) : (fanoutDistance + orderIndex);
                    startFanoutPoint = startPoint - QPoint(direction, 0);
                    break;
                case 2:     // right
                    direction = startHigher ? (fanoutDistance + m_moduleLeftPortNum[startModule] -
                                               orderIndex) : (fanoutDistance + orderIndex);
                    startFanoutPoint = startPoint + QPoint(direction, 0);
                    break;
                case 1:     // bottom
                    // Todo something

                    break;
                case 3:     // top
                    // Todo something

                    break;
                default:
                    qDebug() << "Switch Case Error! PlaceAndRoute";
            }
        }
        if (endModule == -1) {
            endFanoutPoint = endPoint - QPoint(fanoutDistance, 0);
        } else {
            QPoint point = m_moduleDirectionAndOrder[endModule][endPort];
            // 左右相同处理 上下相同处理
            int direction = point.x();// 0,1,2,3 左下右上
            int orderIndex = point.y();

            switch (direction) {
                case 0:     // left
                    direction = startHigher ? (fanoutDistance + orderIndex) : (fanoutDistance +
                                                                               m_moduleLeftPortNum[endModule] -
                                                                               orderIndex);
                    endFanoutPoint = endPoint - QPoint(direction, 0);
                    break;
                case 2:     // right
                    direction = startHigher ? (fanoutDistance + orderIndex) : (fanoutDistance +
                                                                               m_moduleLeftPortNum[endModule] -
                                                                               orderIndex);
                    endFanoutPoint = endPoint + QPoint(direction, 0);
                    break;
                case 1:     // bottom
                    // Todo something

                    break;
                case 3:     // top
                    // Todo something

                    break;
                default:
                    qDebug() << "Switch Case Error! PlaceAndRoute";
            }
        }

        //router.addLine(startPoint, startFanoutPoint);
        //router.addLine(endPoint, endFanoutPoint);

        // 从startFanoutPoint到endFanoutPoint
        Pair src = Pair(startFanoutPoint.y(), startFanoutPoint.x());
        Pair dest = Pair(endFanoutPoint.y(), endFanoutPoint.x());

        // from startFanoutPoint --to--> endFanoutPoint
        router.aStarSearch(src, dest);
        if (router.isSuccessFindPaths()) {
            m_paths[i] = router.getPaths();// (y,x)

            for (int j = 0; j < m_paths[i].size(); ++j) {// list(y,x) --> list(x,y)
                QPoint point(m_paths[i][j].y(), m_paths[i][j].x());
                m_paths[i][j] = point;
            }

            // add startPoint and endPoint //  list 存储的是从终点到起点的所有点
            m_paths[i].push_front(endPoint);
            m_paths[i].push_back(startPoint);
            router.addPath(m_paths[i]);// 及时更新被阻挡区域
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

QVector<QPoint> PlaceAndRoute::getModuleSize() {
    return m_moduleSize;
}

QList<WeightAndIndex> PlaceAndRoute::getRoutingOrder() {
    // 确定布线的顺序  对连接关系的权重从小到大进行排序得到的顺序 连接关系的权重定义: max(startPortPos.y(),endPortPos.y())
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
    return list;
}
