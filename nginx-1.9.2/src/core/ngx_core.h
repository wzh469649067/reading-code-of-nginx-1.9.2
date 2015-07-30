
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CORE_H_INCLUDED_
#define _NGX_CORE_H_INCLUDED_


#include <ngx_config.h>

typedef struct ngx_module_s      ngx_module_t;
typedef struct ngx_conf_s        ngx_conf_t;
typedef struct ngx_cycle_s       ngx_cycle_t;
typedef struct ngx_pool_s        ngx_pool_t;
typedef struct ngx_chain_s       ngx_chain_t;
typedef struct ngx_log_s         ngx_log_t;
typedef struct ngx_open_file_s   ngx_open_file_t;
typedef struct ngx_command_s     ngx_command_t;
typedef struct ngx_file_s        ngx_file_t;
typedef struct ngx_event_s       ngx_event_t;
typedef struct ngx_event_aio_s   ngx_event_aio_t;
typedef struct ngx_connection_s  ngx_connection_t;

#if (NGX_THREADS)
typedef struct ngx_thread_task_s  ngx_thread_task_t;
#endif

/*
ÿһ���¼�����ĵĲ�����handler�ص�������������ÿһ���¼�����ģ��ʵ�֣��Դ˾�������¼�������Ρ����ѡ�
*/
typedef void (*ngx_event_handler_pt)(ngx_event_t *ev);

//ngx_connection_handler_pt���͵�handler��Ա��ʾ����������˿��ϳɹ������µ�TCP���Ӻ󣬾ͻ�ص�handler����
typedef void (*ngx_connection_handler_pt)(ngx_connection_t *c);

/*
��4��������������Nginxһϵ�ж�����

NGX_OK����ʾ�ɹ���Nginx�������ִ�и�����ĺ�����������ִ��subrequest����������󣩡�

NGX_DECLINED��������NGX_HTTP_CONTENT_PHASE�׶�Ѱ����һ�����ڸ��������Ȥ��HTTPģ�����ٴδ����������

NGX_DONE����ʾ����Ϊֹ��ͬʱHTTP��ܽ���ʱ���ټ���ִ���������ĺ������֡���ʵ�ϣ���ʱ�������ӵ����ͣ������keepalive���͵��û�����
    �ͻᱣ��סHTTP���ӣ�Ȼ��ѿ���Ȩ����Nginx���������������ã��������³�������һ�����������Ǳ������һ����ʱ�����Ĳ���������ĳ��������ã���
    ����������סNginx������Ϊ����û�аѿ���Ȩ������Nginx��������ngx_http_mytest_handler����Nginx worker���������ˣ���ȴ�����Ļذ�����
    ���ԣ���ͻᵼ��Nginx�����������⣬�ý����ϵ������û�����Ҳ�ò�����Ӧ����������ǰ������ʱ�����Ĳ�����Ϊ�����������֣�����Linux�ں�
    �ж��жϴ����Ļ��֣����ϰ벿�ֺ��°벿�ֶ����������ģ���ʱ���ٵĲ���������������ngx_http_mytest_handler����ʱ�����ϰ벿�֣�Ȼ�󷵻�NGX_DONE��
    �ѿ��ƽ�����Nginx���Ӷ���Nginx�������������������°벿�ֱ�����ʱ�����ﲻ̽�־����ʵ�ַ�ʽ����ʵ��ʹ��upstream��ʽ���������ʱ�õľ���
    ����˼�룩���ٻص��°벿�ִ��������������Ϳ��Ա�֤Nginx�ĸ����������ˡ������Ҫ�����˽�NGX_DONE�����壬��ô����ѧϰ��11�����ݣ����л���
    ����������ü������ݡ�

NGX_ERROR����ʾ������ʱ�����ngx_http_terminate_request��ֹ�����������POST��������ô������ִ����POST���������ֹ��������
*/
#define  NGX_OK          0
#define  NGX_ERROR      -1
#define  NGX_AGAIN      -2
#define  NGX_BUSY       -3
#define  NGX_DONE       -4
#define  NGX_DECLINED   -5
#define  NGX_ABORT      -6


#include <ngx_errno.h>
#include <ngx_atomic.h>
#include <ngx_thread.h>
#include <ngx_rbtree.h>
#include <ngx_time.h>
#include <ngx_socket.h>
#include <ngx_string.h>
#include <ngx_files.h>
#include <ngx_shmem.h>
#include <ngx_process.h>
#include <ngx_user.h>
#include <ngx_parse.h>
#include <ngx_parse_time.h>
#include <ngx_log.h>
#include <ngx_alloc.h>
#include <ngx_palloc.h>
#include <ngx_buf.h>
#include <ngx_queue.h>
#include <ngx_array.h>
#include <ngx_list.h>
#include <ngx_hash.h>
#include <ngx_file.h>
#include <ngx_crc.h>
#include <ngx_crc32.h>
#include <ngx_murmurhash.h>
#if (NGX_PCRE)
#include <ngx_regex.h>
#endif
#include <ngx_radix_tree.h>
#include <ngx_times.h>
#include <ngx_rwlock.h>
#include <ngx_shmtx.h>
#include <ngx_slab.h>
#include <ngx_inet.h>
#include <ngx_cycle.h>
#include <ngx_resolver.h>
#if (NGX_OPENSSL)
#include <ngx_event_openssl.h>
#endif
#include <ngx_process_cycle.h>
#include <ngx_conf_file.h>
#include <ngx_open_file_cache.h>
#include <ngx_os.h>
#include <ngx_connection.h>
#include <ngx_syslog.h>
#include <ngx_proxy_protocol.h>


#define LF     (u_char) '\n'
#define CR     (u_char) '\r'
#define CRLF   "\r\n"


#define ngx_abs(value)       (((value) >= 0) ? (value) : - (value))
#define ngx_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#define ngx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))

void ngx_cpuinfo(void);

#if (NGX_HAVE_OPENAT)
#define NGX_DISABLE_SYMLINKS_OFF        0
#define NGX_DISABLE_SYMLINKS_ON         1
#define NGX_DISABLE_SYMLINKS_NOTOWNER   2
#endif

#endif /* _NGX_CORE_H_INCLUDED_ */