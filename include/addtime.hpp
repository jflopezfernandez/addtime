
#ifndef PROJECT_INCLUDES_addtime_HPP_
#define PROJECT_INCLUDES_addtime_HPP_

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <csetjmp>
#include <cerrno>
#include <cmath>
#include <csignal>
#include <ctime>
#include <clocale>
#include <climits>
#include <cinttypes>
#include <cctype>
#include <cwchar>
#include <cwctype>

#include <algorithm>
#include <any>
#include <atomic>
#include <array>
#include <bitset>
#include <chrono>
#include <execution>
#include <exception>
#include <filesystem>
#include <forward_list>
#include <fstream>
#include <functional>
#include <future>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <memory_resource>
#include <mutex>
#include <new>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <set>
#include <shared_mutex>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <string_view>
#include <system_error>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <valarray>
#include <variant>
#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>

namespace Options = boost::program_options;

/** The google test library */
#include <gtest/gtest.h>

#ifndef NDEBUG
#define EXIT_CODE RUN_ALL_TESTS()
#else
#define EXIT_CODE EXIT_SUCCESS
#endif //

#endif // PROJECT_INCLUDES_addtime_HPP_

