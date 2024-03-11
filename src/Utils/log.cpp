//
// Created by nathanial on 3/9/24.
//
#include "log.h"
#include "iostream"
#include "string"

void Log(const auto &string) {
  std::cout << string << std::endl;
}

void Log(const int &string) {
  std::cout << string << std::endl;
}

void Log(const float &string) {
  std::cout << string << std::endl;
}

void Log(const std::string &string) {
  std::cout << string << std::endl;
}

void Log(const char* &string) {
  std::cout << string << std::endl;
}