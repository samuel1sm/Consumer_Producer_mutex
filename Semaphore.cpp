#include <mutex>
#include <condition_variable>
#include <iostream>

using namespace std;
class Semaphore {
public:
    Semaphore(int count)
    {
        this->count = count;
        this->count_max = count;
    }

    void unlock(int tid) {
        
        unique_lock<mutex> lc(mtx);
      
            if (count < count_max) {
                count++;
                //cout << "thread " << tid << " released" << endl;
                cv.notify_one();
            }
  
    }
    void lock(int tid) {
        unique_lock<mutex> lc(mtx);

        if (count == 0) {
            cv.wait(lc);
            //cout << "thread " << tid << " run" << endl;
        }
        count--;
    }
private:
    mutex mtx;
    condition_variable cv;
    int count;
    int count_max;

};