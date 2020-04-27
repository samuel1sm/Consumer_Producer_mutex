#include <thread>    
#include <mutex> 
#include <iostream>
#include <chrono> 
#include <list>
#include <vector>
#include "Semaphore.cpp"
using namespace std;

struct thread_info {
    int init;
    int fin;
    bool endend;
};


int arr_size = 10;
int arr[10];
int current_size = 0;

vector<thread_info> producer_running_threads_info;
vector<thread_info> consumer_running_threads_info;

Semaphore* producer_semaphore;
Semaphore* consumer_semaphore;

void reset_threads(bool key_consumer) {
    if (key_consumer) {
        for (int i = 0; i < consumer_running_threads_info.size();i++) {
            
            consumer_running_threads_info[i].endend = false;

        }
    }
    else {
        for (int i = 0; i < producer_running_threads_info.size(); i++) {
            producer_running_threads_info[i].endend = false;
        }
    }


}

bool verify_running_threads_info(bool key_consumer) {

    vector<thread_info> vec = (key_consumer) ? consumer_running_threads_info : producer_running_threads_info;
    bool result = true;
    for (int i = 0; i < vec.size(); i++) {
        result = result && vec[i].endend;

        if (!result)
            break;
    }
 
    return result;
}

void prt_arr() {
 
    while (true) {
        cout << "[";

        for (int i = 0; i < arr_size; i++) {
            if (arr[i] == 0) {
                cout << "_";

            }
            else {
                cout << arr[i];
            }


            if (i != arr_size - 1) {
                cout << ", ";
            }
        }

        cout << "]" << endl;

        this_thread::sleep_for(chrono::seconds(1));
    }
    
}

void producer(int position) {
    int  item;

    bool aux = false;
    producer_semaphore->lock(position);
    int positions = position;
    
    int position_insert = producer_running_threads_info[position].init;
    int last_position = producer_running_threads_info[position].fin;


    int actual_position = position_insert;
    while (true) {
        if (aux){
            producer_semaphore->unlock(position);

            while (producer_running_threads_info[position].endend)
            {
                if (verify_running_threads_info(false)) {
                    producer_semaphore->unlock(position);
                }
                producer_semaphore->lock(position);

            }

            actual_position = position_insert;

            aux = false;

        }

        int size = current_size;
        if (actual_position < last_position) {
            this_thread::sleep_for(chrono::seconds(1));
            item = rand() % 8 + 1;

            arr[actual_position] = item;
            current_size++;
            actual_position++;
        }
        else {
            producer_running_threads_info[position].endend = true;
            //mtx.unlock();
            aux = true;

            if (verify_running_threads_info(false))
                reset_threads(true);

            //a->unlock(position);
            producer_semaphore->unlock(position);

        }   
    }
}

void consumer(int position) {
    int item;
    bool aux = true;

    int position_insert = consumer_running_threads_info[position].init;
    int last_position = consumer_running_threads_info[position].fin;
    int actual_position = last_position;
    while (true)
    {
        if (aux) {
            //a->lock(position - consumer_running_threads_info.size());
            producer_semaphore->unlock(position);

            while (consumer_running_threads_info[position].endend) {

                if (verify_running_threads_info(true)) {
                    producer_semaphore->unlock(position);
                }
                producer_semaphore->lock(position);
                //a->unlock(position);

                //cout << "a: " << consumer_running_threads_info[position].endend << endl;
                //mtx.lock();
            }
            actual_position = last_position;
            aux = false;
        }
       

        int size = current_size;


        if (actual_position >  position_insert) {
            this_thread::sleep_for(chrono::seconds(1));
   
            item = arr[actual_position-1];
            arr[actual_position-1] = 0;
            current_size--;
            actual_position--;            
        }  
        else {
          
            consumer_running_threads_info[position].endend = true;
            aux = true;

            if (verify_running_threads_info(true))
                reset_threads(false);

            producer_semaphore->unlock(position);

        }
    }
}

void fill_thread_info_vector(int theads_qtd, bool is_consumer) {
    int value = ceil((float)arr_size / theads_qtd);
 
    for (int i = 0; i < theads_qtd; i++) {
        thread_info new_t;
        if (i == 0)
            new_t.init = i;
        else
            if (is_consumer)
                new_t.init = consumer_running_threads_info[i - 1].init + value;
            else
                new_t.init = producer_running_threads_info[i - 1].init + value;

        if (new_t.init> arr_size)
            new_t.endend = true;
        else
            if (is_consumer)
                new_t.endend = true;
            else
                new_t.endend = false;


        if (new_t.init + value > arr_size)
            new_t.fin = arr_size;
        else
            new_t.fin = new_t.init + value;

         if (is_consumer)
           consumer_running_threads_info.push_back(new_t);
        else
           producer_running_threads_info.push_back(new_t);
    }
}

int main() {
    int qtd_producer = 3;
    int qtd_consumer = 1;

    producer_semaphore = new Semaphore(qtd_producer);
    consumer_semaphore = new Semaphore(qtd_consumer);

    fill_thread_info_vector(qtd_producer,false);
    fill_thread_info_vector(qtd_consumer, true);

    vector<thread> threads;

    for (int i = 0; i < qtd_producer; i++)
        threads.push_back(thread(producer, i));


    for (int i = 0; i < qtd_consumer; i++)
        threads.push_back(thread(consumer, i));


    thread prt_t(prt_arr);
    prt_t.join();

    for (int i = 0; i < qtd_producer + qtd_consumer; i ++)
        threads[i].join();

 

    return 0;
}

