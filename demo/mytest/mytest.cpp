#define MICROPROFILE_MAX_FRAME_HISTORY (2<<10)
#define MICROPROFILE_IMPL
#include "microprofile.h"
#include <unistd.h>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>

MICROPROFILE_DEFINE(MAIN, "MAIN", "Main", 0xff0000);

int main(int argc, char const * const argv[])
{
   MicroProfileOnThreadCreate("Main");
   using namespace ::std::chrono;
   using ::std::cout;
   const auto start = high_resolution_clock::now();
   const milliseconds interval{50};
   ::std::uint64_t count = 0;
   static const auto scoped_timer{MicroProfileGetToken("SCOPED", "ScopeTest", 0xff00ff00, MicroProfileTokenTypeCpu)};
   ::std::unique_ptr<MicroProfileScopeHandler> scopetestptr;
   while (true) {
      count++;
      const auto next = start + count * interval;
      const auto curtime = high_resolution_clock::now();
      if (next > curtime) {
         const auto tonext = duration_cast<microseconds>(next - curtime);
         cout << "tonext.count() == " << tonext.count() << " && count == " << count << '\n';
         usleep(tonext.count());
      } else {
         cout << "Something weird happened, count == " << count << '\n';
      }
      if (count % 5 == 0) {
         scopetestptr = ::std::make_unique<MicroProfileScopeHandler>(scoped_timer);
      } else if (count % 5 == 3) {
         scopetestptr.reset(nullptr);
      }
      MicroProfileFlip(0);
   }
   return 0;
}
