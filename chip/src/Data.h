﻿//
// Created by Huangff on 2021/12/17.
//

#pragma once


static QList<QList<int>> graphData = {


/*
        {1, 5},//0
        {2},//1
        {3, 4},//2
        {1},//3
        {},//4
        {6},//5
        {0}//6
*/

/*        {4},//0
        {0, 2},//1
        {1, 3},//2
        {2},//3
        {1},//4
        {4, 6},//5
        {5, 2},//6
        {7, 6, 3}//7*/

        // test data link https://segmentfault.com/a/1190000039149539

/*        {1, 2},//0
        {3},//1
        {3, 4},//2
        {0, 5},//3
        {5},//4
        {},//5*/

        // 有向无环图 DAG directed acyclic graph
/*        {1, 5},//0
        {},//1
        {3, 4},//2
        {1},//3
        {},//4
        {6},//5
        {}//6*/


/*        {},//0
        {},//1
        {3, 4},//2
        {1},//3
        {},//4
        {6},//5
        {4}//6*/

/*        {1, 5},//0
        {},//1
        {3, 4},//2
        {1},//3
        {},//4
        {6},//5
        {}//6*/

/*        {1, 2},//0
        {3},//1
        {3, 4},//2
        {0, 5},//3
        {5},//4
        {},//5*/


        {1, 2},//0
        {3},//1
        {3, 4},//2
        {0, 5},//3
        {5},//4
        {},//5


/*        {1},//0
        {2},//1
        {3,4},//2  {3，4}一个强连通分支 {3}多个强连通分支
        {1},//3
        {5,6},//4
        {2,7},//5
        {7},//6
        {0},//7*/

/*        {1},//0
        {2,3},//1
        {1,3},//2
        {4,0},//3
        {3},//4*/

};
