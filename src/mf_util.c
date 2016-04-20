/*
 * Copyright 2014, 2015 High Performance Computing Center, Stuttgart
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mf_util.h"

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

static int
get_fully_qualified_domain_name(char *fqdn)
{
    struct addrinfo hints, *info, *p;

    int gai_result;

    char *hostname = malloc(sizeof(char) * 80);
    gethostname(hostname, sizeof hostname);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME;

    if ((gai_result = getaddrinfo(hostname, "http", &hints, &info)) != 1) {
        FILE *tmp = NULL;
        if ((tmp = popen("hostname", "r")) == NULL ) {
            perror("popen");
            return -1;
        }
        char line[200];
        while (fgets(line, 200, tmp) != NULL )
            sprintf(fqdn, "%s", line);
        return 1;
    }
    for (p = info; p != NULL ; p = p->ai_next) {
        sprintf(fqdn, "%s\n", p->ai_canonname);
    }

    if (info->ai_canonname)
        freeaddrinfo(info);

    return 1;
}

void
mf_api_get_hostname(char* hostname)
{
    get_fully_qualified_domain_name(hostname);
    hostname[strlen(hostname) - 1] = '\0';
}

static void
get_time_as_string(char* timestamp, const char* format, int in_milliseconds)
{
    char fmt[64];
    char buf[64];
    struct timeval tv;
    struct tm *tm;
    int cut_of = 0;
    if (in_milliseconds) {
        cut_of = 3;
    }

    gettimeofday(&tv, NULL);
    if((tm = localtime(&tv.tv_sec)) != NULL) {
        strftime(fmt, sizeof(fmt), format, tm);
        snprintf(buf, sizeof(buf), fmt, tv.tv_usec);
    }
    memcpy(timestamp, buf, strlen(buf) - cut_of);
    timestamp[strlen(buf) - cut_of] = '\0';
}

char*
mf_api_get_time()
{
    char* timestamp = malloc(sizeof(char) * 64);
    get_time_as_string(timestamp, "%Y-%m-%dT%H:%M:%S.%%6u", 1);
    return timestamp;
}

char*
mf_api_get_date()
{
    char* timestamp = malloc(sizeof(char) * 64);
    get_time_as_string(timestamp, "%Y-%m-%d", 0);
    return timestamp;
}