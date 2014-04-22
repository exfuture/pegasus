/**
 * @author Oleksandr Natalenko aka post-factum <oleksandr@natalenko.name>
 *
 * @section LICENSE
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 * pegasus — digital channel simulator, panic file
 */

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "pegasus_panic.h"

static void pgp_show_stacktrace()
{
	void* buffer[PGP_BACKTRACE_SIZE];
	size_t buffer_size;
	char** symbols;

	buffer_size = backtrace(buffer, PGP_BACKTRACE_SIZE);
	symbols = backtrace_symbols(buffer, buffer_size);

	for (size_t i = 0; i < buffer_size; i++)
		printf("%s\n", symbols[i]);

	free(symbols);
}

void pgp_usage(char _key, char* _keyvalue)
{
	fprintf(stderr, "Unknown value \"%s\" of key \"%c\"\n", _keyvalue, _key);
	exit(EX_USAGE);
}

void __pgp_malloc(const char* _function_name, const char* _file_name, int _line_number, int _errno)
{
	fprintf(stderr, "malloc failed: file=%s, function=%s, line=%d (%s)\n", _file_name, _function_name, _line_number, strerror(_errno));
	pgp_show_stacktrace();
	exit(EX_OSERR);
}

void __pgp_clock_gettime(const char* _function_name, const char* _file_name, int _line_number, int _errno)
{
	fprintf(stderr, "clock_gettime failed: file=%s, function=%s, line=%d (%s)\n", _file_name, _function_name, _line_number, strerror(_errno));
	pgp_show_stacktrace();
	exit(EX_OSERR);
}

void __pgp_switch_default(const char* _function_name, const char* _file_name, int _line_number)
{
	fprintf(stderr, "switch-case default occurred: file=%s, function=%s, line=%d\n", _file_name, _function_name, _line_number);
	pgp_show_stacktrace();
	exit(EX_SOFTWARE);
}

void __pgp_null(const char* _function_name, const char* _file_name, int _line_number)
{
	fprintf(stderr, "Unhandled NULL occurred: file=%s, function=%s, line=%d\n", _file_name, _function_name, _line_number);
	pgp_show_stacktrace();
	exit(EX_SOFTWARE);
}

void __pgp_range(const char* _function_name, const char* _file_name, int _line_number)
{
	fprintf(stderr, "Out of range error occurred: file=%s, function=%s, line=%d\n", _file_name, _function_name, _line_number);
	pgp_show_stacktrace();
	exit(EX_SOFTWARE);
}

void __pgp_nodata(const char* _function_name, const char* _file_name, int _line_number)
{
	fprintf(stderr, "Data expected but got nothing: file=%s, function=%s, line=%d\n", _file_name, _function_name, _line_number);
	pgp_show_stacktrace();
	exit(EX_SOFTWARE);

}

