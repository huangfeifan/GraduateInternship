//
// Created by Huangff on 2021/7/9.
//

#include "stdlib.h"
#include <iostream>
#include <string>
#include <vector>

struct Stu {
    //定义学生结构体
    int id = 0;//学生id 10位数字
    std::string name;//字符串
    int score[5];//五科成绩数组
    float averageScore = 0.0;//平均成绩
};


void initVector(std::vector<Stu> &class1, int index) {
    //初始化数组 设置数组长度45
    //数据初始化
    for (int j = 0; j < 45; ++j) {
        Stu student;
        student.score[0] = 0;
        student.score[1] = 1;
        student.score[2] = 2;
        student.score[3] = 3;
        student.score[4] = 4;
        student.name = "studentNumber ";
        student.id = 1000000000 + j + 45 * index;
        student.averageScore = 10 / 5.0;
        class1.push_back(student);
    }
}

void generateScores(std::vector<Stu> &class1) {
    //随机生成0-100内的整数  顺便更新平均成绩
    for (int i = 0; i < 45; ++i) {
        float sum = 0.0;
        for (int j = 0; j < 5; ++j) {
            //随机数生成  (rand() % (100))
            class1.at(i).score[j] = rand() % (100);
            sum += class1.at(i).score[j];
        }
        class1.at(i).averageScore = sum / 5;
    }
}

void generateAverageScore(std::vector<Stu> &class1) {
    //计算平均数
    float average;
    int sum = 0;
    for (int i = 0; i < 45; ++i) {
        sum = 0;
        sum += class1.at(i).score[0];
        sum += class1.at(i).score[1];
        sum += class1.at(i).score[2];
        sum += class1.at(i).score[3];
        sum += class1.at(i).score[4];
        average = sum * 1.0 / 5;
/*        std::cout<<"学号:";
        std::cout<<class1.at(i).id;
        std::cout<<"  姓名:";
        std::cout<< class1.at(i).name<<"  平均成绩:"<<average<<std::endl;*/
        class1.at(i).averageScore = average;
    }

}

void bubbleSort(std::vector<Stu> &class1, int length) {
    //冒泡排序
    for (int i = 0; i < length; ++i) {
        for (int j = i + 1; j < length; ++j) {
            if (class1.at(i).averageScore < class1.at(j).averageScore) {
                Stu stu = class1.at(j);
                class1.at(j) = class1.at(i);;
                class1.at(i) = stu;
            }
        }
    }
}

void selectionSort(std::vector<Stu> &class1) {
    //选择排序
    int size = 45;
    int minIndex;
    for (int i = 0; i < (size - 1); i++) {
        minIndex = i;
        Stu stu = class1.at(i);
        for (int index = i + 1; index < size; index++) {
            if (class1.at(index).averageScore > stu.averageScore) {
                stu = class1.at(index);
                minIndex = index;
            }
        }
        class1.at(minIndex) = class1.at(i);
        class1.at(i) = stu;
    }

}

struct Node {
    Stu student;
    struct Node *next;
};

void printClassInfo(std::vector<Stu> &class1, int length) {
    for (int i = 0; i < length; ++i) {
        std::cout << "学号:";
        std::cout << class1.at(i).id;
        std::cout << " 姓名:";
        std::cout << class1.at(i).name;
        std::cout << " 平均成绩:";
        std::cout << class1.at(i).averageScore;
        std::cout << " 学科1成绩:";
        std::cout << class1.at(i).score[0];
        std::cout << " 学科2成绩:";
        std::cout << class1.at(i).score[1];
        std::cout << " 学科3成绩:";
        std::cout << class1.at(i).score[2];
        std::cout << " 学科4成绩:";
        std::cout << class1.at(i).score[3];
        std::cout << " 学科5成绩:";
        std::cout << class1.at(i).score[4];
        std::cout << std::endl;
    }
}

void creatNode(std::vector<Stu> &class1, std::vector<Stu> &class2, std::vector<Stu> &class3, std::vector<Stu> &class4) {
    //将所有班级学生信息放到一个数组
    std::vector<Stu> allClassStu;
    for (int i = 0; i < 45; ++i) {
        allClassStu.push_back(class1.at(i));
    }
    for (int i = 0; i < 45; ++i) {
        allClassStu.push_back(class2.at(i));
    }
    for (int i = 0; i < 45; ++i) {
        allClassStu.push_back(class3.at(i));
    }
    for (int i = 0; i < 45; ++i) {
        allClassStu.push_back(class4.at(i));
    }

    //将四个班级的学生按照平均成绩排序
    bubbleSort(allClassStu, 45 * 4);
    //printClassInfo(allClassStu, 45 * 4);

    //将数据加到单链表
    //存放节点的数组
    std::vector<Node> allNode;
    for (int i = 0; i < 45 * 4; ++i) {
        Node item;
        //给item添加学生信息
        item.student = allClassStu.at(i);
        //设置空指针
        item.next = nullptr;
        allNode.push_back(item);
    }

    //头指针 指向第一个学生节点
    Node *head = new Node;
    head = &allNode.at(0);

    //临时指针 用来链接其余对象
    Node *p = head;
    for (int i = 1; i < 45 * 4; ++i) {
        p->next = &allNode.at(i);
        p = p->next;
    }

    std::cout << "  Node Data " << std::endl;
    //根据单链表的头节点 打印单链表的数据
    Node *temp = head;

    //指针非空则打印节点信息
    while (temp) {
        //打印节点信息
        std::cout << "学号:" << temp->student.id << " 姓名:" << temp->student.name << " 平均成绩:"
                  << temp->student.averageScore << "  学科1成绩:" << temp->student.score[0] << "  学科2成绩:"
                  << temp->student.score[1] << "  学科3成绩:" << temp->student.score[2] << "  学科4成绩:"
                  << temp->student.score[3] << "  学科5成绩:" << temp->student.score[4] << std::endl;
        //指针后移
        temp = temp->next;
    }

    //除头指针外的其余指针删除
    temp = nullptr;
    p = nullptr;
    delete (temp);
    delete (p);
}

/*
 *
 *     std::cout << "************************************成绩排序******************************************" << endl;

    //四个数组 存放每个班级学生的相关信息 学号 姓名 五科的成绩
    vector<Stu> classNumber1;
    vector<Stu> classNumber2;
    vector<Stu> classNumber3;
    vector<Stu> classNumber4;

    //四个数组初始化
    initVector(classNumber1, 0);
    initVector(classNumber2, 1);
    initVector(classNumber3, 2);
    initVector(classNumber4, 3);

    //随机生成成绩
    generateScores(classNumber1);
    generateScores(classNumber2);
    generateScores(classNumber3);
    generateScores(classNumber4);

    //功能提醒
    cout << "成绩随机生成且打印             请输入1 : " << endl;
    cout << "平均成绩计算且打印             请输入2 : " << endl;
    cout << "在各个班级内根据成绩使用冒泡排序  请输入3 : " << endl;
    cout << "在各个班级内根据成绩使用选择排序  请输入4 : " << endl;
    cout << "根据成绩构造单链表             请输入5 : " << endl << endl;

    float base = 0;
    cin >> base;
    if (base == 1) {

        //打印初始化的学生信息
        std::cout << "------1班随机成绩*******" << std::endl << endl;
        printClassInfo(classNumber1, 45);
        std::cout << "------2班随机成绩*******" << std::endl << endl;
        printClassInfo(classNumber2, 45);
        std::cout << "------3班随机成绩*******" << std::endl << endl;
        printClassInfo(classNumber3, 45);
        std::cout << "------4班随机成绩*******" << std::endl << endl;
        printClassInfo(classNumber4, 45);
    } else if (base == 2) {

        std::cout << "----class1---------------------------------------------------------------------------" << endl;
        generateAverageScore(classNumber1);
        std::cout << "----class1  END---------------------------------------------------------------------------"
                  << endl;

        std::cout << "----class2---------------------------------------------------------------------------" << endl;
        generateAverageScore(classNumber2);
        std::cout << "----class2  END---------------------------------------------------------------------------"
                  << endl;

        std::cout << "----class3---------------------------------------------------------------------------" << endl;
        generateAverageScore(classNumber3);
        std::cout << "----class3  END---------------------------------------------------------------------------"
                  << endl;

        std::cout << "----class4---------------------------------------------------------------------------" << endl;
        generateAverageScore(classNumber4);
        std::cout << "----class4  END---------------------------------------------------------------------------"
                  << endl;

    } else if (base == 3) {
        //对每个班级的平均成绩使用冒泡排序 实现降序
        bubbleSort(classNumber1, 45);
        bubbleSort(classNumber2, 45);
        bubbleSort(classNumber3, 45);
        bubbleSort(classNumber4, 45);

        //打印排序后的学生信息
        std::cout << "------1班平均成绩降序成绩*******" << std::endl;
        printClassInfo(classNumber1, 45);
        std::cout << "------2班平均成绩降序成绩*******" << std::endl;
        printClassInfo(classNumber2, 45);
        std::cout << "------3班平均成绩降序成绩*******" << std::endl;
        printClassInfo(classNumber3, 45);
        std::cout << "------4班平均成绩降序成绩*******" << std::endl;
        printClassInfo(classNumber4, 45);

    } else if (base == 4) {
        //对每个班级的平均成绩使用选择排序 实现降序
        selectionSort(classNumber1);
        selectionSort(classNumber2);
        selectionSort(classNumber3);
        selectionSort(classNumber4);

        //打印排序后的学生信息
        std::cout << "------1班平均成绩降序成绩*******" << std::endl;
        printClassInfo(classNumber1, 45);
        std::cout << "------2班平均成绩降序成绩*******" << std::endl;
        printClassInfo(classNumber2, 45);
        std::cout << "------3班平均成绩降序成绩*******" << std::endl;
        printClassInfo(classNumber3, 45);
        std::cout << "------4班平均成绩降序成绩*******" << std::endl;
        printClassInfo(classNumber4, 45);

    } else if (base == 5) {
        creatNode(classNumber1, classNumber2, classNumber3, classNumber4);

    } else {
        std::cout << "输入错误请重新输入!";
    }
    std::cout << endl;
    std::cout << "********************************成绩排序END******************************************" << endl;*/