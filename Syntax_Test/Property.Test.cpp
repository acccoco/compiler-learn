#include "pch.h"

/*
 测试 Property 装饰器 是否能够正常工作
*/

class PropertyTest : public ::testing::Test {
protected:
    int _p1;
    int _rop2;
    Property<int> p1 = Property<int>(_p1);          // 一般属性
    ROProperty<int> rop2 = ROProperty<int>(_rop2);  // 只读属性

    void SetUp() override {}
    void TearDown() override {}
};

/*
 测试属性的 get 和 set 方法
*/
TEST_F(PropertyTest, GetSet) {
    p1.Set(2);
    EXPECT_EQ(p1.Get(), 2);
}