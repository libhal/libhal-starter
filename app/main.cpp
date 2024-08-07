#include <libhal-exceptions/control.hpp>
#include <libhal-util/serial.hpp>
#include <libhal-util/steady_clock.hpp>
#include <libhal/error.hpp>
#include <libhal/steady_clock.hpp>

#include <app/resource_list.hpp>

resource_list resources{};

int main()
{
  using namespace std::chrono_literals;

  try {
    resources = initialize_platform();
  } catch (...) {
    hal::halt();
  }

  hal::set_terminate(+[]() { resources.reset(); });

  auto& led = *resources.led;
  auto& clock = *resources.clock;
  auto& console = *resources.console;

  hal::print(console, "Starting Application!\n");
  hal::print(console, "Will reset after ~10 seconds\n");

  for (int i = 0; i < 10; i++) {
    // Print message
    hal::print(console, "Hello, World\n");

    // Toggle LED
    led.level(true);
    hal::delay(clock, 500ms);

    led.level(false);
    hal::delay(clock, 500ms);
  }

  hal::print(console, "Resetting!\n");
  hal::delay(clock, 100ms);
  resources.reset();

  return 0;
}
