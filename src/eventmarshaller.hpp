#ifndef __EVENT_MARSHALL_HPP__

#include <SDL2/SDL.h>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <algorithm>
#include <functional>

#include "generic_exception.hpp"

typedef std::function<bool(SDL_Event *)> EventHandler;

class EventMarshaller {
    private:
    inline static EventMarshaller* instance{nullptr};
    std::unordered_map<SDL_EventType, std::unordered_set<std::shared_ptr<EventHandler>>> handlers;
    EventMarshaller() {};
    ~EventMarshaller() {
        std::for_each(handlers.begin(), handlers.end(), [&](auto item) { 
            std::for_each(item.second.begin(), item.second.end(), [item](std::shared_ptr<EventHandler> it) mutable {
                item.second.erase(it);
            });
        });
    }

    public:
    EventMarshaller(EventMarshaller &other) = delete;
    void operator=(const EventMarshaller &other) = delete;

    static inline std::shared_ptr<EventMarshaller *> getInstance() { 
        if (instance == nullptr) {
            instance = new EventMarshaller();
        }

        return std::make_shared<EventMarshaller *>(instance);
    }

    inline void registerEventHandler(SDL_EventType eventType, EventHandler eventHandler) {
        if (handlers[eventType].find(std::make_shared<EventHandler>(eventHandler)) != handlers[eventType].end()) {
            // the entry already exists, this is an error
            std::cerr << "Entry for event type " << eventType << " at address " << static_cast<void *>(&eventHandler) << " already exists!" << std::endl;
            throw new GenericException("Event Handler Already Registered!");
        } else {
            handlers[eventType].insert(std::make_shared<EventHandler>(eventHandler));
        }
    }

    inline void runEvents(SDL_Event *event) {
        std::unordered_set<std::shared_ptr<EventHandler>> events = handlers[(SDL_EventType)event->type];
        bool keep_going = true;

        for(auto iter = events.begin(); iter != events.end() && keep_going; iter++) {
            keep_going = (*(*iter))(event);
        }
    }

};

#define __EVENT_MARSHALL_HPP__
#endif
