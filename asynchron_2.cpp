#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include <vector>
#include <functional>
#include <future>
#include <cassert>

enum class EventCode {
    add,
    quit
};

struct Context{
};

struct ContextAdd: public Context{
    ContextAdd(int &r,bool &d): result(r),done(d) {};
    int   &result;
    bool  &done;
};

struct Data{
};

struct DataAdd : public Data{
    DataAdd(int l,int r) : lhv(l), rhv(r) {};
    int lhv;
    int rhv;
};

struct Event {
    EventCode code;
    std::shared_ptr<Data>    data;
    std::shared_ptr<Context> context;
    std::function<void(std::shared_ptr<Context>)> callback;
};


class Handler {
public:
    virtual bool event(Event &) = 0;
};

class AddHandler : public Handler {
public:
    bool event(Event& event) override {
        if(event.code==EventCode::add){
            if(auto cnt = std::static_pointer_cast<ContextAdd>(event.context))
            if(auto dt = std::static_pointer_cast<DataAdd>(event.data)){
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                cnt->result = dt->lhv+dt->rhv;
            }
            event.callback(event.context);
         return true;
        } else return false;
    }
};

class EventLoop {
public:
    void send(const Event& event) {
        m_event_queue.push(event);
    }

    void addHandler(Handler *handler) {
        m_handlers.push_back(handler);
    }

    void operator ()() {
        while (!m_quit) {
            if (!m_event_queue.empty()) {
                auto &ev = m_event_queue.front();
                m_event_queue.pop();

                switch (ev.code) {
                	// Special event for stopping
                    case EventCode::quit:
                        m_quit = true;
                        break;
                    // All other events are handled by handlers
                    default:
                        for (auto handler : m_handlers) {
                        	assert(handler);
                        	if (handler->event(ev))
                        		break;
                        }
                }
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }
private:
	bool m_quit = false;
    std::queue<Event> m_event_queue;
    std::vector<Handler *> m_handlers;
    std::thread m_eventLoopThread;
};



auto main() -> int {
    AddHandler handler;
    EventLoop eventLoop;
    
    eventLoop.addHandler(&handler);

    std::thread message_processor{std::ref(eventLoop)};

    int sum{};
    bool done{false};

    eventLoop.send({
        EventCode::add,
        std::make_shared<DataAdd>(2,3),
        std::make_shared<ContextAdd>(sum,done),
        [](std::shared_ptr<Context> cnt){ 
            if(auto ptr=std::static_pointer_cast<ContextAdd>(cnt))
                ptr->done = true;
          }
    });
    
    
    // some UI thungs
    while(!done){
        std::cout << '.';
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::cout << std::endl << "result=" << sum << std::endl;

    eventLoop.send({
        EventCode::quit,
        std::make_shared<Data>(),
        std::make_shared<Context>(),
        [](auto){}
    });
    message_processor.join();

	return 0;
}
