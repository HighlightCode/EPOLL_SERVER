#include <iostream>
#include <epoll_server.h>


using namespace shape;

class TestRectangle : public Rectangle
{
    public:
        TestRectangle(float w, float h) : Rectangle(w,h) {};
        void Print() const;
};