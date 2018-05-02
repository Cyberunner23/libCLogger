
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

    CLogger::getInstance()->addSink(thing);
    CLogger::getInstance()->start();

    LOG(INFO) << "hey1";

    LOG(WARNING) << "hey2";

    return 0;
}
