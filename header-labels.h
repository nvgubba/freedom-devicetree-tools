/* Copyright (c) 2019 SiFive Inc. */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef __HEADER_LABELS__H
#define __HEADER_LABELS__H

/* This file contains the suffixes for the "bare header" #defines so that
 * they can be kept synchronized between freedom-bare_header-generator and
 * freedom-metal_header-generator. */

/* Generic Devices */
#define METAL_BASE_ADDRESS_LABEL "BASE_ADDRESS"
#define METAL_SIZE_LABEL "SIZE"

/* Clocks */
#define METAL_CLOCK_FREQUENCY_LABEL "CLOCK_FREQUENCY"
#define METAL_CLOCK_DIV_LABEL "CLOCK_DIV"
#define METAL_CLOCK_MULT_LABEL "CLOCK_MULT"

/* FE310-G000 PRCI */
#define METAL_HFROSCCFG_LABEL "HFROSCCFG"
#define METAL_HFXOSCCFG_LABEL "HFXOSCCFG"
#define METAL_PLLCFG_LABEL "PLLCFG"
#define METAL_PLLOUTDIV_LABEL "PLLOUTDIV"

/* CLINT */
#define METAL_MSIP_BASE_LABEL "MSIP_BASE"
#define METAL_MTIMECMP_BASE_LABEL "MTIMECMP_BASE"
#define METAL_MTIME_LABEL "MTIME"

/* PLIC */
#define METAL_RISCV_MAX_PRIORITY_LABEL "RISCV_MAX_PRIORITY"
#define METAL_RISCV_NDEV_LABEL "RISCV_NDEV"

/* CLIC */
#define METAL_SIFIVE_NUMINTS_LABEL "SIFIVE_NUMINTS"
#define METAL_SIFIVE_NUMLEVELS_LABEL "SIFIVE_NUMLEVELS"
#define METAL_SIFIVE_NUMINTBITS_LABEL "SIFIVE_NUMINTBITS"

/* FU540-C000 L2 */
#define METAL_SIFIVE_FU540_C000_L2_CONFIG_LABEL "CONFIG"
#define METAL_SIFIVE_FU540_C000_L2_WAYENABLE_LABEL "WAYENABLE"

#endif /* __HEADER_LABELS__H */
