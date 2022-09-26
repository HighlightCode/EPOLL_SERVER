#include "epoll_server.h"
#include <iostream>
#include <stdio.h>


namespace shape {

Rectangle::Rectangle(float w, float h) : w_(w), h_(h) {}

float Rectangle::GetSize() const { return w_ * h_; }

void Rectangle::Print() const {
  printf("내 크기는 : %f 입니다. \n", GetSize());
}

} 