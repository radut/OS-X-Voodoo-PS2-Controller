/*
 * Copyright (c) 1998-2000 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.1 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

#include "ApplePS2MouseDevice.h"
#include "VoodooPS2Controller.h"

// =============================================================================
// ApplePS2MouseDevice Class Implementation
//

#define super IOService
OSDefineMetaClassAndStructors(ApplePS2MouseDevice, IOService);

bool ApplePS2MouseDevice::attach(IOService * provider)
{
  if( !super::attach(provider) )  return false;

  assert(_controller == 0);
  _controller = (ApplePS2Controller *)provider;
  _controller->retain();

  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void ApplePS2MouseDevice::detach( IOService * provider )
{
  assert(_controller == provider);
  _controller->release();
  _controller = 0;

  super::detach(provider);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void ApplePS2MouseDevice::installInterruptAction(OSObject *         target,
                                                 PS2InterruptAction action)
{
  _controller->installInterruptAction(kDT_Mouse, target, action);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void ApplePS2MouseDevice::uninstallInterruptAction()
{
  _controller->uninstallInterruptAction(kDT_Mouse);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void ApplePS2MouseDevice::installPowerControlAction(OSObject *            target,
                                                    PS2PowerControlAction action)
{
  _controller->installPowerControlAction(kDT_Mouse, target, action);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void ApplePS2MouseDevice::uninstallPowerControlAction()
{
  _controller->uninstallPowerControlAction(kDT_Mouse);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

PS2Request * ApplePS2MouseDevice::allocateRequest(int max)
{
  return _controller->allocateRequest(max);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void ApplePS2MouseDevice::freeRequest(PS2Request * request)
{
  _controller->freeRequest(request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool ApplePS2MouseDevice::submitRequest(PS2Request * request)
{
  return _controller->submitRequest(request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void ApplePS2MouseDevice::submitRequestAndBlock(PS2Request * request)
{
  _controller->submitRequestAndBlock(request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void ApplePS2MouseDevice::installMessageAction(OSObject* target, PS2MessageAction action)
{
  _controller->installMessageAction(kDT_Mouse, target, action);
}

void ApplePS2MouseDevice::uninstallMessageAction()
{
  _controller->uninstallMessageAction(kDT_Mouse);
}

void ApplePS2MouseDevice::dispatchKeyboardMessage(int message, void *data)
{
  _controller->dispatchMessage(kDT_Keyboard, message, data);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void ApplePS2MouseDevice::setCommandByte(UInt8 setBits, UInt8 clearBits)
{
    _controller->setCommandByte(setBits, clearBits);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void ApplePS2MouseDevice::lock()
{
    _controller->lock();
}

void ApplePS2MouseDevice::unlock()
{
    _controller->unlock();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

OSMetaClassDefineReservedUnused(ApplePS2MouseDevice, 0);
OSMetaClassDefineReservedUnused(ApplePS2MouseDevice, 1);
OSMetaClassDefineReservedUnused(ApplePS2MouseDevice, 2);
OSMetaClassDefineReservedUnused(ApplePS2MouseDevice, 3);
OSMetaClassDefineReservedUnused(ApplePS2MouseDevice, 4);
OSMetaClassDefineReservedUnused(ApplePS2MouseDevice, 5);
OSMetaClassDefineReservedUnused(ApplePS2MouseDevice, 6);
OSMetaClassDefineReservedUnused(ApplePS2MouseDevice, 7);
