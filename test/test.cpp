
#include "../include/sinks/File.hpp"
#include "../include/CLogger.hpp"


int main(int argc, char** argv)
{

    //Add sinks
    auto fileSink = std::static_pointer_cast<Sink>(std::make_shared<File>("Test.txt"));

    CLogger::getInstance()->addSink(fileSink);
    CLogger::getInstance()->start();

    LOG(INFO) << "hey1";

    LOG(WARNING) << "hey2";

    return 0;
}
