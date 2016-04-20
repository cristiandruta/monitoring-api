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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../contrib/mf_publisher.h"

const char* json_string;
const char* workflow;
const char* server;

static void
read_params(int argc, char **argv)
{
    /* set default server connection */
    server = "http://localhost:3030";

    if (argc < 2) {
        fprintf(stderr, "usage: %s -w workflow [-s server] -d json_string\n", argv[0]);
        exit(1);
    }

    int c;
    while ((c = getopt(argc, argv, "s:w:d:")) != -1) {
        switch (c) {
        case 's':
        {
            size_t len = strlen(optarg);
            if ((len > 0) && (optarg[len - 1] == '/')) {
                optarg[len - 1] = '\0'; /* remove trailing slash */
            }
            server = optarg;
            break;
        }
        case 'w':
            workflow = optarg;
            break;
        case 'd':
            json_string = optarg;
            break;
        default:
            printf ("?? getopt returned character code 0%o ??\n", c);
        }
    }
    if (optind < argc) {
        printf ("non-option ARGV-elements: ");
        while (optind < argc) {
            printf ("%s ", argv[optind++]);
        }
        printf ("\n");
    }
}

int
is_resource_available(const char* URL)
{
    return mf_head(URL);
}

char*
create_experiment(
    const char* server,
    const char* workflow,
    const char* json_string)
{
    /*
    if (json_string == '\0') {
        puts("Error: No JSON document found (-d option)");
        exit (1);
    }
    */

    return mf_create_experiment(server, workflow, json_string);
}

int
main (int argc, char **argv) {
    read_params(argc, argv);

    if (!is_resource_available(server)) {
        printf("Error: Given server %s is not reachable.\n", server);
        exit (1);
    }

    char* response = create_experiment(server, workflow, json_string);
    char* token = strsep(&response, "\"");
    token = strsep(&response, "\"");
    printf ("EXPID=%s\n", token);

    exit (0);
}
