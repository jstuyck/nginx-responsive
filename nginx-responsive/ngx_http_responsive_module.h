
#ifndef NGX_HTTP_RESPONSIVE_MODULE_H
#define NGX_HTTP_RESPONSIVE_MODULE_H

static char* ngx_http_responsive(ngx_conf_t* cf, ngx_command_t* cmd, void* conf);

static u_char* ngx_responsive_string = (u_char*)"test";

static ngx_command_t ngx_http_responsive_commands[] = {
	{
		ngx_string("fun"),
		NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS,
		ngx_http_responsive,
		0,0,
		NULL
	},
	ngx_null_command
};

static ngx_http_module_t ngx_http_responsive_ctx = {
	NULL,	//Preconfiguration
	NULL,	//Postconfiguration
	NULL,	//create main configuration
	NULL,	//init main configuration
	NULL,	//create server configuration
	NULL,	//merge server configuration
	NULL,	//create location configuration
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