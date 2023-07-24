#pragma once



class DUMMY_TYPE { /*  EMPTY CLASS  */ };

class EventArgs {
public:
    EventArgs() {}

    virtual const char* GetTypename() { return "EventArgs"; }

};

class IEventCallback {
public:
    virtual void operator ()(void*, EventArgs*) = 0;
    virtual bool operator ==(IEventCallback*) = 0;
    virtual int Dump() = 0;

};

template< class _T = DUMMY_TYPE>
class EventCallback : public IEventCallback {
private:
    enum CallbackType : int {
        MemberCallback = 0,
        StaticCallback = 1
    };

    using MemberCallback_t = void(_T::*)(void*, EventArgs*);
    using StaticCallback_t = void(*)(void*, EventArgs*);

private:
    struct {
        _T* hInstance;
        union {
            MemberCallback_t m_ptmf;
            StaticCallback_t s_ptf;
        };
    };

public:
    template<class K>
    EventCallback(void(*_callback)(void*, K*));
    template<class K>
    EventCallback(_T* _instance, void(_T::* _callback)(void*, K*));

private:
    CallbackType GetCallbackType();

public:
    void operator ()(void* _sender, EventArgs* _args) override;
    bool operator ==(IEventCallback* other);
    int Dump() override;
};

constexpr int MAX_CALLBACK_COUNT = 256;
class Event {
public:
    Event() { }

public:
    void Add(IEventCallback* _callback);
    void Remove(IEventCallback* _callback);
    void Invoke(EventArgs* _args);
    void Dump();

private:
    int m_count = 0;
    IEventCallback** m_callbacks = new IEventCallback * [MAX_CALLBACK_COUNT];
};
