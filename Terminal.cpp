#include <thread>    
#include <mutex> 
#include <iostream>
#include <chrono>  

using namespace std;

mutex mtx;
int arr[30];
int arr_size = 30;
int current_size = 0;

void prt_arr(char a, int num) {
    for (int i = 0; i < arr_size; i++ ){
        cout << arr[i] << " "; 
    }
    
    cout <<"  s: " <<current_size<< " m: " << a << " " << num << endl;
    
}

void producer( ) {
    int  item;
    mtx.lock();
    bool aux = false;
    while (true) {
        if (aux) {
            mtx.lock();
            aux = false;
        }
           

        if (current_size != arr_size) {
            this_thread::sleep_for(chrono::seconds(1));
            item = rand() % 8 + 1;


            arr[current_size] = item;
            current_size++;
            //cout << "a: " << item << endl;
            prt_arr('a', item);
        }
        else  {
            mtx.unlock();
            //mtx.lock();
            aux = true;

        }



    }


}

void consumer( ) {
    int item;
    bool aux = false;
    mtx.lock();

    while (true)
    {
        if (aux) {
            mtx.lock();
            aux = false;
        }

        if (current_size > 0) {
            this_thread::sleep_for(chrono::seconds(1));

            item = arr[current_size - 1];
            arr[current_size - 1] = 0;
            current_size--;

            prt_arr('r', item);
            

            //cout << "r: " << item << endl;

            //if (current_size == arr_size - 2) {
            //    mtx.unlock();
            //    //mtx.lock();

            //}
        }  else {
          

            mtx.unlock();
            aux = true;

            //mtx.lock();

            //cout << "alou 1" << endl;

        }


    }
}

void print_block(int n, char c) {
    // critical section (exclusive access to std::cout signaled by locking mtx):
    while (true)
    {

        mtx.lock();
        //this_thread::sleep_for(chrono::seconds(1));

        for (int i = 0; i < n; ++i) {

            std::cout << c; }
        std::cout << '\n';
        mtx.unlock();
    }

}

void print_block2(int n, char c) {
    // critical section (exclusive access to std::cout signaled by locking mtx):
    while (true)
    {
        mtx.lock();
        //this_thread::sleep_for(chrono::seconds(1));

        for (int i = 0; i < n; ++i) { 

            std::cout << c; }
        std::cout << '\n';
        mtx.unlock();
    }
 
}


int main() {

    thread th1(producer);
    thread th2(consumer);

    th1.join();
    th2.join();

    return 0;
}

