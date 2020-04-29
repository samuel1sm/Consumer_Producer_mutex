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

    Semaphore(int count, int count_int)
    {
        this->count = count_int;
        this->count_max = count;
    }

    void up() {
        
        unique_lock<mutex> lc(mtx);

        if (count < count_max) {
            //cout <<"aaaa" << count << endl;

            count++;
            //cout << count << " - " << count_max << endl;

            //cout << "bbbbb" << count << endl;

            //cout << "thread " << tid << " released" << endl;
            cv.notify_one();

        }
  
    }

    void up(int tid) {

        unique_lock<mutex> lc(mtx);
        if (count < count_max) {
            //cout << "a: " << count << endl;

            count++;
            //cout <<"b: " << count << endl;

            //cout << "thread " << tid << " released" << endl;
            cv.notify_one();
        }
    }

    void down() {
        unique_lock<mutex> lc(mtx);

        if (count == 0) {
            cv.wait(lc);
            //cout << "thread " << tid << " run" << endl;
        }
        count--;
    }

    void down(int tid) {
        //cout << count << endl;
        unique_lock<mutex> lc(mtx);
        if (count == 0) {
            cv.wait(lc);
        }
        count--;
    }

    void setCountMax() {
        count = count_max;
    }

    void setCountMin() {
        count = 0;
    }
private:
    mutex mtx;
    condition_variable cv;
    int count;
    int count_max;

};