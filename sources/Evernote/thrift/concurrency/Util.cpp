/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include "Util.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <time.h>

#include <sys/time.h>
namespace apache { namespace thrift { namespace concurrency {

int64_t Util::currentTimeTicks(int64_t ticksPerSec) {
  int64_t result;

  struct timeval now;
  int ret = gettimeofday(&now, NULL);
  assert(ret == 0);
  toTicks(result, now, ticksPerSec);

  return result;
}

}}} // apache::thrift::concurrency
