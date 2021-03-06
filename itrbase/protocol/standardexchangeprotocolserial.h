//
// Created by ghdawn on 16-1-10.
//

#ifndef ITRLIB_STANDARDEXCHANGEPROTOCOLSERIAL_H
#define ITRLIB_STANDARDEXCHANGEPROTOCOLSERIAL_H

#include "istandardexchangeprocessor.h"
#include "standardexchangepackage.h"
#include "standardexchangepackageserial.h"

namespace itr_protocol
{
    class StandardExchangeProtocolSerial : public IStandardExchangeProcessor
    {
    public:
        U8 S0, S1;
        StandardExchangeProtocolSerial();

        static const int HeaderLength =
                1 + StandardExchangePackage::HeaderLength + StandardExchangePackageSerial::HeaderLength;
        static const int TailLength = StandardExchangePackageSerial::TailLength;

        U32 FillBuffer(const StandardExchangePackage &SEPackage, U8 *buffer);

        U32 ProcessByte(U8 *buffer, S32 offset, S32 length);

        void AddReceiveFun(OnReceiveSEP *fun);

    private:
        U8 receiveBuffer[256 + HeaderLength + TailLength];
        int receiveBufferState;
        vector<OnReceiveSEP *> receiveList;
    };
}


#endif //ITRLIB_STANDARDEXCHANGEPROTOCOLSERIAL_H
