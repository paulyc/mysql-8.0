/* Copyright (c) 2016, 2017, Oracle and/or its affiliates. All Rights Reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Suite 500, Boston, MA 02110-1335 USA */

/** @file storage/temptable/src/plugin.cc
Glue code for registering the TempTable plugin at MySQL. */

#include "mysql/plugin.h"
#include "handler.h"
#include "temptable/allocator.h"
#include "temptable/handler.h"
#include "mem_root_fwd.h"
#include "table.h"

static handler* create_handler(handlerton* hton, TABLE_SHARE* table_share, bool,
                               MEM_ROOT* mem_root) {
  return new (mem_root) temptable::Handler(hton, table_share);
}

static st_mysql_storage_engine temptable_storage_engine = {
    MYSQL_HANDLERTON_INTERFACE_VERSION};

static int init(void* p) {
  handlerton* h = static_cast<handlerton*>(p);

  h->state = SHOW_OPTION_YES;
  h->db_type = DB_TYPE_TEMPTABLE;
  h->create = create_handler;
  h->flags = HTON_ALTER_NOT_SUPPORTED | HTON_CAN_RECREATE | HTON_HIDDEN |
             HTON_NOT_USER_SELECTABLE | HTON_NO_PARTITION |
             HTON_NO_BINLOG_ROW_OPT | HTON_SUPPORTS_EXTENDED_KEYS;

  temptable::Allocator<uint8_t>::init();

  return 0;
}

// clang-format off
mysql_declare_plugin(temptable) {
  MYSQL_STORAGE_ENGINE_PLUGIN,
  &temptable_storage_engine,
  "TempTable",
  "Oracle Corporation",
  "InnoDB temporary storage engine",
  PLUGIN_LICENSE_GPL,
  init,
  /* check uninstall */
  nullptr,
  /* destroy */
  nullptr,
  /* 1.0 */
  0x0100,
  /* status variables */
  NULL,
  /* system variables */
  NULL,
  /* config options */
  NULL,
  /* flags */
  0,
}
mysql_declare_plugin_end;
// clang-format on
