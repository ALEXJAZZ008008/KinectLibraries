#include "testLibrary.h"
#include <pthread.h>


const int A[3][3] = {
    {6, 1, 1},
    {4, -2, 5},
    {2, 8, 7}
};

const int detA = -306;

const int B[50][50] = {
    {17	,6	,4	,9	,13	,20	,2	,7	,9	,19	,12	,15	,18	,3	,12	,10	,14	,3	,20	,4	,13	,20	,16	,15	,11	,6	,9	,3	,11	,1	,3	,7	,14	,13	,17	,17	,4	,5	,4	,11	,15	,8	,20	,7	,2	,3	,3	,19	,17	,19},
    {19	,14	,16	,1	,8	,11	,14	,15	,2	,1	,11	,8	,12	,4	,18	,3	,3	,19	,4	,14	,8	,2	,14	,13	,10	,11	,14	,17	,13	,9	,6	,13	,9	,5	,12	,8	,4	,17	,16	,7	,18	,12	,12	,13	,17	,12	,18	,11	,4	,9},
    {3	,14	,7	,19	,17	,11	,1	,14	,12	,10	,18	,14	,19	,1	,7	,20	,15	,4	,6	,18	,20	,19	,1	,7	,18	,20	,19	,7	,17	,17	,2	,19	,14	,17	,19	,2	,7	,18	,7	,17	,5	,17	,19	,5	,11	,2	,14	,20	,3	,8},
    {19	,4	,11	,19	,11	,5	,2	,11	,10	,9	,6	,15	,14	,13	,9	,7	,3	,6	,8	,11	,5	,1	,17	,19	,8	,15	,17	,5	,11	,13	,9	,19	,6	,20	,2	,12	,18	,20	,9	,17	,3	,3	,15	,12	,14	,17	,15	,4	,17	,10},
    {13	,3	,4	,10	,8	,10	,11	,14	,14	,10	,7	,9	,12	,6	,2	,6	,3	,16	,2	,15	,14	,14	,19	,3	,9	,17	,10	,7	,5	,11	,6	,12	,1	,17	,12	,19	,10	,10	,4	,12	,5	,17	,10	,13	,5	,15	,14	,3	,13	,6},
    {2	,10	,13	,10	,19	,13	,2	,14	,14	,16	,3	,1	,17	,11	,4	,2	,13	,10	,14	,4	,13	,16	,16	,15	,20	,9	,16	,8	,10	,18	,6	,7	,11	,11	,6	,4	,9	,5	,17	,6	,8	,17	,13	,12	,11	,19	,11	,6	,1	,19},
    {6	,20	,6	,7	,18	,14	,17	,4	,13	,7	,19	,7	,18	,14	,14	,6	,7	,16	,9	,20	,8	,11	,1	,13	,1	,10	,9	,11	,9	,2	,9	,18	,6	,6	,17	,9	,4	,5	,13	,14	,6	,8	,18	,9	,15	,10	,7	,8	,18	,10},
    {11	,7	,14	,19	,12	,8	,17	,3	,1	,16	,13	,9	,20	,10	,7	,1	,14	,8	,20	,11	,3	,18	,8	,17	,20	,12	,20	,12	,20	,19	,3	,9	,19	,15	,4	,15	,20	,11	,15	,5	,19	,9	,4	,1	,20	,14	,14	,9	,11	,6},
    {20	,12	,14	,8	,13	,8	,15	,20	,2	,10	,10	,6	,1	,11	,18	,11	,15	,6	,9	,14	,1	,18	,15	,8	,4	,6	,20	,8	,13	,3	,10	,19	,19	,5	,9	,1	,9	,16	,17	,10	,2	,12	,8	,11	,9	,18	,3	,7	,11	,9},
    {20	,5	,15	,3	,12	,20	,3	,4	,7	,1	,13	,4	,18	,9	,3	,16	,12	,1	,13	,1	,9	,13	,15	,15	,14	,15	,18	,8	,14	,11	,15	,1	,8	,20	,8	,19	,17	,7	,2	,8	,12	,15	,20	,9	,2	,11	,3	,14	,13	,15},
    {4	,16	,10	,16	,5	,1	,14	,1	,11	,4	,11	,17	,13	,3	,20	,13	,15	,14	,4	,17	,4	,3	,5	,17	,12	,11	,8	,11	,15	,3	,5	,11	,1	,13	,17	,16	,13	,10	,20	,11	,4	,15	,9	,3	,2	,6	,1	,2	,16	,9},
    {20	,6	,2	,8	,7	,18	,11	,12	,14	,15	,13	,9	,20	,10	,11	,2	,5	,9	,8	,15	,15	,5	,6	,7	,14	,13	,10	,14	,7	,12	,16	,15	,14	,13	,14	,12	,8	,13	,10	,20	,17	,16	,14	,10	,13	,20	,20	,9	,18	,4},
    {20	,11	,5	,5	,10	,19	,20	,18	,9	,10	,11	,18	,11	,18	,15	,2	,15	,10	,4	,3	,8	,4	,14	,12	,8	,7	,5	,20	,11	,1	,2	,4	,17	,4	,5	,4	,18	,19	,16	,16	,4	,8	,19	,10	,16	,1	,20	,6	,8	,18},
    {10	,14	,19	,9	,5	,16	,13	,14	,17	,4	,15	,8	,10	,18	,20	,16	,20	,13	,16	,11	,17	,1	,10	,20	,13	,3	,16	,15	,12	,16	,8	,7	,20	,2	,7	,10	,16	,4	,15	,20	,11	,9	,20	,12	,14	,7	,3	,7	,2	,12},
    {17	,18	,4	,2	,17	,2	,17	,4	,15	,7	,11	,16	,17	,6	,6	,19	,18	,2	,18	,7	,15	,3	,13	,11	,17	,10	,18	,9	,4	,17	,1	,4	,2	,6	,3	,11	,10	,15	,17	,5	,20	,20	,14	,17	,7	,20	,10	,3	,15	,8},
    {3	,20	,17	,3	,4	,6	,10	,8	,20	,13	,20	,8	,5	,5	,9	,11	,2	,7	,8	,11	,12	,13	,5	,7	,1	,8	,19	,17	,12	,19	,5	,7	,10	,18	,14	,20	,17	,12	,4	,11	,8	,12	,3	,15	,19	,8	,14	,12	,7	,5},
    {9	,11	,11	,19	,5	,7	,9	,10	,11	,4	,5	,17	,10	,12	,10	,3	,8	,16	,14	,8	,4	,19	,4	,13	,2	,16	,12	,3	,14	,20	,1	,9	,12	,19	,12	,18	,18	,9	,10	,2	,1	,17	,1	,18	,11	,7	,6	,6	,17	,5},
    {19	,3	,20	,20	,4	,14	,17	,20	,7	,15	,3	,16	,19	,13	,16	,17	,8	,14	,6	,9	,20	,8	,17	,8	,20	,16	,12	,2	,9	,11	,4	,11	,14	,14	,4	,20	,9	,18	,13	,16	,5	,6	,13	,2	,20	,3	,16	,4	,8	,11},
    {16	,3	,2	,12	,5	,3	,2	,4	,3	,5	,3	,8	,12	,9	,17	,7	,14	,3	,11	,4	,6	,9	,16	,16	,14	,14	,3	,2	,17	,6	,3	,1	,15	,15	,3	,14	,7	,8	,19	,13	,8	,13	,12	,5	,2	,19	,12	,1	,17	,9},
    {20	,6	,9	,2	,9	,15	,3	,18	,13	,19	,2	,5	,17	,5	,3	,6	,12	,3	,17	,6	,19	,20	,19	,9	,5	,3	,18	,4	,15	,3	,6	,12	,14	,5	,10	,9	,12	,4	,17	,18	,7	,12	,20	,10	,5	,3	,9	,6	,4	,15},
    {14	,17	,3	,5	,7	,3	,4	,13	,16	,6	,9	,16	,15	,19	,4	,15	,16	,2	,12	,1	,5	,19	,3	,10	,9	,1	,10	,7	,8	,11	,4	,6	,15	,12	,19	,19	,19	,13	,18	,20	,5	,20	,15	,20	,16	,7	,6	,12	,3	,2},
    {1	,6	,20	,8	,19	,14	,8	,8	,9	,16	,9	,19	,12	,2	,8	,1	,8	,1	,7	,19	,8	,14	,4	,14	,3	,12	,5	,7	,10	,12	,3	,5	,8	,17	,12	,10	,15	,13	,12	,19	,19	,2	,14	,16	,19	,18	,16	,18	,18	,17},
    {17	,17	,1	,17	,9	,10	,17	,4	,2	,4	,8	,7	,5	,3	,2	,1	,5	,9	,6	,14	,2	,20	,2	,20	,6	,7	,18	,1	,8	,16	,12	,5	,12	,9	,1	,5	,8	,7	,12	,9	,14	,11	,11	,10	,16	,10	,18	,1	,1	,14},
    {19	,5	,16	,1	,4	,16	,17	,9	,6	,6	,16	,14	,14	,3	,11	,14	,2	,14	,10	,19	,13	,16	,10	,7	,6	,19	,16	,11	,16	,2	,19	,4	,3	,20	,2	,8	,15	,17	,18	,1	,20	,11	,6	,7	,6	,13	,15	,19	,14	,3},
    {14	,19	,17	,1	,19	,15	,2	,10	,4	,2	,13	,9	,2	,4	,7	,13	,16	,15	,9	,4	,4	,7	,4	,17	,7	,20	,18	,2	,15	,14	,19	,20	,2	,2	,17	,15	,20	,20	,1	,11	,9	,2	,20	,2	,4	,12	,9	,3	,15	,18},
    {16	,7	,18	,4	,20	,19	,8	,3	,6	,12	,16	,17	,13	,13	,4	,11	,5	,11	,8	,19	,1	,14	,18	,15	,4	,6	,6	,3	,9	,11	,5	,19	,20	,7	,10	,12	,11	,20	,18	,5	,19	,19	,11	,15	,17	,14	,19	,17	,9	,4},
    {15	,4	,2	,13	,9	,18	,11	,12	,9	,14	,19	,16	,14	,12	,5	,15	,8	,3	,12	,16	,15	,5	,2	,20	,7	,17	,14	,13	,14	,4	,10	,17	,6	,11	,8	,16	,11	,3	,9	,5	,1	,18	,1	,11	,16	,1	,6	,17	,8	,13},
    {8	,6	,8	,15	,3	,7	,9	,5	,11	,11	,20	,4	,15	,2	,19	,15	,12	,13	,15	,12	,7	,6	,1	,1	,3	,18	,14	,18	,19	,19	,8	,15	,12	,2	,16	,20	,7	,5	,1	,7	,13	,9	,14	,4	,6	,11	,11	,19	,20	,11},
    {14	,13	,6	,13	,6	,14	,14	,8	,10	,9	,4	,18	,18	,19	,14	,16	,5	,3	,9	,9	,14	,14	,12	,8	,18	,12	,3	,20	,16	,12	,11	,4	,20	,15	,8	,20	,2	,1	,15	,2	,17	,16	,11	,9	,5	,1	,20	,3	,8	,4},
    {4	,10	,17	,10	,9	,4	,13	,12	,18	,13	,3	,20	,20	,15	,10	,6	,13	,3	,9	,6	,8	,11	,16	,14	,2	,18	,9	,12	,15	,9	,6	,8	,4	,12	,11	,11	,4	,13	,4	,15	,5	,3	,2	,4	,7	,17	,6	,11	,9	,1},
    {15	,8	,9	,11	,12	,1	,6	,6	,11	,13	,14	,11	,16	,15	,19	,14	,10	,2	,3	,16	,13	,9	,7	,6	,19	,19	,6	,20	,3	,19	,2	,4	,4	,11	,15	,20	,2	,3	,3	,15	,19	,13	,18	,16	,17	,7	,6	,9	,4	,16},
    {1	,17	,19	,6	,6	,15	,9	,6	,19	,14	,2	,18	,12	,2	,3	,12	,4	,3	,1	,5	,1	,13	,4	,5	,8	,11	,15	,12	,8	,14	,9	,1	,7	,17	,18	,3	,10	,9	,13	,11	,16	,6	,7	,8	,17	,17	,19	,4	,7	,16},
    {6	,12	,4	,15	,13	,11	,1	,13	,13	,13	,11	,12	,19	,18	,15	,8	,16	,4	,6	,2	,19	,16	,7	,15	,1	,15	,6	,11	,12	,10	,4	,7	,19	,18	,7	,2	,1	,18	,6	,7	,17	,9	,5	,19	,12	,5	,2	,12	,7	,9},
    {1	,11	,6	,4	,15	,10	,20	,6	,20	,19	,11	,4	,12	,19	,15	,2	,3	,4	,7	,16	,17	,12	,5	,13	,7	,12	,18	,7	,10	,17	,1	,14	,8	,16	,14	,7	,19	,11	,7	,17	,12	,17	,3	,1	,12	,12	,6	,13	,18	,2},
    {2	,19	,3	,14	,5	,19	,4	,17	,5	,5	,18	,4	,1	,20	,12	,16	,6	,7	,14	,14	,15	,12	,11	,12	,15	,1	,17	,9	,2	,11	,20	,13	,6	,7	,20	,12	,13	,8	,9	,12	,16	,4	,7	,17	,6	,19	,12	,5	,5	,12},
    {17	,6	,3	,4	,3	,13	,3	,20	,14	,15	,10	,9	,3	,18	,4	,7	,5	,7	,20	,15	,17	,12	,19	,14	,16	,9	,8	,10	,5	,12	,9	,11	,4	,10	,2	,11	,1	,5	,9	,20	,7	,7	,5	,13	,14	,1	,5	,11	,7	,4},
    {14	,16	,18	,8	,6	,13	,8	,15	,6	,5	,8	,15	,18	,16	,12	,13	,11	,5	,19	,13	,8	,11	,13	,1	,11	,13	,10	,2	,17	,14	,20	,9	,8	,16	,12	,19	,1	,9	,8	,18	,5	,10	,14	,11	,16	,2	,13	,20	,9	,15},
    {7	,16	,12	,13	,7	,18	,4	,7	,14	,3	,14	,17	,10	,11	,6	,15	,2	,6	,10	,9	,13	,2	,3	,7	,14	,11	,14	,18	,1	,8	,16	,6	,8	,3	,9	,11	,5	,20	,13	,8	,7	,7	,2	,14	,9	,1	,16	,10	,15	,11},
    {20	,8	,11	,16	,9	,17	,10	,12	,14	,13	,15	,16	,17	,4	,3	,3	,9	,18	,5	,8	,12	,15	,8	,10	,18	,8	,17	,2	,18	,5	,1	,11	,3	,3	,7	,9	,10	,3	,4	,11	,12	,16	,6	,15	,9	,14	,11	,14	,3	,6},
    {1	,12	,3	,2	,11	,12	,7	,3	,2	,10	,11	,7	,5	,8	,5	,3	,3	,15	,16	,17	,11	,20	,2	,5	,2	,19	,13	,9	,2	,12	,14	,16	,9	,6	,6	,11	,3	,10	,4	,7	,17	,20	,6	,2	,10	,12	,14	,9	,18	,19},
    {9	,2	,18	,19	,2	,4	,20	,19	,6	,10	,7	,11	,12	,3	,18	,11	,3	,12	,16	,7	,6	,8	,11	,15	,7	,17	,12	,17	,14	,18	,15	,16	,2	,11	,16	,15	,1	,18	,2	,13	,6	,4	,18	,18	,6	,3	,16	,1	,2	,16},
    {8	,2	,13	,16	,6	,5	,19	,18	,5	,14	,3	,2	,13	,1	,2	,10	,16	,4	,15	,17	,5	,20	,9	,18	,1	,17	,7	,8	,11	,9	,13	,12	,13	,20	,20	,1	,15	,1	,7	,16	,9	,5	,9	,1	,14	,16	,5	,6	,10	,18},
    {16	,11	,8	,10	,17	,18	,2	,17	,14	,16	,12	,3	,1	,19	,5	,18	,6	,5	,15	,16	,10	,7	,20	,6	,4	,8	,10	,13	,5	,3	,12	,15	,1	,15	,4	,17	,8	,14	,16	,15	,18	,15	,16	,6	,19	,13	,13	,17	,1	,2},
    {16	,16	,11	,9	,1	,1	,15	,6	,17	,8	,6	,3	,13	,7	,2	,16	,13	,2	,3	,18	,5	,18	,17	,15	,15	,12	,15	,17	,12	,9	,5	,13	,12	,7	,16	,3	,16	,20	,5	,3	,13	,8	,13	,4	,19	,2	,3	,7	,16	,4},
    {4	,19	,9	,9	,19	,10	,6	,12	,7	,14	,1	,14	,8	,6	,9	,15	,20	,19	,14	,11	,17	,10	,10	,3	,15	,18	,18	,18	,3	,7	,16	,3	,16	,6	,4	,10	,9	,6	,15	,17	,20	,20	,16	,19	,15	,2	,11	,2	,15	,15},
    {10	,3	,2	,7	,15	,4	,9	,1	,16	,9	,16	,10	,1	,7	,1	,2	,9	,15	,10	,13	,20	,9	,18	,17	,18	,19	,15	,19	,14	,9	,5	,11	,5	,18	,20	,6	,9	,3	,14	,1	,5	,20	,3	,2	,6	,3	,17	,11	,5	,14},
    {9	,12	,5	,11	,10	,20	,11	,9	,14	,17	,5	,4	,10	,10	,18	,2	,14	,12	,5	,20	,1	,5	,3	,3	,12	,14	,1	,4	,12	,17	,8	,7	,9	,19	,17	,8	,1	,14	,17	,9	,17	,13	,20	,12	,14	,16	,19	,8	,14	,16},
    {13	,10	,3	,11	,12	,15	,19	,7	,1	,17	,9	,10	,4	,13	,4	,2	,16	,7	,2	,9	,11	,3	,8	,12	,2	,5	,14	,6	,2	,9	,18	,2	,12	,13	,9	,14	,1	,19	,17	,15	,14	,18	,17	,13	,3	,2	,10	,15	,12	,11},
    {15	,1	,4	,17	,5	,11	,9	,4	,13	,6	,14	,3	,3	,1	,2	,16	,9	,4	,17	,2	,2	,7	,19	,8	,19	,14	,9	,18	,2	,8	,18	,3	,2	,6	,15	,4	,2	,13	,6	,16	,5	,9	,2	,14	,3	,5	,6	,11	,18	,9},
    {16	,7	,5	,16	,10	,10	,20	,4	,8	,13	,8	,2	,5	,17	,7	,19	,14	,13	,4	,18	,17	,15	,19	,17	,17	,2	,9	,12	,4	,8	,9	,4	,10	,2	,10	,6	,12	,18	,7	,8	,10	,13	,10	,18	,4	,5	,14	,17	,12	,13}};

    const double detB =-6.196548805599823e+70;

class TestPThread : public TestLibrary
{
public:
   explicit TestPThread();

    void run_tests();

private:

    pthread_t* my_thread;

    int get_determinant();

};

TestPThread::TestPThread()
    :TestLibrary()
{

}

TestPThread::get_determinant()
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
