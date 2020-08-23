/*
 * Copyright (c) 2020 Deomid "rojer" Ryabkov
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mgos.h"
#include "mgos_bh1750.h"
#include "mgos_rpc.h"

static void timer_cb(void *arg) {
  struct mgos_bh1750 *bh = (struct mgos_bh1750 *) arg;
  int mt = mgos_bh1750_get_meas_time_ms(bh);
  int wt = mgos_bh1750_get_wait_time_ms(bh);
  if (mgos_bh1750_data_valid(bh)) {
    int raw;
    float lux = mgos_bh1750_read_lux(bh, &raw);
    LOG(LL_INFO, ("** %.2f lux (raw %d) mt %d wt %d", lux, raw, mt, wt));
  } else {
    LOG(LL_INFO, ("-- Inv, mt %d wt %d", mt, wt));
  }
}

static void set_config_handler(struct mg_rpc_request_info *ri, void *cb_arg,
                               struct mg_rpc_frame_info *fi,
                               struct mg_str args) {
  struct mgos_bh1750 *bh = (struct mgos_bh1750 *) cb_arg;
  int mode = -1, mtime = MGOS_BH1750_MTIME_DEFAULT;

  json_scanf(args.p, args.len, ri->args_fmt, &mode, &mtime);

  if (mode < 0) {
    mg_rpc_send_errorf(ri, -1, "mode is required");
    return;
  }

  if (!mgos_bh1750_set_config(bh, (enum mgos_bh1750_mode) mode, mtime)) {
    mg_rpc_send_errorf(ri, -2, "failed to set config %d %d", mode, mtime);
    return;
  }

  mg_rpc_send_responsef(ri, NULL);
  (void) fi;
}

enum mgos_app_init_result mgos_app_init(void) {
  uint8_t addr = mgos_bh1750_detect();
  if (addr == 0) {
    LOG(LL_ERROR, ("-- No sensors found"));
    return MGOS_APP_INIT_ERROR;
  }
  LOG(LL_INFO, ("++ Found BH1750 sensor at %#x", addr));
  struct mgos_bh1750 *bh = mgos_bh1750_create(addr);
  if (bh == NULL) {
    LOG(LL_ERROR, ("-- Failed to init sensor"));
    return MGOS_APP_INIT_ERROR;
  }
  if (!mgos_bh1750_set_config(bh, MGOS_BH1750_MODE_CONT_HIGH_RES,
                              MGOS_BH1750_MTIME_DEFAULT)) {
    LOG(LL_ERROR, ("-- Failed to set config"));
    return MGOS_APP_INIT_ERROR;
  }
  mgos_set_timer(50, MGOS_TIMER_REPEAT, timer_cb, bh);
  mg_rpc_add_handler(mgos_rpc_get_global(), "BH1750.SetConfig",
                     "{mode: %d, mtime: %d}", set_config_handler, bh);
  return MGOS_APP_INIT_SUCCESS;
}
