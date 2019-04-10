/*
 * This file is part of the GROMACS molecular simulation package.
 *
 * Copyright (c) 2016,2017, by the GROMACS development team, led by
 * Mark Abraham, David van der Spoel, Berk Hess, and Erik Lindahl,
 * and including many others, as listed in the AUTHORS file in the
 * top-level source directory and at http://www.gromacs.org.
 *
 * GROMACS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * GROMACS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GROMACS; if not, see
 * http://www.gnu.org/licenses, or write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.
 *
 * If you want to redistribute modifications to GROMACS, please
 * consider that scientific software is very special. Version
 * control is crucial - bugs must be traceable. We will be happy to
 * consider code for inclusion in the official distribution, but
 * derived work must not be called official GROMACS. Details are found
 * in the README & COPYING files - if they are missing, get the
 * official version at http://www.gromacs.org.
 *
 * To help us fund GROMACS development, we humbly ask that you cite
 * the research papers on the package. Check out http://www.gromacs.org.
 */

/*! \libinternal \file
 *  \brief Defines the CUDA 3D-FFT functions for PME.
 *
 *  \author Aleksei Iupinov <a.yupinov@gmail.com>
 */

#ifndef GMX_EWALD_PME_3DFFT_CUH
#define GMX_EWALD_PME_3DFFT_CUH

/*#include "gromacs/fft/fft.h"        // for the enum gmx_fft_direction*/
/*#include <hcc_detail/hip_runtime_api.h>*/
#include <hipfft.h>                  // for the cufft types
#include <rocfft.h>

#ifndef GMX_FFT_DIRECTION
#define GMX_FFT_DIRECTION 
enum gmx_fft_direction
{
    GMX_FFT_FORWARD,         /**< Forward complex-to-complex transform  */
    GMX_FFT_BACKWARD,        /**< Backward complex-to-complex transform */
    GMX_FFT_REAL_TO_COMPLEX, /**< Real-to-complex valued FFT            */
    GMX_FFT_COMPLEX_TO_REAL  /**< Complex-to-real valued FFT            */
};
#endif

struct PmeGpu;

/*! \brief \internal A 3D FFT class for performing R2C/C2R transforms
 * \todo Make this class actually parallel over multiple GPUs
 */
class GpuParallel3dFft
{
    hipfftHandle   planR2C_;
    hipfftHandle   planC2R_;
    hipfftReal    *realGrid_;
    hipfftComplex *complexGrid_;

    public:
        /*! \brief
         * Constructs CUDA FFT plans for performing 3D FFT on a PME grid.
         *
         * \param[in] pmeGpu                  The PME GPU structure.
         */
        GpuParallel3dFft(const PmeGpu *pmeGpu);
        /*! \brief Destroys CUDA FFT plans. */
        ~GpuParallel3dFft();
        /*! \brief Performs the FFT transform in given direction */
        void perform3dFft(gmx_fft_direction dir);
};

#endif
