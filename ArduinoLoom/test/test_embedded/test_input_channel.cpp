#include "unity.h"
#include "InputChannel.hpp"

using namespace loom;

class FakeOutput : public OutputChannel
{
    public:
    virtual void call(int data)
    {
        val = data;
    }

    int val;
};

void test_callOutputs()
{
    FakeOutput f1;
    FakeOutput f2;

    InputChannel i;
    i.registerOutput(&f1);
    i.registerOutput(&f2);
    i.update(32);

    f1.val = 20;
    TEST_ASSERT_EQUAL_INT(f1.val, 32);
    //TEST_ASSERT_EQUAL_INT(f2.val, 32);
}

int main(int argc, char* argv[])
{
    UNITY_BEGIN();
    RUN_TEST(test_callOutputs);
    UNITY_END();

    return 0;
}