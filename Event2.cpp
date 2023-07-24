#include "event.h"
#include <stdio.h>


////////////////////////////////////////////////////////////////////////////////////////////////////////
// Callback imp

template<typename _T>
template<class K>
EventCallback<_T>::EventCallback(void(*_callback)(void*, K*)) : hInstance(nullptr), s_ptf((StaticCallback_t)_callback) { }

template<typename _T>
template<class K>
EventCallback<_T>::EventCallback(_T* _instance, void(_T::* _callback)(void*, K*)) : hInstance(_instance), m_ptmf((MemberCallback_t)_callback) { }

template<typename _T>
typename EventCallback<_T>::CallbackType EventCallback<_T>::GetCallbackType() { 
    return (CallbackType)(hInstance == 0); 
}


template<typename _T>
void EventCallback<_T>::operator ()(void* _sender, EventArgs* _args) {
    switch (GetCallbackType()) {
    case StaticCallback: return (*s_ptf)(_sender, _args);
    case MemberCallback: return (hInstance->*m_ptmf)(_sender, _args);
    }
}

template<typename _T>
bool EventCallback<_T>::operator ==(IEventCallback* other) {
    EventCallback* otherEventCallback = (EventCallback< _T>*)other;
    if (otherEventCallback == nullptr) return false;

    if (GetCallbackType() == StaticCallback && otherEventCallback->GetCallbackType() == StaticCallback)
        return this->s_ptf == otherEventCallback->s_ptf;
    else if (GetCallbackType() == MemberCallback && otherEventCallback->GetCallbackType() == MemberCallback)
        return this->m_ptmf == otherEventCallback->m_ptmf && this->hInstance == otherEventCallback->hInstance;
    else return false;

}

template<typename _T>
int EventCallback<_T>::Dump() {
    switch (GetCallbackType()) {
    case StaticCallback: return (int)(s_ptf);
    case MemberCallback: return (int)(&m_ptmf);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Event imp

Event::Event() { }

void Event::Add(IEventCallback* _callback) {
    if (m_count >= MAX_CALLBACK_COUNT) {
        // Max size reached.
        return;
    }
    for (int i = 0; i < m_count; i++) {
        if (m_callbacks[i] != 0 && (*m_callbacks[i] == _callback)) {
            // Callback already exists
            printf("Unable to add %#X callback, it already exists\n", _callback->Dump());
            return;
        }
    }
    // Matching callback not found

    m_callbacks[m_count++] = _callback;
    printf("Added %#X callback.\n", _callback->Dump());

}

void Event::Remove(IEventCallback* _callback) {
    int removed_index = -1;

    // Removes the element ( if find )
    for (int i = 0; i < m_count; i++) {
        if (*m_callbacks[i] == _callback) {
            m_callbacks[i] = 0;
            removed_index = i;
            printf("%#X Removed from the event list\n", _callback->Dump());
        }
    }

    // there is no reasson to fix holes if there is any hole
    if (removed_index < 0) {
        return;
    }

    // Fix the holes in the list
    for (int i = removed_index; i < m_count; i++) {
        if (m_callbacks[i] == 0)
            if (i < m_count - 1)
                if (m_callbacks[i + 1] != 0) {
                    m_callbacks[i] = m_callbacks[i + 1];
                    m_callbacks[i + 1] = 0;
                }
    }

    // Shrinks counter
    m_count--;
}

void Event::Invoke(EventArgs* _args) {
    for (int i = 0; i < m_count; i++) {
        if (m_callbacks[i] != 0) {
            (*m_callbacks[i])(0, _args);
        }
    }
}

void Event::Dump() {
    printf("Starting to dump contents of Event:\n");
    for (int i = 0; i < m_count; i++) {
        int callback_dump = 0;
        if (m_callbacks[i] != 0) {
            callback_dump = m_callbacks[i]->Dump();
        }
        printf("0x%.8X", callback_dump);
        if (i < m_count - 1)
            printf(",");
        printf("\n");

    }
    printf("Dump finished.\n");
}
