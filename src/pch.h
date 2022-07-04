#ifndef NEWLANG_PCH_H_
#define NEWLANG_PCH_H_


#include <map>
#include <set>
#include <iosfwd>
#include <memory>
#include <vector>
#include <deque>
#include <iterator>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <type_traits>
#include <map>
#include <string>
#include <cmath>
#include <random>
#include <typeindex>
#include <locale>
#include <codecvt>
#include <functional>
#include <regex>
#include <variant>
#include <filesystem>

#include <sstream>
#include <iostream>
#include <fstream>
#include <fcntl.h>

#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _MSC_VER

#include <windows.h>
#include <wchar.h>

#else

#include <wait.h>
#include <dlfcn.h>
#include <sys/param.h>

#endif

#include <ffi.h>

#include "warning_push.h"
#include <torch/torch.h>
#include <ATen/ATen.h>

#include <contrib/logger/logger.h>
#include "warning_pop.h"

#include "types.h"

#undef LOG_RUNTIME
#define LOG_RUNTIME(...)  LOG_EXCEPT(newlang::Interrupt, ##__VA_ARGS__)

#endif // NEWLANG_PCH_H_
