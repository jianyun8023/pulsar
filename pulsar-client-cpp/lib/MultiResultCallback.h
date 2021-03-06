/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
#pragma once

#include <pulsar/ConsumerConfiguration.h>  // for ResultCallback

#include <atomic>
#include <memory>

namespace pulsar {

class MultiResultCallback {
   public:
    MultiResultCallback(ResultCallback callback, int numToComplete)
        : callback_(callback),
          numToComplete_(numToComplete),
          numCompletedPtr_(std::make_shared<std::atomic_int>(0)) {}

    void operator()(Result result) {
        if (result == ResultOk) {
            if (++(*numCompletedPtr_) == numToComplete_) {
                callback_(result);
            }
        } else {
            callback_(result);
        }
    }

   private:
    ResultCallback callback_;
    const int numToComplete_;
    const std::shared_ptr<std::atomic_int> numCompletedPtr_;
};

}  // namespace pulsar
