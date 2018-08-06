/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include "fdt.h++"
#include "libfdt.h++"
#include <fstream>
#include <iostream>
#include <string>

using std::cerr;
using std::endl;
using std::fstream;
using std::string;

static void show_usage(string name)
{
  std::cerr << "Usage: " << name << " <option(s)>\n"
	    << "Options:\n"
	    << "\t-h,--help\t\t\tShow this help message\n"
	    << "\t-d,--dtb <eg. xxx.dtb>\t\tSpecify fullpath to the DTB file\n"
	    << "\t-o,--output <eg. dts.fixup>\t\tGenerate Zephyr RTOS DTS fixup file\n"
	    << endl;
}

static void write_config_file(const fdt &dtb, fstream &os)
{
  os << "/*" << endl;
  os << " * Copyright (c) 2018 SiFive Inc." << endl;
  os << " *" << endl;
  os << " * SPDX-License-Identifier: Apache-2.0" << endl;
  os << " *" << endl;
  os << " * Auto-generated by freedom-zephyrdtsfixup-generator" << endl;
  os << " *     https://github.com/sifive/freedom-devicetree-tools" << endl;
  os << " */" << endl;

  auto emit_comment = [&](const node &n, string dev_name, int dev_num) {
    os << endl << "/* " << dev_name << "_" << dev_num << " -> " << n.name() << " */\n";
  };

  auto emit_fixup = [&](string out_prefix, string in_prefix, int dev_num, target_addr base, string suffix) {
    os << "#define ";
    os << out_prefix << "_" << dev_num << "_" << suffix << "\t";
    os << in_prefix << "_" << std::hex << base << std::dec << "_" << suffix << endl;
  };

  auto emit_base_addr = [&](string out_prefix, string in_prefix, int dev_num, 
      target_addr base) {
    emit_fixup(out_prefix, in_prefix, dev_num, base, "BASE_ADDR");
  };

  auto emit_size = [&](string out_prefix, string in_prefix, int dev_num,
      target_addr base) {
    emit_fixup(out_prefix, in_prefix, dev_num, base, "SIZE");
  };

  auto emit_irq = [&](string out_prefix, string in_prefix, int dev_num, target_addr base, int irq_num) {
    emit_fixup(out_prefix, in_prefix, dev_num, base, "IRQ_" + std::to_string(irq_num)); 
  };

  auto emit_irqs = [&](const node &n, string out_prefix, string in_prefix, int dev_num, target_addr base) {
    if(n.field_exists("interrupts")) {
      auto irqs = n.get_fields<uint32_t>("interrupts");
      for(int i = 0; i < irqs.size(); i++)
        emit_irq(out_prefix, in_prefix, dev_num, base, i); 
    }
  };

  dtb.match(
    std::regex("sifive,gpio0"), [&](node n) {
      static int dev_num = 0;
      emit_comment(n, "GPIO", dev_num);
      n.named_tuples(
        "reg-names", "reg",
        "control", tuple_t<target_addr, target_size>(), [&](target_addr base, target_size size) {
          emit_base_addr("FE310_GPIO", "SIFIVE_GPIO0", dev_num, base);
          emit_irqs(n, "FE310_GPIO", "SIFIVE_GPIO0", dev_num, base);
          emit_size("FE310_GPIO", "SIFIVE_GPIO0", dev_num, base);
      });
      dev_num++;
    },
    std::regex("sifive,uart0"), [&](node n) {
      static int dev_num = 0;
      emit_comment(n, "UART", dev_num);
      n.named_tuples(
        "reg-names", "reg",
        "control", tuple_t<target_addr, target_size>(), [&](target_addr base, target_size size) {
          emit_base_addr("FE310_UART", "SIFIVE_UART0", dev_num, base);
          emit_irqs(n, "FE310_UART", "SIFIVE_UART0", dev_num, base);
          emit_size("FE310_UART", "SIFIVE_UART0", dev_num, base);
      });
      dev_num++;
    }
  );
};

int main (int argc, char* argv[])
{
  string dtb_file;
  string config_file;

  if ((argc < 2) && (argc > 5)) {
      show_usage(argv[0]);
      return 1;
  } else {
      for (int i = 1; i < argc; ++i) {
          string arg = argv[i];
          if ((arg == "-d") || (arg == "--dtb")) {
              if (i + 1 < argc) {
                  dtb_file = argv[++i];
              } else {
                  std::cerr << "--dtb option requires file path." << std::endl;
                  show_usage(argv[0]);
                  return 1;
              }
          } else if ((arg == "-o") || (arg == "--output")) {
              if (i + 1 < argc) {
                  config_file = argv[++i];
              } else {
                  std::cerr << "--output option requires file path." << std::endl;
                  show_usage(argv[0]);
                  return 1;
              }
          } else {
              show_usage(argv[0]);
              return 1;
          }
      }
  }

  if (dtb_file.empty()) {
      std::cerr << "--dtb option requires file path." << std::endl;
      show_usage(argv[0]);
      return 1;
  }

  fdt f(dtb_file);

  if (!config_file.empty()) {
    std::fstream cfg;

    cfg.open(config_file, fstream::in | fstream::out | fstream::trunc);
    if (cfg.is_open() == false) {
      std::cerr << "Error: Failed to create " << config_file << std::endl;
      return 1;
    }

    write_config_file(f, cfg);
  }

  return 0;
}
