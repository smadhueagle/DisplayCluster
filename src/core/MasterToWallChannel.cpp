/*********************************************************************/
/* Copyright (c) 2014, EPFL/Blue Brain Project                       */
/*                     Raphael Dumusc <raphael.dumusc@epfl.ch>       */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/*   1. Redistributions of source code must retain the above         */
/*      copyright notice, this list of conditions and the following  */
/*      disclaimer.                                                  */
/*                                                                   */
/*   2. Redistributions in binary form must reproduce the above      */
/*      copyright notice, this list of conditions and the following  */
/*      disclaimer in the documentation and/or other materials       */
/*      provided with the distribution.                              */
/*                                                                   */
/*    THIS  SOFTWARE IS PROVIDED  BY THE  UNIVERSITY OF  TEXAS AT    */
/*    AUSTIN  ``AS IS''  AND ANY  EXPRESS OR  IMPLIED WARRANTIES,    */
/*    INCLUDING, BUT  NOT LIMITED  TO, THE IMPLIED  WARRANTIES OF    */
/*    MERCHANTABILITY  AND FITNESS FOR  A PARTICULAR  PURPOSE ARE    */
/*    DISCLAIMED.  IN  NO EVENT SHALL THE UNIVERSITY  OF TEXAS AT    */
/*    AUSTIN OR CONTRIBUTORS BE  LIABLE FOR ANY DIRECT, INDIRECT,    */
/*    INCIDENTAL,  SPECIAL, EXEMPLARY,  OR  CONSEQUENTIAL DAMAGES    */
/*    (INCLUDING, BUT  NOT LIMITED TO,  PROCUREMENT OF SUBSTITUTE    */
/*    GOODS  OR  SERVICES; LOSS  OF  USE,  DATA,  OR PROFITS;  OR    */
/*    BUSINESS INTERRUPTION) HOWEVER CAUSED  AND ON ANY THEORY OF    */
/*    LIABILITY, WHETHER  IN CONTRACT, STRICT  LIABILITY, OR TORT    */
/*    (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY WAY OUT    */
/*    OF  THE  USE OF  THIS  SOFTWARE,  EVEN  IF ADVISED  OF  THE    */
/*    POSSIBILITY OF SUCH DAMAGE.                                    */
/*                                                                   */
/* The views and conclusions contained in the software and           */
/* documentation are those of the authors and should not be          */
/* interpreted as representing official policies, either expressed   */
/* or implied, of The University of Texas at Austin.                 */
/*********************************************************************/

#include "MasterToWallChannel.h"

#include "MPIChannel.h"
#include "DisplayGroup.h"
#include "ContentWindow.h"
#include "Options.h"
#include "Markers.h"
#include "PixelStreamFrame.h"

MasterToWallChannel::MasterToWallChannel( MPIChannelPtr mpiChannel )
    : mpiChannel_( mpiChannel )
{
}

template< typename T >
void MasterToWallChannel::broadcast( const T& object,
                                     const MPIMessageType type )
{
    const std::string& serializedString = buffer_.serialize( object );

    mpiChannel_->broadcast( type, serializedString );
}

template< typename T >
void MasterToWallChannel::broadcastAsync( const T& object,
                                          const MPIMessageType type )
{
    const std::string serializedStringCopy = asyncBuffer_.serialize( object );

    QMetaObject::invokeMethod( this, "broadcast", Qt::QueuedConnection,
                               Q_ARG( MPIMessageType, type ),
                               Q_ARG( std::string, serializedStringCopy ));
}

void MasterToWallChannel::sendAsync( DisplayGroupPtr displayGroup )
{
    broadcastAsync( displayGroup, MPI_MESSAGE_TYPE_DISPLAYGROUP );
}

void MasterToWallChannel::sendAsync( OptionsPtr options )
{
    broadcastAsync( options, MPI_MESSAGE_TYPE_OPTIONS );
}

void MasterToWallChannel::sendAsync( MarkersPtr markers )
{
    broadcastAsync( markers, MPI_MESSAGE_TYPE_MARKERS );
}

void MasterToWallChannel::send( PixelStreamFramePtr frame )
{
    assert( !frame->segments.empty() && "received an empty frame" );
    broadcast( frame, MPI_MESSAGE_TYPE_PIXELSTREAM );
}

void MasterToWallChannel::sendQuit()
{
    mpiChannel_->sendAll( MPI_MESSAGE_TYPE_QUIT );
}

// cppcheck-suppress passedByValue
void MasterToWallChannel::broadcast( const MPIMessageType type, const std::string data )
{
    mpiChannel_->broadcast( type, data );
}
