/***********************************************************************************************************************
 * 
 * > QuadroCore <
 * 
 * Copyright (C) 2012 by Chris Channing
 *
 ***********************************************************************************************************************
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
 * an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations under the License.
 *
 ***********************************************************************************************************************/

#ifndef CPU_H_
#define CPU_H_

#define CPU_HZ ((uint32_t)24000000)
#define CPU_MHZ (CPU_HZ / 1000000)

#include "ccp.h"
#include "clock.h"
#include "pmic.h"
#include "port.h"

void sleep(uint16_t value);

#endif /* CPU_H_ */