#
# Copyright 2016 BMC Software, Inc.
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

AC_PREREQ([2.64])
AC_INIT([meter-plugin-pitemp], [1.0.0])
AM_INIT_AUTOMAKE([ -Wall -Werror subdir-objects foreign])
AC_CONFIG_HEADERS([config.h])
AC_PROG_CC_C99
AC_CHECK_LIB(m, floor)
AC_CHECK_LIB(jansson, json_load_file)
AC_CHECK_LIB(meterplugin, plugin_run)
AC_CHECK_HEADERS([stdlib.h])
AC_PROG_INSTALL
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
