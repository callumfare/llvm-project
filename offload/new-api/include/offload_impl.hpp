//===- offload_impl.hpp- Implementation helpers for the Offload library ---===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#pragma once

#include <iostream>
#include <memory>
#include <offload_api.h>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSet.h"

// Use the StringSet container to efficiently deduplicate repeated error
// strings (e.g. if the same error is hit constantly in a long running program)
llvm::StringSet<> &ErrorStrs();
// TODO: Need a better container type to avoid allocating duplicate error structs,
// even with deduplicated detail strings
using ErrPtrT = std::unique_ptr<offload_error_struct_t>;
std::vector<ErrPtrT> &Errors();

struct offload_impl_result_t {
  offload_impl_result_t(std::nullptr_t) : Result(OFFLOAD_RESULT_SUCCESS) {}
  offload_impl_result_t(offload_error_code_t Code) {
    if (Code == OFFLOAD_ERROR_CODE_SUCCESS) {
      Result = nullptr;
    } else {
      auto Err = std::unique_ptr<offload_error_struct_t>(
          new offload_error_struct_t{Code, nullptr});
      Result = Err.get();
      Errors().emplace_back(std::move(Err));
    }
  }

  offload_impl_result_t(offload_error_code_t Code, llvm::StringRef Details) {
    assert(Code != OFFLOAD_ERROR_CODE_SUCCESS);
    Result = nullptr;
    auto DetailsStr = ErrorStrs().insert(Details).first->getKeyData();
    auto Err = std::unique_ptr<offload_error_struct_t>(
        new offload_error_struct_t{Code, DetailsStr});
    Result = Err.get();
    Errors().emplace_back(std::move(Err));
  }

  operator offload_result_t() { return Result; }

private:
  offload_result_t Result;
};
