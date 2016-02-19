
[![Build Status](https://travis-ci.org/jstuyck/nginx-responsive.svg?branch=master)](https://travis-ci.org/jstuyck/nginx-responsive)


# nginx-responsive

This module optimize image delivery depending on the device that request the image.


## Build

To build the project you need nginx source code, as nginx does not load dynamicly libraries.

```bash
> $ ./configure --add-module=../nginx-responsive
```

## debug

First reconfigure the project with debug option and recompile nginx.

```bash
> $ ./configure --add-module=../nginx-responsive --with-debug
```

To debug the project, use the nginx configuration file provided in the test folder


Then run the following command to run nginx with GDB


```bash
> $ cd objs
> $ gdb --args ./nginx -p `pwd`/ -c ../../test/nginx.conf
> (gdb) run
```


## Development 

This project is in development, don't use it now :)


