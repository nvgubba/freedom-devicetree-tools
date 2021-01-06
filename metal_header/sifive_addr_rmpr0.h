/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef __METAL_HEADER_SIFIVE_ADDR_RMPR0__H
#define __METAL_HEADER_SIFIVE_ADDR_RMPR0__H

#include <device.h>

#include <regex>

class sifive_addr_rmpr0 : public Device {
public:
  sifive_addr_rmpr0(std::ostream &os, const fdt &dtb);
  void include_headers();
  void declare_inlines();
  void define_inlines();
  void declare_structs();
  void define_structs();
  void create_handles();
};

#endif
