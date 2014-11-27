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
 * tracker.cc
 *  Created on: 2013-9-29
 *      Author: ghdawn
 */

#include "lktracker.h"
#include "../itrvision.h"
#include <math.h>
#include <stdio.h>
namespace itr_vision
{
    LKTracker::LKTracker()
    {
        minDet = 100;
        level = 2;
        stopth = 0.1f;
        max_residue = 10;
        max_iterations=10;
    }

    LKTracker::~LKTracker()
    {
    }


    void LKTracker::_ComputeDt(const Point2D &U, const Point2D &V, S32 L, S32 hw, S32 *dt)
    {
        S32 y, x, g1, g2;
        for (y = -hw; y <= hw; ++y)
            for (x = -hw; x <= hw; ++x)
            {
                g1 = Scale::Interpolation(last->img[L], U.Y + y, U.X + x);
                g2 = Scale::Interpolation(current->img[L], V.Y + y, V.X + x);
                *dt++ = g1 - g2;
            }
    }
    void LKTracker::_ComputeGrad(const Point2D &U,const  Point2D &V, S32 L, S32 hw, S32 *dx, S32 *dy)
    {
        S32 y, x, g1, g2;
        F32 uy,ux,vy,vx;
        for (y = -hw; y <= hw; ++y)
            for (x = -hw; x <= hw; ++x)
            {
                uy=U.Y+y;
                ux=U.X+x;
                vy=V.Y+y;
                vx=V.X+x;
                g1 = Scale::Interpolation(last->gradx[L], uy, ux);
                g2 = Scale::Interpolation(current->gradx[L], vy, vx);
                *dx++ = g1 + g2;
                g1 = Scale::Interpolation(last->grady[L], uy, ux);
                g2 = Scale::Interpolation(current->grady[L], vy, vx);
                *dy++ = g1 + g2;
            }
    }
    void LKTracker::_ComputeGrad2(const Point2D &U, S32 L, S32 hw, S32 *dx, S32 *dy)
    {
        S32 y, x;
        F32 uy,ux;
        for (y = -hw; y <= hw; ++y)
            for (x = -hw; x <= hw; ++x)
            {
                uy=U.Y+y;
                ux=U.X+x;
                *dx++ = Scale::Interpolation(last->gradx[L], uy, ux);
                *dy++ = Scale::Interpolation(last->grady[L], uy, ux);
            }
    }
    S32 LKTracker::_ComputeSum(S32 *a, S32 *b, S32 *sum, S32 length)
    {
        S32 result = 0;
        itr_math::CalculateObj->Multi(a, b, length, sum);
        itr_math::CalculateObj->AddSum(sum, length, result);
        return result;
    }
    S32 LKTracker::_SumDiff(S32 *a, S32 length)
    {
        int ans = 0;
        while (length--)
        {
            ans += *a++;
        }
        return ans;
    }
    LKTracker::TrackResult LKTracker::Compute(const Point2D &U, Point2D &V, int L)
    {
        S32 hw = windowWidth >> 1;
        S32 length = windowWidth * windowWidth;

        F32 det, speedx = 1, speedy = 1;
        S32 gxx, gxy, gyy, ex, ey;
        LKTracker::TrackResult result = Tracked;
        //_ComputeGrad2(U, L, hw, Dx, Dy);
        S32 iteration=0;
        for (; iteration < max_iterations && (fabs(speedx) > stopth || fabs(speedy) > stopth); ++iteration)
        {
            if (U.X - hw < 0 || U.Y - hw < 0 || V.X - hw < 0 || V.Y - hw < 0
                    || U.X + hw >= last->width[L] || U.Y + hw >= last->height[L]
                    || V.X + hw >= last->width[L] || V.Y + hw >= last->height[L])
            {
                result = OOB;
                break;
            }

            _ComputeGrad(U, V, L, hw, Dx, Dy);
            _ComputeDt(U, V, L, hw, Dt);
            gxx = _ComputeSum(Dx, Dx, Sum, length);
            gxy = _ComputeSum(Dx, Dy, Sum, length);
            gyy = _ComputeSum(Dy, Dy, Sum, length);
            ex = _ComputeSum(Dx, Dt, Sum, length);
            ey = _ComputeSum(Dy, Dt, Sum, length);
            det = gxx * gyy - gxy * gxy;
            if (det < minDet)
            {
                result = SmallDet;
                break;
            }
            det = 1 / det;
            speedx = (gyy * ex - gxy * ey) * det;
            speedy = (gxx * ey - gxy * ex) * det;
            V.X += speedx;
            V.Y += speedy;
        }
        if ( V.X < hw || V.Y < hw
                || V.X + hw >= last->width[L] || V.Y + hw >= last->height[L])
        {
            result = OOB;
        }
        _ComputeDt(U, V, L, hw, Dt);
        if (_SumDiff(Dt, length) / (length) > max_residue)
        {
            result = LARGE_RESIDUE;
        }
        if(iteration>=max_iterations)
        {
            result=MAX_ITERATION;
        }
        // TODO  大残差的解决
        return result;
    }

    void LKTracker::Compute(const vector<CommFeaturePoint> &fl, vector<CommFeaturePoint> &flresult, S32 FeatureNum,Pyramid *t0,Pyramid *t1)
    {
        vector<CommFeaturePoint>::const_iterator feat = fl.begin();
        vector<CommFeaturePoint>::iterator featr = flresult.begin();
        Point2D U, V;
        S32 l, i;
        LKTracker::TrackResult result;

        last = t0;
        current = t1;
        int subsampling = current->GetSubsampling();
        F32 subsamplingDiv=1;
        for (i = 0; i < level; ++i)
        {
            subsamplingDiv*=subsampling;
        }
        subsamplingDiv=1/subsamplingDiv;
        while (feat != fl.begin()+FeatureNum)
        {
            if (feat->Quality < 0)
            {
                ++feat;
                ++featr;
                continue;
            }
            U.X = (feat->X)*subsamplingDiv;
            U.Y = (feat->Y)*subsamplingDiv;
            V.X = U.X;
            V.Y = U.Y;
            for (l = level - 1; l >= 0; --l)
            {
                U.X *= subsampling;
                U.Y *= subsampling;
                V.X *= subsampling;
                V.Y *= subsampling;
                result = Compute(U, V, l);
                if (result != Tracked)
                {
                    break;
                }
            }

            featr->X = V.X;
            featr->Y = V.Y;
            featr->Quality = -result;
            ++featr;
            ++feat;
        }
    }

} // namespace itr_vision
