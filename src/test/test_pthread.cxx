#include "testLibrary.h"

class TestPThread : public TestLibrary
{
public:
   explicit TestPThread();

    void run_tests();

};

TestPThread::TestPThread()
    :TestLibrary()
{

}

void TestPThread::run_tests()
{

}

int main()
{
  TestPThread test;
  test.run_tests();
  return test.get_result();
}

