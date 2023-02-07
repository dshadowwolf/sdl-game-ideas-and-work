#ifndef __EVENT_MARSHALL_HPP__

#include <SDL2/SDL.h>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <algorithm>
#include <functional>

#include "generic_exception.hpp"

typedef std::function<bool(SDL_Event *)> EventHandler; // bool ((*EventHandler)(SDL_Event *));

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
        std::for_each(handlers.begin(), handlers.end(), [event](auto item) { 
            if (item.first == event->type) {
                std::for_each(item.second.begin(), item.second.end(), [event](auto it) { 
                    (*it)(event); 
                });
            }
        });
    }

};

#define __EVENT_MARSHALL_HPP__
#endif
