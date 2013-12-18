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
 * convolutesquare.h
 *  Created on: 2013-9-27
 *      Author: buaa
 */

#ifndef CONVOLUTESQUARE_H_
#define CONVOLUTESQUARE_H_

#include "itrbase.h"
#include "../image/image.h"

namespace itr_vision
{

    class ConvoluteSquare
    {
        public:
            static const int MAX_KERNEL_WIDTH = 71;
            typedef struct
            {
                int width;
                float data[MAX_KERNEL_WIDTH];
            } ConvolutionKernel;

            ConvoluteSquare();
            ConvoluteSquare(S32 FilterDim, S32 Width, S32 Height);
            virtual ~ConvoluteSquare();
            void Convolute(const ImageGray &Input, F32 *Filter, ImageGray &Output);

            void _computeKernels(float sigma, ConvolutionKernel *gauss,
                                 ConvolutionKernel *gaussderiv);

            void _convolveImageHoriz(const ImageGray &imgin, ConvolutionKernel kernel, ImageGray &imgout);

            void _convolveImageVert(const ImageGray &imgin, ConvolutionKernel kernel, ImageGray &imgout);

            void _KLTComputeGradients(const ImageGray &img, float sigma, ImageGray &gradx,
                                      ImageGray &grady);

            void _convolveSeparate(const ImageGray &imgin, ConvolutionKernel horiz_kernel,
                                   ConvolutionKernel vert_kernel, ImageGray &imgout);
            void _KLTComputeSmoothedImage(const ImageGray &img, float sigma, ImageGray &smooth);
        private:
            ConvolutionKernel gauss_kernel;
            ConvolutionKernel gaussderiv_kernel;
            float sigma_last;
            S16 *multBufferS16;
            S16 *imageBufferS16;
            S32 width;
            S32 height;
            S32 filterDim;
            S32 r;
    };

} // namespace itr_image
#endif // CONVOLUTESQUARE_H_
