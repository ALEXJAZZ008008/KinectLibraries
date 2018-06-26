#include "testLibrary.h"

class TestBuffer : public TestLibrary
{
public:
   explicit TestBuffer();

    void run_tests();

};

TestBuffer::TestBuffer()
    :TestLibrary()
{

}

void TestBuffer::run_tests()
{

}

int main()
{
  TestBuffer test;
  test.run_tests();
  return test.get_result();
}
