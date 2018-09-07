/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _INIT_LOG_H_
#define _INIT_LOG_H_

//#include <cutils/klog.h>

#define KLOG_ERROR_LEVEL   3
#define KLOG_WARNING_LEVEL 4
#define KLOG_NOTICE_LEVEL  5
#define KLOG_INFO_LEVEL    6
#define KLOG_DEBUG_LEVEL   7

#define KLOG_DEFAULT_LEVEL  3  /* messages <= this level are logged */


#define ERROR(x...)  printf(x)// init_klog_write(KLOG_ERROR_LEVEL, x)
#define NOTICE(x...) printf(x)//init_klog_write(KLOG_NOTICE_LEVEL, x)
#define INFO(x...)   printf(x) //init_klog_write(KLOG_INFO_LEVEL, x)

void init_klog_write(int level, const char* fmt, ...); //__printflike(2, 3);
int selinux_klog_callback(int level, const char* fmt, ...); //__printflike(2, 3);

#endif
