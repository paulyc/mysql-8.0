/* Copyright (c) 2017 Oracle and/or its affiliates. All rights reserved.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   51 Franklin Street, Suite 500, Boston, MA 02110-1335 USA */

#include "dd/impl/system_views/key_column_usage.h"

namespace dd {
namespace system_views {

const Key_column_usage &Key_column_usage::instance()
{
  static Key_column_usage *s_instance= new Key_column_usage();
  return *s_instance;
}

Key_column_usage::Key_column_usage()
{
  m_target_def.set_view_name(view_name());

  System_view_select_definition_impl *first_select, *second_select;
  first_select= m_target_def.get_first_select();
  second_select= m_target_def.get_second_select();


  // First SELECT for UNION

  first_select->add_field(FIELD_CONSTRAINT_CATALOG, "CONSTRAINT_CATALOG",
                          "cat.name" + m_target_def.fs_name_collation());
  first_select->add_field(FIELD_CONSTRAINT_SCHEMA, "CONSTRAINT_SCHEMA",
                          "sch.name" + m_target_def.fs_name_collation());
  first_select->add_field(FIELD_CONSTRAINT_NAME, "CONSTRAINT_NAME",
                          "CONVERT(idx.name USING utf8)" +
                          m_target_def.fs_name_collation());
  first_select->add_field(FIELD_TABLE_CATALOG, "TABLE_CATALOG",
                          "cat.name" + m_target_def.fs_name_collation());
  first_select->add_field(FIELD_TABLE_SCHEMA, "TABLE_SCHEMA",
                          "sch.name" + m_target_def.fs_name_collation());
  first_select->add_field(FIELD_TABLE_NAME, "TABLE_NAME",
                          "tbl.name" + m_target_def.fs_name_collation());
  first_select->add_field(FIELD_COLUMN_NAME, "COLUMN_NAME",
                          "col.name COLLATE utf8_tolower_ci");
  first_select->add_field(FIELD_ORDINAL_POSITION, "ORDINAL_POSITION",
                          "icu.ordinal_position");
  first_select->add_field(FIELD_POSITION_IN_UNIQUE_CONSTRAINT,
                          "POSITION_IN_UNIQUE_CONSTRAINT", "NULL");
  first_select->add_field(FIELD_REFERENCED_TABLE_SCHEMA,
                          "REFERENCED_TABLE_SCHEMA", "NULL");
  first_select->add_field(FIELD_REFERENCED_TABLE_NAME,
                          "REFERENCED_TABLE_NAME", "NULL");
  first_select->add_field(FIELD_REFERENCED_COLUMN_NAME,
                          "REFERENCED_COLUMN_NAME", "NULL");

  first_select->add_from("mysql.indexes idx");
  first_select->add_from("JOIN mysql.tables tbl ON idx.table_id=tbl.id");
  first_select->add_from("JOIN mysql.schemata sch ON tbl.schema_id=sch.id");
  first_select->add_from("JOIN mysql.catalogs cat ON cat.id=sch.catalog_id");
  first_select->add_from("JOIN mysql.index_column_usage icu"
                         " ON icu.index_id=idx.id");
  first_select->add_from("JOIN mysql.columns col ON icu.column_id=col.id"
                         " AND idx.type IN ('PRIMARY', 'UNIQUE')");

  first_select->add_where("CAN_ACCESS_COLUMN(sch.name, tbl.name, col.name)");
  first_select->add_where("AND IS_VISIBLE_DD_OBJECT(tbl.hidden, "
                          "col.hidden OR idx.hidden OR icu.hidden)");


  // Second SELECT for UNION

  second_select->add_field(FIELD_CONSTRAINT_CATALOG, "CONSTRAINT_CATALOG",
                           "cat.name" + m_target_def.fs_name_collation());
  second_select->add_field(FIELD_CONSTRAINT_SCHEMA, "CONSTRAINT_SCHEMA",
                           "sch.name" + m_target_def.fs_name_collation());
  second_select->add_field(FIELD_CONSTRAINT_NAME, "CONSTRAINT_NAME",
    "CONVERT(fk.name USING utf8)" + m_target_def.fs_name_collation());
  second_select->add_field(FIELD_TABLE_CATALOG, "TABLE_CATALOG",
                           "cat.name" + m_target_def.fs_name_collation());
  second_select->add_field(FIELD_TABLE_SCHEMA, "TABLE_SCHEMA",
                           "sch.name" + m_target_def.fs_name_collation());
  second_select->add_field(FIELD_TABLE_NAME, "TABLE_NAME",
                           "tbl.name" + m_target_def.fs_name_collation());
  second_select->add_field(FIELD_COLUMN_NAME, "COLUMN_NAME",
                           "col.name COLLATE utf8_tolower_ci");
  second_select->add_field(FIELD_ORDINAL_POSITION, "ORDINAL_POSITION",
                           "fkcu.ordinal_position");
  second_select->add_field(FIELD_POSITION_IN_UNIQUE_CONSTRAINT,
                           "POSITION_IN_UNIQUE_CONSTRAINT",
                           "fkcu.ordinal_position");
  second_select->add_field(FIELD_REFERENCED_TABLE_SCHEMA,
                           "REFERENCED_TABLE_SCHEMA",
                           "fk.referenced_table_schema");
  second_select->add_field(FIELD_REFERENCED_TABLE_NAME,
                           "REFERENCED_TABLE_NAME",
                           "fk.referenced_table_name");
  second_select->add_field(FIELD_REFERENCED_COLUMN_NAME,
                           "REFERENCED_COLUMN_NAME",
                           "fkcu.referenced_column_name");

  second_select->add_from("mysql.foreign_keys fk");
  second_select->add_from("JOIN mysql.tables tbl ON fk.table_id=tbl.id");
  second_select->add_from("JOIN mysql.foreign_key_column_usage fkcu"
                           " ON fkcu.foreign_key_id=fk.id");
  second_select->add_from("JOIN mysql.schemata sch ON fk.schema_id=sch.id");
  second_select->add_from("JOIN mysql.catalogs cat ON cat.id=sch.catalog_id");
  second_select->add_from("JOIN mysql.columns col ON fkcu.column_id=col.id");

  second_select->add_where("CAN_ACCESS_COLUMN(sch.name, tbl.name, col.name)");
  second_select->add_where("AND IS_VISIBLE_DD_OBJECT("
                           "tbl.hidden, col.hidden)");

}

}
}
