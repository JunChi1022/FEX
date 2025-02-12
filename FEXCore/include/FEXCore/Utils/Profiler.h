// SPDX-License-Identifier: MIT
#pragma once
#include <atomic>
#include <cstdint>
#include <string_view>

#ifdef _M_X86_64
#include <x86intrin.h>
#endif

#include <FEXCore/Utils/CompilerDefs.h>

namespace FEXCore::Profiler {
// FEXCore live-stats
constexpr uint8_t STATS_VERSION = 2;
enum class AppType : uint8_t {
  LINUX_32,
  LINUX_64,
  WIN_ARM64EC,
  WIN_WOW64,
};

struct ThreadStatsHeader {
  uint8_t Version;
  AppType app_type;
  uint8_t _pad[2];
  char fex_version[48];
  std::atomic<uint32_t> Head;
  std::atomic<uint32_t> Size;
  uint32_t Pad;
};

struct ThreadStats {
  std::atomic<uint32_t> Next;
  std::atomic<uint32_t> TID;

  // Accumulated time (In unscaled CPU cycles!)
  uint64_t AccumulatedJITTime;
  uint64_t AccumulatedSignalTime;

  // Accumulated event counts
  uint64_t AccumulatedSIGBUSCount;
  uint64_t AccumulatedSMCCount;
  uint64_t AccumulatedFloatFallbackCount;
};

#ifdef ENABLE_FEXCORE_PROFILER

#ifdef _M_ARM_64
/**
 * @brief Get the raw cycle counter with synchronizing isb.
 *
 * `CNTVCTSS_EL0` also does the same thing, but requires the FEAT_ECV feature.
 */
static inline uint64_t GetCycleCounter() {
  uint64_t Result {};
  __asm volatile(R"(
      isb;
      mrs %[Res], CNTVCT_EL0;
    )"
                 : [Res] "=r"(Result));
  return Result;
}
#else
static inline uint64_t GetCycleCounter() {
  unsigned dummy;
  uint64_t tsc = __rdtscp(&dummy);
  return tsc;
}
#endif

FEX_DEFAULT_VISIBILITY void Init();
FEX_DEFAULT_VISIBILITY void Shutdown();
FEX_DEFAULT_VISIBILITY void TraceObject(std::string_view const Format);
FEX_DEFAULT_VISIBILITY void TraceObject(std::string_view const Format, uint64_t Duration);

// A class that follows scoping rules to generate a profile duration block
class ProfilerBlock final {
public:
  ProfilerBlock(std::string_view const Format);

  ~ProfilerBlock();

private:
  uint64_t DurationBegin;
  std::string_view const Format;
};

#define UniqueScopeName2(name, line) name##line
#define UniqueScopeName(name, line) UniqueScopeName2(name, line)

// Declare an instantaneous profiler event.
#define FEXCORE_PROFILE_INSTANT(name) FEXCore::Profiler::TraceObject(name)

// Declare a scoped profile block variable with a fixed name.
#define FEXCORE_PROFILE_SCOPED(name) FEXCore::Profiler::ProfilerBlock UniqueScopeName(ScopedBlock_, __LINE__)(name)

template<typename T, size_t FlatOffset = 0>
class AccumulationBlock final {
public:
  AccumulationBlock(T* Stat)
    : Begin {GetCycleCounter()}
    , Stat {Stat} {}

  ~AccumulationBlock() {
    const auto Duration = GetCycleCounter() - Begin + FlatOffset;
    if (Stat) {
      auto ref = std::atomic_ref<T>(*Stat);
      ref.fetch_add(Duration, std::memory_order_relaxed);
    }
  }

private:
  uint64_t Begin;
  T* Stat;
};

#define FEXCORE_PROFILE_ACCUMULATION(ThreadState, Stat)                                                                          \
  FEXCore::Profiler::AccumulationBlock<decltype(ThreadState->ThreadStats->Stat)> UniqueScopeName(ScopedAccumulation_, __LINE__)( \
    ThreadState->ThreadStats ? &ThreadState->ThreadStats->Stat : nullptr);
#define FEXCORE_PROFILE_INSTANT_INCREMENT(ThreadState, Stat, value) \
  do {                                                              \
    if (ThreadState->ThreadStats) {                                 \
      ThreadState->ThreadStats->Stat += value;                      \
    }                                                               \
  } while (0)

#else
[[maybe_unused]]
static void Init() {}
[[maybe_unused]]
static void Shutdown() {}
[[maybe_unused]]
static void TraceObject(std::string_view const Format) {}
[[maybe_unused]]
static void TraceObject(std::string_view const, uint64_t) {}

#define FEXCORE_PROFILE_INSTANT(...) \
  do {                               \
  } while (0)
#define FEXCORE_PROFILE_SCOPED(...) \
  do {                              \
  } while (0)
#define FEXCORE_PROFILE_ACCUMULATION(...) \
  do {                                    \
  } while (0)
#define FEXCORE_PROFILE_INSTANT_INCREMENT(...) \
  do {                                         \
  } while (0)

#endif
} // namespace FEXCore::Profiler
