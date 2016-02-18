#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

#include "ngx_http_responsive_module.h"


static ngx_int_t ngx_http_responsive_handler(ngx_http_request_t* r)
{
	ngx_int_t	rc;
	ngx_buf_t*	b;
	ngx_chain_t	out;

	if (!(r->method & (NGX_HTTP_GET|NGX_HTTP_HEAD)))
	{
		return NGX_HTTP_NOT_ALLOWED;
	}

	rc = ngx_http_discard_request_body(r);
	if (rc != NGX_OK)
	{
		return rc;
	}

	r->headers_out.content_type_len = sizeof("text/html") - 1;
	r->headers_out.content_type.len = sizeof("text/html") - 1;
	r->headers_out.content_type.date = (u_cahr*)"text/html";

	if (r->method == NGX_HTTP_HEAD)
	{
		r->headers_out.status = NGX_HTTP_OK;
		r->headers_out.content_length_n = sizeof(ngx_reponsive_string) - 1;
		return ngx_http_send_header(r);
	}

	b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
	if (b == NULL)
	{
		return NGX_HEEP_INTERNAL_SERVER_ERROR;
	}

	out.buf = b;
	out.next = NULL;

	b->pos = ngx_reponsive_string;
	b->last = ngx_reponsive_string + sizeof(ngx_fun_string) - 1;
	b->memory = 1;
	b->last_buf = 1;

	r->headers_out.status = NGX_HTTP_OK;
	r->headers_out.content_length_n = sizeof(ngx_fun_string) - 1;

	rc = ngx_http_send_header(r);

	if (rc == NGX_ERROR || rc > NGX_OK || r->header_only)
	{
		return (rc);
	}
	return ngx_http_output_filter(r, &out);
}

static char* ngx_http_responsive(ngx_conf_t* cf, ngx_command_t* cmd, void* conf)
{
	ngx_http_core_loc_conf_t*	clcf;

	clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_code_module);
	clcf->handler = ngx_http_responsive_handler;

	return NGX_CONF_OK;
}


