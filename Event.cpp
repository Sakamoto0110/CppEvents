#include <iostream>


// Another alternative is use dynamic storage 
template<class T, int hBuffer = 32>
struct Event
{
	// Type of function pointer that holds the handler
	// TODO: add suport for EventArgs
	typedef void(*EventHandler)(T& source);

	size_t Count() const
	{
		int old_ptr = m_ptr;
		size_t c = 0;
		while (old_ptr-- > 0)
			c++;

		return c;
	}

	Event& operator +=(const EventHandler handler)
	{
		if (m_ptr < hBuffer)
			m_handlers[m_ptr++] = std::move(handler);
		return *this;
	}

	Event& operator -=(const EventHandler handler)
	{
		int old_ptr = m_ptr;
		bool skipFix = true;

		// Removes the element ( if find )
		while (old_ptr-- > 0)
		{
			if (m_handlers[old_ptr] == handler)
				skipFix = m_handlers[old_ptr] = 0;
		}

		// there is no reasson to fix holes if there is any hole
		if (skipFix) 
			return *this;

		// Fix the holes in the list
		while (old_ptr++ < m_ptr)
		{
			if (m_handlers[old_ptr] == 0)
				if (old_ptr < m_ptr - 1)
					if (m_handlers[old_ptr + 1] != 0)
					{
						m_handlers[old_ptr] = m_handlers[old_ptr + 1];
						m_handlers[old_ptr + 1] = 0;
					}
		}
		m_ptr--;
		return *this;
	}

	void Invoke(T& source)
	{
		int old_ptr = m_ptr;
		while (old_ptr-- > 0) 
			if (m_handlers[old_ptr] != 0)
				m_handlers[old_ptr](source);

	}
private:
	EventHandler* m_handlers = new EventHandler[hBuffer];
	int m_ptr = 0;
};


struct Foo
{
	int x = 32;
	int y = 582;
	int z = 995;
	int w = 193;

	void Method()
	{
		MyEvent.Invoke(*this);
	}

	Event<Foo> MyEvent;
};

void FooHandler0(Foo& source)
{
	std::cout << "Handler 0 called " << source.x << std::endl;
}

void FooHandler1(Foo& source)
{
	std::cout << "Handler 1 called " << source.y << std::endl;
}

void FooHandler2(Foo& source)
{
	std::cout << "Handler 2 called " << source.z << std::endl;
}

void FooHandler3(Foo& source)
{
	std::cout << "Handler 3 called " << source.w << std::endl;
}

void FooHandler4(Foo& source)
{
	std::cout << "Handler 4 called " << source.x << std::endl;
}

void FooHandler5(Foo& source)
{
	std::cout << "Handler 5 called " << source.y << std::endl;
}

void FooHandler6(Foo& source)
{
	std::cout << "Handler 6 called " << source.z << std::endl;
}

void FooHandler7(Foo& source)
{
	std::cout << "Handler 7 called " << source.w << std::endl;
}
void FooHandler8(Foo& source)
{
	std::cout << "Handler 8 called " << source.w << std::endl;
}
void FooHandler9(Foo& source)
{
	std::cout << "Handler 9 called " << source.w << std::endl;
}

int main()
{
	Foo f;
	f.MyEvent += FooHandler0; 
	f.MyEvent += FooHandler1;
	f.MyEvent += FooHandler2; 
	f.MyEvent += FooHandler3;
	f.MyEvent += FooHandler4;
	f.MyEvent += FooHandler5;
	f.MyEvent += FooHandler6;
	f.MyEvent += FooHandler7;
	f.MyEvent += FooHandler8;
	f.MyEvent += FooHandler9;
	f.Method();
	std::cout << f.MyEvent.Count() << " <- \n\n\n" << std::endl;
	f.MyEvent -= FooHandler4;
	std::cout << f.MyEvent.Count() << " <- \n\n\n" << std::endl;
	f.MyEvent -= FooHandler4;
	std::cout << f.MyEvent.Count() << " <- \n\n\n" << std::endl;
	f.Method();
	
	std::cin.get();
	return 0;
}
