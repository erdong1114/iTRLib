/*
 *
 *   Copyright (C) 2013 BUAA iTR Research Center. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS S32ERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * main.cc
 *  Created on: 2013-9-10
 *      Author: buaa
 */

#include "itrbase.h"
#include "platform_test.h"
#include "containertest.h"
#include "mathtest.h"
#include "helpertest.h"
#include "math.h"
#include "ssptest.h"

int main()
{
    //Platform
//    TestAssistMacro();
//    TestDebugSupport();
//    TestMemoryOperation();
//    TestTypedef();
    //Container
//    TestCycleQueue();
    //Math
//    TestMathInit();
//    TestCalculate();
//    TestNumerical();
//    TestStatistics();
//    TestVector();

//    TestCalculateTest();

//    TestMatrix();
//    TestGeometry();
//    TestTransform();
    //test SSPS
//    SSPTest();
    //Protocol
    SEPTest();
    //Test Helper
//    TestGaussianGenerate();
//    TestCalcEff();
    //Finish


//    TestMathdeDeinit();

    TRACE_INFO("OK All");
    return 0;
}

