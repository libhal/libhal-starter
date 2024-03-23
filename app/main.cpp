#include <chrono>

#include <libhal-exceptions/control.hpp>
#include <libhal-util/serial.hpp>
#include <libhal-util/steady_clock.hpp>
#include <libhal/error.hpp>
#include <libhal/steady_clock.hpp>

#include <app/hardware_map.hpp>

hardware_map_t hardware_map{};

int main()
{
  using namespace std::chrono_literals;

  try {
    hardware_map = initialize_platform();
  } catch (...) {
    hal::halt();
  }

  hal::set_terminate(+[]() { hardware_map.reset(); });

  auto& led = *hardware_map.led;
  auto& clock = *hardware_map.clock;
  auto& console = *hardware_map.console;

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
  hardware_map.reset();

  return 0;
}
