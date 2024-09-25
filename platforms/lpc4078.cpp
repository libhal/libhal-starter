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

#include <libhal-arm-mcu/dwt_counter.hpp>
#include <libhal-arm-mcu/lpc40/clock.hpp>
#include <libhal-arm-mcu/lpc40/constants.hpp>
#include <libhal-arm-mcu/lpc40/output_pin.hpp>
#include <libhal-arm-mcu/lpc40/uart.hpp>
#include <libhal-arm-mcu/system_control.hpp>

#include <resource_list.hpp>

void initialize_platform(resource_list& p_list)
{
  using namespace hal::literals;

  p_list.reset = +[]() { hal::cortex_m::reset(); };
  // Set the MCU to the maximum clock speed
  hal::lpc40::maximum(12.0_MHz);

  auto cpu_frequency = hal::lpc40::get_frequency(hal::lpc40::peripheral::cpu);
  static hal::cortex_m::dwt_counter counter(cpu_frequency);
  p_list.clock = &counter;

  static hal::lpc40::output_pin led(1, 10);
  p_list.status_led = &led;

  static std::array<hal::byte, 64> receive_buffer{};
  static hal::lpc40::uart uart0(0,
                                receive_buffer,
                                hal::serial::settings{
                                  .baud_rate = 115200,
                                });
  p_list.console = &uart0;
}
