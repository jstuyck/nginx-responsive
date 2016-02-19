/*
** ngx_http_responsive_module.c for nginx_responsive
**
** Made by Julien Stuyck
** Login   jtstuyck@github
**
** Started on  18 Feb 2016
** Last update 19 Feb 2016
*/

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

#include "ngx_http_responsive_module.h"


static ngx_int_t	ngx_http_responsive_init(ngx_conf_t *cf)
{
	ngx_http_handler_pt        *h;
	ngx_http_core_main_conf_t  *cmcf;

	cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);
	h = ngx_array_push(&cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers);
	if (h == NULL) {
		return NGX_ERROR;
	}
	*h = ngx_http_responsive_handler;
	return NGX_OK;
}


static void*	ngx_http_responsive_create_loc_conf(ngx_conf_t*	cf)
{
	ngx_http_responsive_loc_conf_t  *conf;

	conf = ngx_palloc(cf->pool, sizeof(ngx_http_responsive_loc_conf_t));
	if (conf == NULL) {
		return NULL;
	}
	conf->enable = NGX_CONF_UNSET;
	return conf;
}


static ngx_int_t ngx_http_responsive_handler(ngx_http_request_t* r)
{
	u_char	*last;
	//u_char	*filename;
	size_t	root;

	ngx_chain_t 					out;
	ngx_str_t 						path;
	ngx_buf_t						*b;
	ngx_http_responsive_loc_conf_t 	*locconf;


	if (r->uri.data[r->uri.len - 1] != '/') {
		return NGX_DECLINED;
	}

	if (!(r->method & (NGX_HTTP_GET|NGX_HTTP_HEAD))) {
		return NGX_DECLINED;
	}

	locconf = ngx_http_get_module_loc_conf(r, ngx_http_responsive_module);

	if (!locconf->enable) {
		return NGX_DECLINED;
	}

	last = ngx_http_map_uri_to_path(r, &path, &root, 0);

	if (last == NULL) {
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}

	ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "http responsive: \"%s\"", path.data);


	b = ngx_http_responsive_compress(r, &path, 10,10);



	if (b == NULL) {
        return NGX_ERROR;
    }

    if (r == r->main) {
        b->last_buf = 1;
    }

    b->last_in_chain = 1;

	out.buf = b;
    out.next = NULL;

 
	return ngx_http_output_filter(r, &out);
}



static ngx_buf_t *ngx_http_responsive_compress(ngx_http_request_t *r,ngx_str_t *path, int sizex, int sizey)
{
	size_t		len;
	ngx_buf_t	*b;

	len = sizeof("</pre><hr>") - 1;

	b = ngx_create_temp_buf(r->pool, len);
	if (b == NULL) {
        return NULL;
    }

    b->last = ngx_cpymem(b->last, "</pre><hr>", sizeof("</pre><hr>") - 1);
    return b;
}


