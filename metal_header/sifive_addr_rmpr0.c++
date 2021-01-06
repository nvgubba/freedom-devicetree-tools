/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <sifive_addr_rmpr0.h>

#include <regex>

using std::string;

sifive_addr_rmpr0::sifive_addr_rmpr0(std::ostream &os, const fdt &dtb)
    : Device(os, dtb, "sifive,addr_rmpr0") {}

void sifive_addr_rmpr0::include_headers() {
  dtb.match(std::regex(compat_string),
            [&](node n) { emit_include(compat_string); });
}

void sifive_addr_rmpr0::declare_inlines() {
  Inline *func;
  std::list<Inline *> extern_inlines;
  int count = 0;

  dtb.match(std::regex(compat_string), [&](node n) {
    n.named_tuples(
        "reg-names", "reg", "control", tuple_t<target_addr, target_size>(),
        [&](target_addr base, target_size size) {
          if (count == 0) {
            func = create_inline_dec("base", "unsigned long",
                                     "const struct metal_addr_rmpr *addr_rmpr");
            extern_inlines.push_back(func);

            func = create_inline_dec("size", "unsigned long",
                                     "const struct metal_addr_rmpr *addr_rmpr");
            extern_inlines.push_back(func);
          }
        });
    count++;
  });
  os << "\n";
  os << "/* --------------------- sifive_addr_rmpr0 ------------ */\n";
  while (!extern_inlines.empty()) {
    func = extern_inlines.front();
    extern_inlines.pop_front();
    emit_inline_dec(func, "sifive_addr_rmpr0");
    delete func;
  }
  os << "\n";
}

void sifive_addr_rmpr0::define_inlines() {
  Inline *base_func;
  Inline *size_func;

  int count = 0;

  dtb.match(std::regex(compat_string), [&](node n) {
    if (count == 0) {
      base_func = create_inline_def(
          "base", "unsigned long",
          "(uintptr_t)addr_rmpr == (uintptr_t)&__metal_dt_" + n.handle(),
          platform_define(n, METAL_BASE_ADDRESS_LABEL),
          "const struct metal_addr_rmpr *addr_rmpr");

      size_func = create_inline_def("size", "unsigned long",
                                    "(uintptr_t)addr_rmpr == (uintptr_t)&__metal_dt_" +
                                        n.handle(),
                                    platform_define(n, METAL_SIZE_LABEL),
                                    "const struct metal_addr_rmpr *addr_rmpr");
    } else {
      add_inline_body(base_func,
                      "(uintptr_t)addr_rmpr == (uintptr_t)&__metal_dt_" + n.handle(),
                      platform_define(n, METAL_BASE_ADDRESS_LABEL));
      add_inline_body(size_func,
                      "(uintptr_t)addr_rmpr == (uintptr_t)&__metal_dt_" + n.handle(),
                      platform_define(n, METAL_SIZE_LABEL));
    }

    count += 1;
  });
  os << "\n";
  os << "/* --------------------- sifive_addr_rmpr0 ------------ */\n";
  if (count != 0) {
    add_inline_body(base_func, "else", "0");
    emit_inline_def(base_func, "sifive_addr_rmpr0");
    delete base_func;
    add_inline_body(size_func, "else", "0");
    emit_inline_def(size_func, "sifive_addr_rmpr0");
    delete size_func;
  }
  os << "\n";
}

void sifive_addr_rmpr0::declare_structs() {
  dtb.match(std::regex(compat_string),
            [&](node n) { emit_struct_decl("sifive_addr_rmpr0", n); });
}

void sifive_addr_rmpr0::define_structs() {
  dtb.match(std::regex(compat_string), [&](node n) {
    emit_struct_begin("sifive_addr_rmpr0", n);

    emit_struct_field("addr_rmpr.vtable",
                      "&__metal_driver_vtable_sifive_addr_rmpr0.addr_rmpr");

    emit_struct_end();
  });
}

void sifive_addr_rmpr0::create_handles() {
  dtb.match(std::regex(compat_string), [&](node n) {
    emit_def_handle("__METAL_DT_ADDR_RMPR_HANDLE", n, ".addr_rmpr");
  });
}
