#include <benchmark/benchmark.h>
#include <fmt/compile.h>
#include <fmt/ostream.h>
#include <string>
#include <sstream>
#include <iostream>

std::string str1 = "A moderate string for profile";
std::string str2 = "Another moderate string";
std::string str3 = "Other moderate string";
std::string str4 = "More moderate string";
std::string str5 = "Fifth moderate string";

// void naive(benchmark::State& state) {
//   benchmark::ClobberMemory();
//   for (auto _ : state) {
//     std::string output = "Result: " + str1 + ": (" + str2 + ',' + str3 + ',' +
//                          str4 + ',' + str5 + ')';
//     benchmark::DoNotOptimize(output.data());
//   }
// }
// BENCHMARK(naive);

// void append(benchmark::State& state) {
//   benchmark::ClobberMemory();
//   for (auto _ : state) {
//     std::string output = "Result: ";
//     output += str1;
//     output += ": (";
//     output += str2;
//     output += ',';
//     output += str3;
//     output += ',';
//     output += str4;
//     output += ',';
//     output += str5;
//     output += ')';
//     benchmark::DoNotOptimize(output.data());
//   }
// }
// BENCHMARK(append);

// void appendWithReserve(benchmark::State& state) {
//   benchmark::ClobberMemory();
//   for (auto _ : state) {
//     std::string output = "Result: ";
//     output.reserve(str1.length() + str2.length() + str3.length() +
//                    str4.length() + str5.length() + 16);
//     output += str1;
//     output += ": (";
//     output += str2;
//     output += ',';
//     output += str3;
//     output += ',';
//     output += str4;
//     output += ',';
//     output += str5;
//     output += ')';
//     benchmark::DoNotOptimize(output.data());
//   }
// }
// BENCHMARK(appendWithReserve);

// void format_compile(benchmark::State& state) {
//   benchmark::ClobberMemory();
//   for (auto _ : state) {
//     auto output = fmt::format(FMT_COMPILE("Result: {}: ({},{},{},{})"), str1,
//                               str2, str3, str4, str5);
//     benchmark::DoNotOptimize(output.data());
//   }
// }
// BENCHMARK(format_compile);

// void format_runtime(benchmark::State& state) {
//   benchmark::ClobberMemory();
//   for (auto _ : state) {
//     auto output =
//         fmt::format("Result: {}: ({},{},{},{})", str1, str2, str3, str4, str5);
//     benchmark::DoNotOptimize(output.data());
//   }
// }
// BENCHMARK(format_runtime);

// void format_to(benchmark::State& state) {
//   benchmark::ClobberMemory();
//   for (auto _ : state) {
//     fmt::memory_buffer output;
//     fmt::format_to(std::back_inserter(output), "Result: {}: ({},{},{},{})", str1, str2, str3, str4,
//                    str5);
//     benchmark::DoNotOptimize(output.data());
//   }
// }
// BENCHMARK(format_to);

// void nullop(benchmark::State& state) {
//   for (auto _ : state) {
//     benchmark::ClobberMemory();
//   }
// }
// BENCHMARK(nullop);

// Simulates the native stream concatenation
static void nativeStream(benchmark::State& state) {
  benchmark::ClobberMemory();
  std::stringstream ss;
  
  // Code before the loop is not measured
  for (auto _ : state) {
    ss << "Concat " << str1 << ", " << str2 << ", " << str3 << ", " << str4 << ", and " << str5;
  }
  benchmark::DoNotOptimize(ss);
}
BENCHMARK(nativeStream);

static void fmtToStream(benchmark::State& state) {
  benchmark::ClobberMemory();
  std::stringstream ss;

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    fmt::print(ss, "Concat {}, {}, {}, {}, {}", str1,str2,str3,str4,str5);
  }
  benchmark::DoNotOptimize(ss);
}
// Register the function as a benchmark
BENCHMARK(fmtToStream);

static void fmtBufferToStream(benchmark::State& state) {
  benchmark::ClobberMemory();
  std::stringstream ss;

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    auto out = fmt::memory_buffer();
    fmt::format_to(std::back_inserter(out), "Concat {}, ", str1);
    fmt::format_to(std::back_inserter(out), "{}, ", str2);
    fmt::format_to(std::back_inserter(out), "{}, ", str3);
    fmt::format_to(std::back_inserter(out), "{}, and", str4);
    fmt::format_to(std::back_inserter(out), "{}", str5);

    // Make sure the variable is not optimized away by compiler
    benchmark::DoNotOptimize(out.data());
    ss << fmt::to_string(out);
  }
  benchmark::DoNotOptimize(ss);
}
// Register the function as a benchmark
BENCHMARK(fmtBufferToStream);

static std::string toString(){
  auto out = fmt::memory_buffer();
  fmt::format_to(std::back_inserter(out), "Concat {}, ", str1);
  fmt::format_to(std::back_inserter(out), "{}, ", str2);
  fmt::format_to(std::back_inserter(out), "{}, ", str3);
  fmt::format_to(std::back_inserter(out), "{}, and", str4);
  fmt::format_to(std::back_inserter(out), "{}", str5);

  // Make sure the variable is not optimized away by compiler
  benchmark::DoNotOptimize(out.data());
  // benchmark::DoNotOptimize(created_string);
  return std::move(fmt::to_string(out));
}

static void fmtToStreamWithIntermediateStr(benchmark::State& state) {
  benchmark::ClobberMemory();
  std::stringstream ss;

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    ss << toString();
  }
  benchmark::DoNotOptimize(ss);
}
// Register the function as a benchmark
BENCHMARK(fmtToStreamWithIntermediateStr);
BENCHMARK_MAIN();
