/*
 *  worldsens_debug.h
 *  
 *
 *  Created by Loic Lemaitre on 29/07/09.
 *  Copyright 2009 __WorldSens__. All rights reserved.
 *
 */
#ifndef _WORLDSENS_DEBUG_H
#define _WORLDSENS_DEBUG_H


/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
#if defined(DEBUG)
#define WSNET_S_DBG      1
#define WSNET_S_EXC_DBG  1
#else
#define WSNET_S_DBG      0
#define WSNET_S_EXC_DBG  0
#endif

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

#    define WSNET_S_DBG_OUT(x...) fprintf(stderr, x)

#if WSNET_S_DBG != 0
#    define WSNET_S_DBG_DBG(x...) fprintf(stderr, x)
#else
#    define WSNET_S_DBG_DBG(x...) do { } while (0)
#endif

#if WSNET_S_EXC_DBG != 0
#    define WSNET_S_DBG_EXC(x...) fprintf(stderr, x)
#else
#    define WSNET_S_DBG_EXC(x...) do { } while (0)
#endif



#endif
