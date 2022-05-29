// Copyright 2021 Your Name <your_email>

#include "Log.hpp"

Log::Log()
    :_level(0)
    , _out(&std::cout){}

Log& Log::get_instance() {
  static Log log;
  return log;
}

void Log::Write(std::string_view message) const {
  *_out << message << std::endl;
}

void Log::WriteDebug(std::string_view message) const {
  if (_level > 0) *_out << message << std::endl;
}
