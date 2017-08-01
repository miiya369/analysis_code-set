//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup Potential
 * @brief   Header file for potential class
 * @author  Takaya Miyamoto
 * @since   Mon Jan 30 08:58:23 JST 2017
 */
//--------------------------------------------------------------------------

#ifndef POTENTIAL_H
#define POTENTIAL_H

#include <R_correlator/R_correlator.h>

namespace potential
{
   void tensor_potential(R_CORRELATOR&, R_CORRELATOR&,
                         R_CORRELATOR&, R_CORRELATOR&, R_CORRELATOR&);
   
   void tensor_potential(R_CORRELATOR&, R_CORRELATOR&,
                         const R_CORRELATOR&, const R_CORRELATOR&
                         const R_CORRELATOR&, const R_CORRELATOR&
                         const R_CORRELATOR&, const R_CORRELATOR&);
}

#endif
