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

#define DEBUG

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
	u_char                     *ua;
	size_t                      len;
   
	size_t	root;
	u_char*							ext;
	ngx_chain_t 					out;
	ngx_str_t 						path;
	ngx_buf_t						*b;
	ngx_http_responsive_loc_conf_t 	*locconf;
	ngx_uint_t                      i,j;

	ext = NULL;

	i = r->uri.len - 1;

	while (i > 0 && r->uri.data[i--] != '/');
	
	if (i != 0)
		while (r->uri.data[i++] != '.');
	else
		return NGX_DECLINED;

	for (j=0; j < NB_EXT_ELMT; j++)
		if (ngx_strncasecmp(&r->uri.data[i],exts[j].data,sizeof(exts[j])))
		{
			ext = ngx_cpymem(ext,exts[j].data,sizeof(exts[j]));
			break;
		}

	if (ext == NULL)
		return NGX_DECLINED;

	

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


	//get the user agent name
	ua = r->headers_in.user_agent->value.data;
    len = r->headers_in.user_agent->value.len;



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


    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = sizeof("</pre><hr>");
    r->headers_out.content_type.len = sizeof("image/gif") - 1;
    r->headers_out.content_type.data = (u_char *) "image/gif";
    ngx_http_send_header(r);

 
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


