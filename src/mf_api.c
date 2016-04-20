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
#include "mf_api.h"
#include "mf_util.h"
#include "contrib/mf_publisher.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct mf_state_t mf_state;

struct mf_state_t {
    const char* experiment_id;
    const char* workflow;
    const char* task;
    const char* hostname;
    const char* server;
    const char* path;
    const char* date;
} mf_state_t;

mf_state* status;

static void to_lowercase(char* word, int length);

void
mf_api_init(
    const char* server,
    const char* experiment_id,
    const char* workflow,
    const char* task)
{
    if (status == NULL) {
        status = malloc(sizeof(mf_state));
    }
    status->server = strdup(server);
    status->path = strdup("v1/dreamcloud/mf/metrics");
    status->experiment_id = strdup(experiment_id);

    char tmp[strlen(workflow)];
    strcpy(tmp, workflow);
    to_lowercase(tmp, strlen(tmp));
    status->workflow = strdup(tmp);

    if (task == '\0') {
        status->task = strdup("seq");
    } else {
        strcpy(tmp, task);
        to_lowercase(tmp, strlen(tmp));
        status->task = strdup(tmp);
    }

    char* hostname = malloc(sizeof(char) * 254);
    mf_api_get_hostname(hostname);
    status->hostname = strdup(hostname);

    //status->date = strdup(mf_api_get_date());

    /*
    char URL[256];
    sprintf(URL, "%s/%s_%s",
        status->server,
        status->workflow,
        status->task
        //status->date
    );
    */

    //mf_create_metrics_index(URL, "idx");

    free(hostname);
}

char*
mf_api_update(mf_metric* metric)
{
    char curl_data[4095] = { 0 };
    if (metric->timestamp == '\0') {
        metric->timestamp = strdup(mf_api_get_time());
    }

    sprintf(curl_data,
        "{ \
            \"@timestamp\": \"%s\", \
            \"host\":\"%s\", \
            \"task\":\"%s\", \
            \"type\": \"%s\", \
            \"%s\": \"%s\" \
        }",
        metric->timestamp,
        status->hostname,
        status->task,
        metric->type,
        metric->name,
        metric->value
    );

    char URL[256];
    sprintf(URL, "%s/%s/%s/%s?task=%s",
        status->server,
        status->path,
        status->workflow,
        status->experiment_id,
        status->task
    );

    return publish_json(URL, curl_data);
}

char*
mf_api_raw_update(const char* json)
{
    char* curl_data = strdup(json);
    char URL[256];

    sprintf(URL, "%s/%s/%s/%s?task=%s",
        status->server,
        status->path,
        status->workflow,
        status->experiment_id,
        status->task
    );

    return publish_json(URL, curl_data);
}

// Wrapper to be called from Fortran (different interface)
// Currently the timestamp passed to this function is ignored and
//  a local timestamp is being generated and used.
void mf_api_update_(
    const char* timestamp,
    const char* type,
    const char* name,
    const char* value)
{
   mf_metric* metric = malloc(sizeof(mf_metric));
   // Ignore passed timestamp value and generate local one
   //metric->timestamp = timestamp;
   char *tempTimestamp = mf_api_get_time();
   metric->timestamp = tempTimestamp;
   metric->type = type;
   metric->name = name;
   metric->value = value;
   mf_api_update(metric);
   //printf("\nTimestamp: %s\n", tempTimestamp);
   //printf("\nType: %s\n", type);
   //printf("\nName: %s\n", name);
   //printf("\nValue: %s\n", value);
   free(tempTimestamp);
   free(metric);
}


static void
to_lowercase(char* word, int length)
{
    int i;
    for(i = 0; i < length; i++) {
        word[i] = tolower(word[i]);
    }
}