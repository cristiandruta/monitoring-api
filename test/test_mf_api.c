/*
 * Copyright 2016 High Performance Computing Center, Stuttgart
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
#include <stdlib.h>
#include <string.h>

#include "CuTest.h"
#include "mf_api.h"

void
Test_initialize(CuTest *tc)
{
    const char* server = "http://localhost:3030";
    const char* username = "test_user";
    const char* application = "myApp";

    const char* experiment_id = mf_api_new(server, username, application);
    CuAssertTrue(tc, strlen(experiment_id) == strlen("AVQzilzjcIVzfhf1PDL3"));
}

void
Test_configure(CuTest *tc)
{
    const char* server = "http://localhost:3030";
    const char* experiment_id = "myID_1235";
    const char* username = "test_USER";
    const char* application = "myApp";

    mf_api_set(server, experiment_id, username, application);
    CuAssertStrEquals(tc, "http://localhost:3030", mf_api_get_server());
    CuAssertStrEquals(tc, "myID_1235", mf_api_get_id());
    CuAssertStrEquals(tc, "test_user", mf_api_get_user()); /* lowercase! */
    CuAssertStrEquals(tc, "myapp", mf_api_get_application()); /* lowercase! */
}

void
Test_update(CuTest *tc)
{
    const char* server = "http://localhost:3030";
    const char* experiment_id = "myID_1235";
    const char* username = "test_user";
    const char* application = "myApp";

    mf_api_set(server, experiment_id, username, application);

    /* define metric */
    mf_metric* metric = malloc(sizeof(mf_metric));
    metric->timestamp = mf_api_get_time();
    metric->type = "foobar";
    metric->name = "progress (%)";
    metric->value = "20";

    char* response = mf_api_update(metric);
    CuAssertTrue(tc, strstr(response, "error") == NULL);
}

CuSuite* CuGetSuite(void)
{
    CuSuite* suite = CuSuiteNew();

    // mf_api_new
    SUITE_ADD_TEST(suite, Test_initialize);

    // mf_api_set
    SUITE_ADD_TEST(suite, Test_configure);

    // mf_api_update
    SUITE_ADD_TEST(suite, Test_update);

    return suite;
}