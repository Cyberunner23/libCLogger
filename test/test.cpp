
#include "../include/CLogger.hpp"
#include "../include/Assert.hpp"
#include "../include/Sink.hpp"


class StdOut : public Sink
{
    void append(LogMessage& message) final
    {
        std::cout << message.message;
    }
};


int main(int argc, char** argv)
{

    //Add sinks

    std::shared_ptr<StdOut> stdOut = std::make_shared<StdOut>();

    auto thing = std::static_pointer_cast<Sink>(stdOut);

    {
        CLogger::getInstance()->addSink(thing);
    }
    CLogger::getInstance()->start();

    LogMessage msg; 
    msg.level = INFO; 
    msg.sinkID = 0; 
    msg.fileName = __FILE__; 
    msg.lineNumber = __LINE__; 
    msg.logTime = time(nullptr); 
    LogCapture(std::move(msg)).stream() << "hey1";

    LogMessage msg2;
    msg2.level = INFO;
    msg2.sinkID = 0;
    msg2.fileName = __FILE__;
    msg2.lineNumber = __LINE__;
    msg2.logTime = time(nullptr);
    LogCapture(std::move(msg2)).stream() << "hey";
    

    return 0;
}
