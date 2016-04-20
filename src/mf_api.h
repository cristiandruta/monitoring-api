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
#ifndef MF_API_H_
#define MF_API_H_

extern char MF_WORKFLOW_EXECUTION_ID[22];

typedef struct mf_metric_t mf_metric;

struct mf_metric_t {
    const char* timestamp;
    const char* type;
    const char* name;
    const char* value;
} mf_metric_t;

void mf_api_init(
    const char* server,
    const char* experiment_id,
    const char* workflow,
    const char* task
);

char* mf_api_update(mf_metric* metric);

char* mf_api_raw_update(const char* json);

#endif