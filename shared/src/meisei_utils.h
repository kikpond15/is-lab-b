#pragma once
#include <M5Unified.h>
namespace meisei {
inline void banner(const char* title) {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println(title);
}
}