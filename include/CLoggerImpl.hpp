/*
Copyright 2018 Alex Frappier Lachapelle

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#pragma once

#include <atomic>
#include <memory>
#include <thread>
#include <vector>

#include "3rdParty/blockingconcurrentqueue.h"

#include "Assert.hpp"
#include "LogMessage.hpp"
#include "Singleton.hpp"
#include "Sink.hpp"

class CLoggerImpl
{

public:

    CLoggerImpl() : _isRunning(false) {}
    ~CLoggerImpl()
    {
        stop();
    }


    unsigned long addSink(std::shared_ptr<Sink>& sink)
    {
        assert_msg(!_isRunning.load(), "Sinks must be added before starting the logger.");
        _sinks.push_back(sink);
        return _sinks.size() - 1;
    }


    void start()
    {
        _isRunning.store(true);
        _workerThread = std::thread(&CLoggerImpl::run, this);
    }


    void log(LogMessage&& message)
    {
        static thread_local moodycamel::ProducerToken token(_logQueue);

        assert_msg(message.sinkID < getSinkCount(), "You must use a valid sink id when sending a log.");

        if (_isRunning.load())
        {
            _logQueue.enqueue(token, std::move(message));
        }
    }


    unsigned long getSinkCount()
    {
        return _sinks.size();
    }

private:

    std::atomic<bool> _isRunning;
    std::thread _workerThread;
    std::vector<std::shared_ptr<Sink>> _sinks;

    moodycamel::BlockingConcurrentQueue<LogMessage> _logQueue;


    void stop()
    {
        LogMessage flushEndMsg;
        flushEndMsg.sinkID = (unsigned int)-1; //Intentional underflow

        _isRunning.store(false);
        _logQueue.enqueue(flushEndMsg);

        if (_workerThread.joinable())
        {
            _workerThread.join();
        }
    }


    void run()
    {

        moodycamel::ConsumerToken token(_logQueue);

        LogMessage message{};

        while (true)
        {

            _logQueue.wait_dequeue(token, message);

            if (!_isRunning.load())
            {
                break;
            }

            if (_sinks[message.sinkID] != nullptr)
            {
                _sinks[message.sinkID]->append(message);
            }
        }

        //Flush
        do
        {
            //Logging has been stopped, this was added to wake up the worker if ever the queue was empty.
            //We can safely ignore it.
            if (message.sinkID == -1)
            {
                continue;
            }

            if (_sinks[message.sinkID] != nullptr)
            {
                _sinks[message.sinkID]->append(message);
            }

        } while(_logQueue.try_dequeue(message));
    }

};

using CLogger = Singleton<CLoggerImpl>;
