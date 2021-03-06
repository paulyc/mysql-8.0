/*
 * Copyright (c) 2015, 2017, Oracle and/or its affiliates. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the
 * License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#ifndef _NGS_PROTOCOL_AUTHENTICATION_H_
#define _NGS_PROTOCOL_AUTHENTICATION_H_

#include "ngs_common/bind.h"
#include "ngs/error_code.h"
#include "ngs/interface/account_verification_interface.h"
#include "ngs/memory.h"

namespace ngs {
class Session_interface;
class Authentication_interface;

typedef ngs::Memory_instrumented<Authentication_interface>::Unique_ptr
    Authentication_interface_ptr;

class Authentication_interface {
 public:
  enum Status {
    Ongoing,
    Succeeded,
    Failed,
    Error
  };

  struct Response {
    Response(const Status status_ = Ongoing, const int error_ = 0,
             const std::string &data_ = "")
        : data(data_), status(status_), error_code(error_) {}
    std::string data;
    Status status;
    int error_code;
  };

  typedef Authentication_interface_ptr (*Create)(Session_interface *session);

  Authentication_interface() {}
  Authentication_interface(const Authentication_interface &) = delete;
  Authentication_interface &operator=(const Authentication_interface &) =
      delete;

  virtual ~Authentication_interface() {}

  virtual Response handle_start(const std::string &mechanism,
                                const std::string &data,
                                const std::string &initial_response) = 0;

  virtual Response handle_continue(const std::string &data) = 0;

  virtual ngs::Error_code authenticate_account(
      const std::string &user, const std::string &host,
      const std::string &passwd) const = 0;
};

}  // namespace ngs

#endif
