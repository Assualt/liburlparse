#include "tstring_builder.h"
#include "string_builder.h"
#include <iostream>
#include <benchmark/benchmark.h>

using my_StringBuilder = MyStringBuilder;
static void Append_MyNeWString(benchmark::State &state) {
    TStringBuilder builder;
    for (auto _ : state) {
        for (int i = 0; i < 1000000; i++) {
            builder.append("Hello World");
        }
    }
}

static void Append_MyString(benchmark::State &state) {
    my_StringBuilder builder;
    for (auto _ : state) {
        for (int i = 0; i < 1000000; i++) {
            builder.append("Hello World");
        }
    }
}

static void charAt(benchmark::State &state) {
    my_StringBuilder builder("hello world");
    for (auto _ : state) {
        for (int i = 0; i < 100000; i++) {
            builder.charAt(4);
        }
    }
}

static void charNewAt(benchmark::State &state) {
    TStringBuilder builder("hello world");
    for (auto _ : state) {
        for (int i = 0; i < 100000; i++) {
            builder.charAt(4);
        }
    }
}

BENCHMARK(Append_MyNeWString);
BENCHMARK(Append_MyString);
BENCHMARK(charNewAt);
BENCHMARK(charAt);
BENCHMARK_MAIN();
