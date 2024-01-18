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

#include "application.hpp"

int main()
{
  // Step 1. Call the processor initializer. Setups up processor to run
  // applications such as turning on a coprocessor, or copying memory from
  // storage to memory. With picolibc's crt0.s startup routine, there isn't
  // Much need for this, but for platforms that do not have such support, or
  // need additional support, this function helps.
  if (!initialize_processor()) {
    hal::halt();
  }

  // Step 2. Call the platform initializer. This will create the instances of
  // drivers and resources used by the application to operate. ADCs, PWMs,
  // serial communication, bespoke interfaces, buffers, storage, configuration
  // constants and more can be required by an application and the platform needs
  // to manage creating these for the application to function.
  auto application_resource = initialize_platform();

  // Step 2.1. Check if resource creation was successful. This can be done
  // using the Boost.LEAF error handing to or simply check if an error was
  // emitted.
  if (!application_resource) {
    hal::halt();
  }

  // Step 3. Pass the application_resources
  auto is_finished = application(application_resource.value());

  if (!is_finished) {
    application_resource.value().reset();
  } else {
    hal::halt();
  }

  return 0;
}

namespace boost {
void throw_exception([[maybe_unused]] std::exception const& p_error)
{
  hal::halt();
}
}  // namespace boost
