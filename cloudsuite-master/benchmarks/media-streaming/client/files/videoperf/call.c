/*
   httperf -- a tool for measuring web server performance
   Copyright (C) 2000  Hewlett-Packard Company
   Contributed by David Mosberger-Tang <davidm@hpl.hp.com>

   This file is part of httperf, a web server performance measurment
   tool.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <httperf.h>
#include <call.h>
#include <conn.h>

static u_long next_id = 0;

int inc_call_next_id(int n)
{
	next_id = n+next_id;
	return next_id - n;
}

int get_call_next_id ()
{
	return (int) next_id;
}

	void
call_init (Call *c)
{
# define DEFAULT_METHOD	"GET"

	c->id = next_id++;
	call_set_method (c, DEFAULT_METHOD, sizeof (DEFAULT_METHOD) - 1);
	c->req.version = param.http_version;
	c->req.iov[IE_BLANK].iov_base = (caddr_t) " ";
	c->req.iov[IE_BLANK].iov_len = 1;
	c->req.iov[IE_NEWLINE1].iov_base = (caddr_t) "\r\n";
	c->req.iov[IE_NEWLINE1].iov_len = 2;
	c->req.iov[IE_NEWLINE2].iov_base = (caddr_t) "\r\n";
	c->req.iov[IE_NEWLINE2].iov_len = 2;

	c->file_size = -2;
	c->record_conn_time = 0;
#ifdef UW_DYNOUT
	c->timelimit = param.timeout; 
#endif /* UW_DYNOUT */
}

	void
call_deinit (Call *call)
{
}

	int
call_append_request_header (Call *c, const char *hdr, size_t len)
{
	u_int num_hdrs = c->req.num_extra_hdrs;

	if (num_hdrs >= MAX_EXTRA_HEADERS)
	{
		fprintf (stderr, "%s.call_append_request_header: max headers "
				"(%d) exceeded.\n", prog_name, MAX_EXTRA_HEADERS);
		return -1;
	}
	c->req.iov[IE_FIRST_HEADER + num_hdrs].iov_base = (caddr_t) hdr;
	c->req.iov[IE_FIRST_HEADER + num_hdrs].iov_len = len;
	c->req.num_extra_hdrs = num_hdrs + 1;
	return 0;
}
