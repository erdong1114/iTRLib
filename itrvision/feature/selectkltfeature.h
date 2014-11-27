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
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * selectseature.h
 *  Created on: 2013-10-3
 *      Author: ghdawn
 */

#ifndef SELECTKLTFEATURE_H_
#define SELECTKLTFEATURE_H_

#include "itrbase.h"
#include "../feature/commfeaturepoint.h"
#include "../feature/pyramid.h"
#include "../process/convolutesquare.h"
#include <vector>
using std::vector;
using itr_math::RectangleS;
using itr_math::Matrix;
namespace itr_vision
{

class SelectKLTFeature
{
public:
    SelectKLTFeature(S32 width,S32 height);
    void AddImage(Pyramid *Image);
    S32 SelectGoodFeature(const RectangleF &rect, vector<CommFeaturePoint> &fl, S32 start=0);
    virtual ~SelectKLTFeature();
    int mindist, mineigen;
    S32 windowWidth;
private:
    inline F32 minEigenvalue(F32 gxx, F32 gxy, F32 gyy);
    inline void fillMap(S32 x, S32 y, BOOL *featuremap);
    Pyramid *image;
    F32 *dx,*dy;
    S32 bw;
    S32 width, height;
    BOOL *featuremap;
    ConvoluteSquare conv;
};

} // namespace itr_vision
#endif // SELECTSEATURE_H_
