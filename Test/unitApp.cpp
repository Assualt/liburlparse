#include "unitApp.h"
#include <memory>
class TestApp : public UnitApp{
public:
    TestApp(void) : UnitApp("testApp","test For UnitApp"){
        std::cout << " run in TestApp constroctor" << std::endl;
    }
protected:
    virtual bool onInitAppArgs(argstream &arg);
    virtual bool onInitAppConf(TConfigurationManager &conf);
    virtual TRetCode onRun(void) override;

protected:
    std::string m_strTest;
};

bool TestApp::onInitAppArgs(argstream &arg){
    // arg >> parameter("desc","testApp description", true);
    std::cout << " run in TestApp onInitAppArgs" << std::endl;
    return true;
}


bool TestApp::onInitAppConf(TConfigurationManager &conf){
    std::cout << " run in TestApp onInitAppConf" << std::endl;
    return true;
}

TRetCode TestApp::onRun(void){
    std::cout << " run in TestApp onRun" << std::endl;
    return TRetCode(0);
}



int main(int argc,char **argv){
    std::auto_ptr<UnitApp> _au(new TestApp);
    return _au->run(argc,argv).errNo();
}