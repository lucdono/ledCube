/*
 * 3D LED CUBE
 *
 * Copyright (C) 2021 Luca D'Onofrio.
 *
 * This file is part of LEDCube Project
 *
 * LEDCube is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * LEDCube is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _BITFIELDS_H_
#define _BITFIELDS_H_

/******************************************************************************
 * Defines
 ******************************************************************************/
/*
 * Create value with bit n set.
 */
#define 	BIT(n)   ( 1LL<<(n) )

/*
 * Shift a by n.
 */
#define 	BIT_SHIFT(a, n)   ( (a)<<(n) )

/*
 * Create a bitmask len bits long.
 */
#define 	BIT_MASK(len)   ( BIT(len)-1 )

/*
 * Set the flag bits in word.
 */
#define 	BIT_SET(y, flag)   ( y |= (flag) )

/*
 * Clear the flag bits in word.
 */
#define 	BIT_CLEAR(y, flag)   ( y &= ~(flag) )

/*
 * Flip the flag bits in word.
 */
#define 	BIT_FLIP(y, flag)   ( y ^= (flag) )

/*
 * Test whether all the flag bits in word are set.
 */
#define 	BIT_EQ(y, flag)   ( ((y)&(flag)) == (flag) )

/*
 * Create a bitmask of length len starting at bit shift.
 */
#define 	BF_MASK(shift, len)   ( BIT_MASK(len)<<(shift) )

/*
 * Retrieve a bitfield mask of length starting at bit shift from y.
 */
#define 	BF_GET(y, shift, len)   ( ((y)>>(shift))&(BIT_MASK(len)) )

/*
 * Prepare a bitmask for insertion or combining.
 */
#define 	BF_PREP(x, shift, len)   ( ((x)&BIT_MASK(len))<<(shift) )

/*
 * Insert a new bitfield value x into y.
 */
#define 	BF_SET(y, x, shift, len)   ( y= ((y) &~ BF_MASK(shift, len)) | BF_PREP(x, shift, len) )

#endif /* _BITFIELDS_H_ */
