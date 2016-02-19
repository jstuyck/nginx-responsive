/*
** ngx_http_responsive_module.h for nginx_responsive
**
** Made by Julien Stuyck
** Login   jstuyck@github
**
** Started on  18 Feb 2016
** Last update 19 Feb 2016
*/

#ifndef NGX_HTTP_RESPONSIVE_MODULE_H
#define NGX_HTTP_RESPONSIVE_MODULE_H



static ngx_int_t	ngx_http_responsive_init(ngx_conf_t *cf);
static void*		ngx_http_responsive_create_loc_conf(ngx_conf_t*	cf);
static ngx_int_t 	ngx_http_responsive_handler(ngx_http_request_t* r);

static ngx_buf_t *ngx_http_responsive_compress(ngx_http_request_t *r,ngx_str_t *path, int sizex, int sizey);

typedef struct {
    ngx_flag_t     enable;
} ngx_http_responsive_loc_conf_t;


static ngx_command_t ngx_http_responsive_commands[] = {
	{
		ngx_string("responsive"),
		NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
     	ngx_conf_set_flag_slot,
      	NGX_HTTP_LOC_CONF_OFFSET,
      	offsetof(ngx_http_responsive_loc_conf_t, enable),
      	NULL },
	ngx_null_command
};

static ngx_http_module_t ngx_http_responsive_ctx = {
	NULL,	//Preconfiguration
	ngx_http_responsive_init,	//Postconfiguration
	NULL,	//create main configuration
	NULL,	//init main configuration
	NULL,	//create server configuration
	NULL,	//merge server configuration
	ngx_http_responsive_create_loc_conf,	//create location configuration
	NULL,	//merge location configuration
};

ngx_module_t ngx_http_responsive_module = {
	NGX_MODULE_V1,
	&ngx_http_responsive_ctx,		//module context
	ngx_http_responsive_commands,	//module directives
	NGX_HTTP_MODULE,				//module type
	NULL,							//init master
	NULL,							//init module
	NULL,							//init process
	NULL,							//init thread
	NULL,							//exit thread
	NULL,							//exit process
	NULL,							//exit master
	NGX_MODULE_V1_PADDING
};

#endif