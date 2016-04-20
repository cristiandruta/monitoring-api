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

#include "../mf_api.h"  /* mf_api_init(), mf_api_update() */
#include "../mf_util.h" /* mf_get_time(), mf_api_get_hostname() */

const char* experiment_id;
const char* server;
const char* task;
const char* workflow;
const char* json_string;

static void
read_params(int argc, char **argv)
{
    if (argc == 1) {
        fprintf(stderr, "usage: %s -e experimentID -w workflow -t taskID -d json_string [-s server]\n", argv[0]);
        exit (1);
    }

    /* set default server connection */
    server = "http://localhost:3030";

    int c;
    while ((c = getopt(argc, argv, "e:s:t:w:d:")) != -1) {
        switch (c) {
        case 'e':
            experiment_id = optarg;
            break;
        case 's':
            server = optarg;
            break;
        case 't':
            task = optarg;
            break;
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
main (int argc, char **argv) {
    /* set experiment id, task, workflow, server and json_string */
    read_params(argc, argv);

    /* init monitoring */
    mf_api_init(server, experiment_id, workflow, task);
    char* response = mf_api_raw_update(json_string);
    printf ("Response: %s\n", response);

    /* done */
    exit (0);
}
