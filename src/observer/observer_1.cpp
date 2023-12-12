//
// Created by sheldon on 2021/11/9.
//
#include "observer_1.h"

void Subject::Attach(Observer *obs) {
    _observers.push_back(obs);
}

void Subject::Detach(Observer *obs) {
    _observers.remove(obs);
}

void Subject::Notify() {
    for(auto it = _observers.begin(); it != _observers.end(); ++it) {
        (*it)->Update(this);
    }
}

