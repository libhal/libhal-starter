// Copyright 2024 Khalil Estell
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <libhal-exceptions/control.hpp>
#include <libhal-util/serial.hpp>
#include <libhal-util/steady_clock.hpp>
#include <libhal/error.hpp>

#include <resource_list.hpp>

// This is only global so that the terminate handler can use the resources
// provided.
resource_list resources{};

[[noreturn]] void terminate_handler() noexcept
{
  bool valid = resources.status_led && resources.clock;

  if (not valid) {
    // spin here until debugger is connected
    while (true) {
      continue;
    }
  }

  // Otherwise, blink the led in a pattern, and wait for the debugger.
  // In GDB, use the `where` command to see if you have the `terminate_handler`
  // in your stack trace.

  auto& led = *resources.status_led.value();
  auto& clock = *resources.clock.value();

  while (true) {
    using namespace std::chrono_literals;
    led.level(false);
    hal::delay(clock, 100ms);
    led.level(true);
    hal::delay(clock, 100ms);
    led.level(false);
    hal::delay(clock, 100ms);
    led.level(true);
    hal::delay(clock, 1000ms);
  }
}

void application();

int main()
{
  // Setup the terminate handler before we call anything that can throw
  hal::set_terminate(terminate_handler);

  // Initialize the platform and set as many resources as available for this the
  // supported platforms.
  initialize_platform(resources);

  try {
    application();
  } catch (std::bad_optional_access const& e) {
    if (resources.console) {
      hal::print(*resources.console.value(),
                 "A resource required by the application was not available!\n"
                 "Calling terminate!\n");
    }
  }  // Allow any other exceptions to terminate the application

  // Terminate if the code reaches this point.
  std::terminate();
}

void application()
{
  using namespace std::chrono_literals;

  // Calling `value()` on the optional resources will perform a check and if the
  // resource is not set, it will throw a std::bad_optional_access exception.
  // If it is set, dereference it and store the address in the references below.
  // When std::optional<T&> is in the standard, we will change to use that.
  auto& led = *resources.status_led.value();
  auto& clock = *resources.clock.value();
  auto& console = *resources.console.value();

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
}
