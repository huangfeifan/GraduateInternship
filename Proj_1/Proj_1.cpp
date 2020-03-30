// Proj_1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "pch.h"
#include"../Proj_1/MyClass.h"
#include"Sales_item.h"
using namespace std;

int main()
{
	
	cout << "Hello world!" << endl;
		/*缴费年限 n 
	每年缴费金额 mon 
	收益率5%
	领取年金的年数	year 从第五年开始领取 
	领取金额	n_mon 
	*/ 
	
	//按照收益率5%计算到领取结束 
	double mon=10000,year=40,n_mon=3000;
	double sum_ann=0.0;
	int i=1;
	while(i<=10){
		cout<<"sum_ann "<<endl;
		sum_ann=1+sum_ann;
		i++;
		//mon*1.05*(year-5+i);
	}
	cout<<"sum_ann="<<sum_ann;
	
	//总领取的年金	annuity
	
	return 0;

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
