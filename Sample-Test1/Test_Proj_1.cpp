#include "pch.h"
#include"../Proj_1/MyClass.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(MyClass, Calculate) {
	MyClass my;
	EXPECT_EQ(15, my.Calculate(3.0,'*',5.0));
	EXPECT_TRUE(true);
}