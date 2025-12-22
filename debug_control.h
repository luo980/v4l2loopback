#ifndef _DEBUG_CONTROL_H_
#define _DEBUG_CONTROL_H_

#include <linux/printk.h>
#include <linux/kernel.h>
#include <linux/device.h> // 为了处理 dev_info
#include <linux/ftrace.h> // 为了使用 trace_printk


#ifndef ENABLE_PERF_LOGGING
    #define ENABLE_PERF_LOGGING 1
#endif

#define PERF_TAG "[VB2]"

#if ENABLE_PERF_LOGGING

    /**
     * @brief 性能打点宏 (开启状态)
     * @param name   事件名称 (例如 "Host_Recv")
     * @param key_ts 【身份ID】UVC透传过来的原始时间戳 (用于串联流程)
     * @param now_ts 【当前时刻】ktime_get_ns() (用于计算耗时)
     */
    #define log_point(name, key_ts, now_ts) \
        do { \
            trace_printk("%s %s,%llu,%llu\n", PERF_TAG, name, (unsigned long long)key_ts, (unsigned long long)now_ts); \
            pr_info("%s %s,%llu,%llu\n", PERF_TAG, name, (unsigned long long)key_ts, (unsigned long long)now_ts); \
        } while(0)

#else

    #define log_point(name, key_ts, now_ts) \
        no_printk(KERN_INFO "%s %s,%llu,%llu\n", PERF_TAG, name, (unsigned long long)key_ts, (unsigned long long)now_ts)

    #ifdef pr_info
        #undef pr_info
    #endif
    #define pr_info(fmt, ...) no_printk(fmt, ##__VA_ARGS__)

    #ifdef pr_debug
        #undef pr_debug
    #endif
    #define pr_debug(fmt, ...) no_printk(fmt, ##__VA_ARGS__)

    #ifdef pr_cont
        #undef pr_cont
    #endif
    #define pr_cont(fmt, ...) no_printk(fmt, ##__VA_ARGS__)

    #ifdef dev_info
        #undef dev_info
    #endif
    #define dev_info(dev, fmt, ...) no_printk(fmt, ##__VA_ARGS__)
    
    #ifdef dev_dbg
        #undef dev_dbg
    #endif
    #define dev_dbg(dev, fmt, ...) no_printk(fmt, ##__VA_ARGS__)

#endif 

#endif // _DEBUG_CONTROL_H_