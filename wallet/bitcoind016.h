// Copyright 2019 The Beam Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "bitcoin_bridge.h"

#include "http/http_client.h"

namespace beam
{
    class Bitcoind016: public IBitcoinBridge
    {
    public:
        Bitcoind016() = delete;
        Bitcoind016(io::Reactor& reactor, const std::string& userName, const std::string& pass, const io::Address& address);

        void dumpPrivKey(const std::string& btcAddress, std::function<void(const std::string&, const std::string&)> callback) override;
        void fundRawTransaction(const std::string& rawTx, std::function<void(const std::string&, const std::string&, int)> callback) override;
        void signRawTransaction(const std::string& rawTx, std::function<void(const std::string&, const std::string&, bool)> callback) override;
        void sendRawTransaction(const std::string& rawTx, std::function<void(const std::string&, const std::string&)> callback) override;
        void getRawChangeAddress(std::function<void(const std::string&, const std::string&)> callback) override;
        void createRawTransaction(
            const std::string& withdrawAddress,
            const std::string& contractTxId,
            uint64_t amount,
            int outputIndex,
            Timestamp locktime,
            std::function<void(const std::string&, const std::string&)> callback) override;
        void getTxOut(const std::string& txid, int outputIndex, std::function<void(const std::string&, const std::string&, double, uint16_t)> callback) override;

    protected:
        void sendRequest(const std::string& method, const std::string& params, std::function<void(const std::string&)> callback);

    private:
        HttpClient m_httpClient;
        io::Address m_address;
        std::string m_authorization;
    };
}