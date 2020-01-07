#include "stringbuilder_1.h"
#include "stringbuilder.h"
#include <iostream>
using namespace std;
typedef StringBuilder<char> my_StringBuilder;

#include <benchmark/benchmark.h>

static void Append_MyNeWString(benchmark::State &state) {
    TStringBuilder builder;
    for (auto _ : state) {
        for (int i = 0; i < 10000; i++) {
            builder.append("Hello World");
        }
    }
}

static void Append_MyString(benchmark::State &state) {
    my_StringBuilder builder;
    for (auto _ : state) {
        for (int i = 0; i < 10000; i++) {
            builder.Append("Hello World");
        }
    }
}

static void charAt(benchmark::State &state) {
    my_StringBuilder builder("hello world");
    for (auto _ : state) {
        for (int i = 0; i < 10000; i++) {
            builder.charAt(4);
        }
    }
}
static void charNewAt(benchmark::State &state) {
    TStringBuilder builder("hello world");
    for (auto _ : state) {
        for (int i = 0; i < 10000; i++) {
            builder.charAt(4);
        }
    }
}

BENCHMARK(Append_MyNeWString);
BENCHMARK(Append_MyString);
BENCHMARK(charNewAt);
BENCHMARK(charAt);
BENCHMARK_MAIN();