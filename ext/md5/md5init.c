/************************************************

  md5init.c -

  $Author$
  created at: Fri Aug  2 09:24:12 JST 1996

  Copyright (C) 1995-2001 Yukihiro Matsumoto

************************************************/
/* This module provides an interface to the RSA Data Security,
   Inc. MD5 Message-Digest Algorithm, described in RFC 1321. */

#include "ruby.h"
#include "md5.h"

static VALUE cMD5;

static VALUE
md5i_update(obj, str)
    VALUE obj, str;
{
    md5_state_t *md5;
    char *p;
    int len;

    p = rb_str2cstr(str, &len);
    Data_Get_Struct(obj, md5_state_t, md5);
    md5_append(md5, p, len);

    return obj;
}

static VALUE
md5i_digest(obj)
    VALUE obj;
{
    md5_state_t *md5, ctx;
    md5_byte_t digest[16];

    Data_Get_Struct(obj, md5_state_t, md5);
    ctx = *md5;
    md5_finish(&ctx, digest);

    return rb_str_new(digest, 16);
}

static VALUE
md5i_hexdigest(obj)
    VALUE obj;
{
    md5_state_t *md5, ctx;
    md5_byte_t digest[16];
    char buf[33];
    int i;

    Data_Get_Struct(obj, md5_state_t, md5);
    ctx = *md5;
    md5_finish(&ctx, digest);

    for (i=0; i<16; i++) {
	sprintf(buf+i*2, "%02x", digest[i]);
    }
    return rb_str_new(buf, 32);
}

static VALUE
md5i_clone(obj)
    VALUE obj;
{
    md5_state_t *md5, *md5_new;

    Data_Get_Struct(obj, md5_state_t, md5);
    obj = Data_Make_Struct(CLASS_OF(obj), md5_state_t, 0, free, md5_new);
    *md5_new = *md5;

    return obj;
}

static VALUE
md5i_new(argc, argv, class)
    int argc;
    VALUE* argv;
    VALUE class;
{
    VALUE obj;
    md5_state_t *md5;

    rb_scan_args(argc, argv, "01", &arg);
    obj = Data_Make_Struct(class, md5_state_t, 0, free, md5);
    md5_init(md5);
    if (argc == 1) {
	md5i_update(obj, argv[0]);
    }

    return obj;
}

void
Init_md5()
{
    cMD5 = rb_define_class("MD5", rb_cObject);

    rb_define_singleton_method(cMD5, "new", md5i_new, -1);
    rb_define_singleton_method(cMD5, "md5", md5i_new, -1);

    rb_define_method(cMD5, "update", md5i_update, 1);
    rb_define_method(cMD5, "digest", md5i_digest, 0);
    rb_define_method(cMD5, "hexdigest", md5i_hexdigest, 0);
    rb_define_method(cMD5, "clone",  md5i_clone, 0);
}
