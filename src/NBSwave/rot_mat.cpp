//--------------------------------------------------------------------------
/**
 * @file
 * @ingroup NBS wave function
 * @brief   Definition for rotation matrix
 * @author  Takaya Miyamoto
 * @since   Tue Sep  1 22:25:06 JST 2015
 */
//--------------------------------------------------------------------------

#include <NBSwave/NBSwave.h>

int NBSwave::rot_matrix[384];

void NBSwave::rot_matrix_init()
{
   int R[384] = {
      
      1, 0, 0, 0,                 // E
      0, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1,
      
      1, 0, 0, 0,                 // 6C4 (1)
      0, 0,-1, analysis::xSIZE,
      0, 1, 0, 0,
      0, 0, 0, 1,
      
      0, 0, 1, 0,                 // 6C4 (2)
      0, 1, 0, 0,
      -1, 0, 0, analysis::xSIZE,
      0, 0, 0, 1,
      
      0,-1, 0, analysis::xSIZE,   // 6C4 (3)
      1, 0, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1,
      
      1, 0, 0, 0,                 // 6C4 (4)
      0, 0, 1, 0,
      0,-1, 0, analysis::xSIZE,
      0, 0, 0, 1,
      
      0, 0,-1, analysis::xSIZE,   // 6C4 (5)
      0, 1, 0, 0,
      1, 0, 0, 0,
      0, 0, 0, 1,
      
      0, 1, 0, 0,                 // 6C4 (6)
      -1, 0, 0, analysis::xSIZE,
      0, 0, 1, 0,
      0, 0, 0, 1,
      
      1, 0, 0, 0,                 // 3C2 (1)
      0,-1, 0, analysis::xSIZE,
      0, 0,-1, analysis::xSIZE,
      0, 0, 0, 1,
      
      -1, 0, 0, analysis::xSIZE,   // 3C2 (2)
      0, 1, 0, 0,
      0, 0,-1, analysis::xSIZE,
      0, 0, 0, 1,
      
      -1, 0, 0, analysis::xSIZE,   // 3C2 (3)
      0,-1, 0, analysis::xSIZE,
      0, 0, 1, 0,
      0, 0, 0, 1,
      
      0, 0, 1, 0,                 // 8C3 (1)
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 0, 1,
      
      0,-1, 0, analysis::xSIZE,   // 8C3 (2)
      0, 0,-1, analysis::xSIZE,
      1, 0, 0, 0,
      0, 0, 0, 1,
      
      0, 0,-1, analysis::xSIZE,   // 8C3 (3)
      1, 0, 0, 0,
      0,-1, 0, analysis::xSIZE,
      0, 0, 0, 1,
      
      0,-1, 0, analysis::xSIZE,   // 8C3 (4)
      0, 0, 1, 0,
      -1, 0, 0, analysis::xSIZE,
      0, 0, 0, 1,
      
      0, 1, 0, 0,                 // 8C3 (5)
      0, 0, 1, 0,
      1, 0, 0, 0,
      0, 0, 0, 1,
      
      0, 0, 1, 0,                 // 8C3 (6)
      -1, 0, 0, analysis::xSIZE,
      0,-1, 0, analysis::xSIZE,
      0, 0, 0, 1,
      
      0, 1, 0, 0,                 // 8C3 (7)
      0, 0,-1, analysis::xSIZE,
      -1, 0, 0, analysis::xSIZE,
      0, 0, 0, 1,
      
      0, 0,-1, analysis::xSIZE,	 // 8C3 (8)
      -1, 0, 0, analysis::xSIZE,
      0, 1, 0, 0,
      0, 0, 0, 1,
      
      0, 1, 0, 0,                 // 6C2 (1)
      1, 0, 0, 0,
      0, 0,-1, analysis::xSIZE,
      0, 0, 0, 1,
      
      0,-1, 0, analysis::xSIZE,   // 6C2 (2)
      -1, 0, 0, analysis::xSIZE,
      0, 0,-1, analysis::xSIZE,
      0, 0, 0, 1,
      
      -1, 0, 0, analysis::xSIZE,   // 6C2 (3)
      0, 0,-1, analysis::xSIZE,
      0,-1, 0, analysis::xSIZE,
      0, 0, 0, 1,
      
      -1, 0, 0, analysis::xSIZE,	 // 6C2 (4)
      0, 0, 1, 0,
      0, 1, 0, 0,
      0, 0, 0, 1,
      
      0, 0, 1, 0,                 // 6C2 (5)
      0,-1, 0, analysis::xSIZE,
      1, 0, 0, 0,
      0, 0, 0, 1,
      
      0, 0,-1, analysis::xSIZE,   // 6C2 (6)
      0,-1, 0, analysis::xSIZE,
      -1, 0, 0, analysis::xSIZE,
      0, 0, 0, 1
   };
   
   for (int i=0; i<384; i++) NBSwave::rot_matrix[i] = R[i];
}
