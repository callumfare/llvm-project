//===------- Offload API tests - olMemcpy2D -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "../common/Fixtures.hpp"
#include <OffloadAPI.h>
#include <gtest/gtest.h>

using olMemcpy2DTest = OffloadQueueTest;
OFFLOAD_TESTS_INSTANTIATE_DEVICE_FIXTURE(olMemcpy2DTest);

TEST_P(olMemcpy2DTest, SuccessHtoD) {
  constexpr size_t Width = 64;
  constexpr size_t Height = 16;

  void *Alloc;
  ASSERT_SUCCESS(
      olMemAlloc(Device, OL_ALLOC_TYPE_DEVICE, Width * Height, &Alloc));
  std::vector<uint8_t> Input(Width * Height, 42);
  ASSERT_SUCCESS(olMemcpy2D(Queue, Alloc, Device, Input.data(), Host, Width,
                            Width, Width, Height, nullptr));
  olWaitQueue(Queue);
  olMemFree(Alloc);
}


TEST_P(olMemcpy2DTest, SuccessDtoH) {
  constexpr size_t Width = 64;
  constexpr size_t Height = 16;
  void *Alloc;
  std::vector<uint8_t> Input(Width * Height, 42);
  std::vector<uint8_t> Output(Width * Height, 0);

  ASSERT_SUCCESS(
      olMemAlloc(Device, OL_ALLOC_TYPE_DEVICE, Width * Height, &Alloc));
  ASSERT_SUCCESS(olMemcpy2D(Queue, Alloc, Device, Input.data(), Host, Width,
                            Width, Width, Height, nullptr));
  ASSERT_SUCCESS(olMemcpy2D(Queue, Output.data(), Host, Alloc, Device, Width,
                            Width, Width, Height, nullptr));
  ASSERT_SUCCESS(olWaitQueue(Queue));
  for (uint8_t Val : Output) {
    ASSERT_EQ(Val, 42);
  }
  ASSERT_SUCCESS(olMemFree(Alloc));
}

TEST_P(olMemcpy2DTest, SuccessDtoH_Strided) {
  constexpr size_t Width = 64;
  constexpr size_t OutPitch = Width + 8;
  constexpr size_t Height = 16;
  void *Alloc;
  std::vector<uint8_t> Input(Width * Height, 42);
  std::vector<uint8_t> Output(OutPitch * Height, 0);

  ASSERT_SUCCESS(
      olMemAlloc(Device, OL_ALLOC_TYPE_DEVICE, Width * Height, &Alloc));
  ASSERT_SUCCESS(olMemcpy2D(Queue, Alloc, Device, Input.data(), Host, Width,
                            Width, Width, Height, nullptr));
  ASSERT_SUCCESS(olMemcpy2D(Queue, Output.data(), Host, Alloc, Device, OutPitch,
                            Width, Width, Height, nullptr));
  ASSERT_SUCCESS(olWaitQueue(Queue));
  for (size_t Y = 0; Y < Height; Y++) {
    for (size_t X = 0; X < OutPitch; X++) {
        auto Idx = Y * OutPitch + X;
        ASSERT_EQ(Output[Idx], X < Width ? 42 : 0);
    }
  }
//   for (uint8_t Val : Output) {
//     ASSERT_EQ(Val, 42);
//   }
  ASSERT_SUCCESS(olMemFree(Alloc));
}

TEST_P(olMemcpy2DTest, SuccessHtoD_Strided) {
  constexpr size_t Width = 64;
  constexpr size_t Height = 16;
  constexpr size_t DevPitch = Width + 8;

  void *Alloc;
  ASSERT_SUCCESS(
      olMemAlloc(Device, OL_ALLOC_TYPE_DEVICE, DevPitch * Height, &Alloc));
  std::vector<uint8_t> Input(Width * Height, 42);
  ASSERT_SUCCESS(olMemcpy2D(Queue, Alloc, Device, Input.data(), Host, DevPitch,
                            Width, Width, Height, nullptr));
  olWaitQueue(Queue);
  olMemFree(Alloc);
}
