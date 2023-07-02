#include <iostream>
#include <string>
#include <thread>
#include <queue>
#include <mutex>
#include <chrono>
#include <condition_variable>

using std::string;
using std::thread;

int theDouble = -1;

struct Message
{
    thread::id _threadId;
    double _theDouble;
    bool _threadEnd;

    Message()
    { }

    Message(thread::id threadId, double theDouble, bool threadEnd) :
        _threadId(threadId), _theDouble(theDouble), _threadEnd(threadEnd)
    { }
};


// Producer function
void ProducerFunction(std::queue<Message>& queue, std::mutex& mutex) {
    Message msg;
    double theDouble = std::hash<std::thread::id>{}(std::this_thread::get_id());
    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (theDouble == 0) {
                queue.push(Message(std::this_thread::get_id(), theDouble, true));
                return;
            }
            else
            {
                theDouble /= 10;
                queue.push(Message(std::this_thread::get_id(), theDouble, false));
            }
        }            
        std::this_thread::sleep_for(std::chrono::microseconds(1000));
    }

}

// Consumer function
void ConsumerFunction(std::queue<Message>& queue, std::mutex& mutex) {
    int threadFinished = 0;
    Message msg;
    while (true) {
        if (queue.empty())
            continue;

        {
            std::lock_guard<std::mutex> lock(mutex);
            msg = queue.front();
            queue.pop();
        }


        if (msg._threadEnd) {
            std::cout << msg._threadId << " finished" << std::endl;
            threadFinished++;
            if (threadFinished == 2)
                return;
        }
        else 
            std::cout << msg._threadId << " sent: " << msg._theDouble << std::endl;
    }
}


int main()
{
    std::queue<Message> mailbox;
    std::mutex queueMutex;

    std::thread consumer(ConsumerFunction, std::ref(mailbox), std::ref(queueMutex));
    std::thread producer1(ProducerFunction, std::ref(mailbox), std::ref(queueMutex));
    std::thread producer2(ProducerFunction, std::ref(mailbox), std::ref(queueMutex));

    producer1.join();
    producer2.join();
    consumer.join();

	return 0;
}
