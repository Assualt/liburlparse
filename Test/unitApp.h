#ifndef _UNIT_APP_H
#define _UNIT_APP_H
#include <iostream>
#include <sstream>
#include <map>
#include <getopt.h>
#include <vector>
#include <string.h>
using namespace std;

enum argType
{
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING
};

struct parameter
{
public:
    char m_strShortArg;
    std::string m_strFullArg;
    std::string m_strDesciption;
    argType m_nType;
    bool m_bNecessary;
    parameter(const char s, const std::string &f, const std::string &d, argType nType, bool n = false)
        : m_strShortArg(s), m_strFullArg(f), m_strDesciption(d), m_nType(nType), m_bNecessary(n)
    {
    }
    parameter(const std::string &f, const std::string &d, argType nType, bool n = false)
        : m_strShortArg(' '), m_strFullArg(f), m_strDesciption(d), m_nType(nType), m_bNecessary(n)
    {
    }
};

class argstream
{
public:
    argstream()
    {
        m_nIndex = 1;
    }
    argstream &arg(const parameter par)
    {
        std::cout << " >> Args << " << std::endl;
        m_argsMap[par.m_strFullArg] = par.m_nType;
        //参数1
        m_strLongArgsList.push_back(par.m_strFullArg.c_str());
        //参数2
        m_strNecessaryList.push_back(par.m_bNecessary ? required_argument : no_argument);
        //参数3
        //参数4
        if (par.m_strShortArg >= 'a' && par.m_strShortArg <= 'z')
        {
            m_strFlagsList.push_back(0); //should append NULL
            m_strValList.push_back(par.m_strShortArg);
        }
        else
        {
            m_strFlagsList.push_back(1); //should append &flag
            m_strValList.push_back(++m_nIndex);
        }
        if (par.m_strShortArg >= 'a' && par.m_strShortArg <= 'z')
        {
            m_strShortArg.push_back(par.m_strShortArg);
            if (par.m_bNecessary)
            {
                m_strShortArg.push_back(':');
            }
            m_strShortArgsList.push_back(par.m_strShortArg);
        }
        return *this;
    }
    void parseArgs(int argc, char **argv)
    {
        while (1)
        {
            int optIndex = 0;
            int flag = 0;
            std::vector<option> options = getArgsOption(flag);
            int c = getopt_long(argc, argv, m_strShortArg.c_str(), options.data(), &optIndex);
            if (c == -1)
                break;
            if (c == 0)
            {
                for (int i = 0; i < m_strFlagsList.size(); ++i)
                {
                    if (m_strFlagsList[i] == flag)
                    {
                        if(!argv[optind])
                            m_argsVal[options[i].name] = "";
                        else
                            m_argsVal[options[i].name] =argv[optind];
                        std::cout << "Get Opt arg:" << options[i].name << " val:" << argv[optind] << std::endl;
                        break;
                    }
                }
            }
            else
            {
                for (int i = 0; i < m_strValList.size(); ++i)
                {
                    if (m_strValList[i] == c)
                    {
                        if(!argv[optind])
                            m_argsVal[options[i].name] = "";
                        else
                            m_argsVal[options[i].name] =argv[optind];
                        // std::cout << "Get Opt arg:" << options[i].name << " val:" << argv[optind] << std::endl;
                        break;
                    }
                }
            }
        }
    }

protected:
    std::vector<option> getArgsOption(int flag)
    {
        std::vector<option> ret;
        for (int i = 0; i < m_strLongArgsList.size(); ++i)
        {
            option o;
            o.name = m_strLongArgsList[i].c_str();
            o.has_arg = m_strNecessaryList[i];
            o.flag = m_strFlagsList[i] == 0 ? NULL : &flag;
            o.val = m_strValList[i];
            ret.push_back(o);
        }
        return ret;
    }

private:
    std::map<std::string, argType> m_argsMap;
    std::map<std::string, std::string> m_argsVal;
    std::string m_strShortArg;
    std::vector<std::string> m_strLongArgsList;
    std::vector<char> m_strShortArgsList;
    std::vector<int> m_strNecessaryList;
    std::vector<int> m_strFlagsList;
    std::vector<int> m_strValList;
    size_t m_nIndex;
};

class TConfigurationManager
{
public:
    TConfigurationManager()
    {
    }
};

class TRetCode
{
public:
    TRetCode(int nCode) : m_nCode(nCode)
    {
    }
    TRetCode(int nCode, const std::string &ErrMsg) : m_nCode(nCode), m_strErrMsg(ErrMsg)
    {
    }

public:
    int errNo() const
    {
        return m_nCode;
    }

private:
    int m_nCode;
    std::string m_strErrMsg;
};

class UnitApp
{
public:
    UnitApp(const std::string &appName, const std::string &appDesc);

protected:
    virtual bool onInitAppArgs(argstream &arg) = 0;
    virtual bool onInitAppConf(TConfigurationManager &conf) = 0;
    virtual TRetCode onRun(void) = 0;

protected:
    void onInit();
    void parseAppArgs(int argc, char **argv);

public:
    TRetCode run(int argc, char **argv);

protected:
    std::string m_strAppName;
    std::string m_strAppDesc;
    argstream m_argstream;
    TConfigurationManager m_confMgr;
    int m_nLogLevel;
};

UnitApp::UnitApp(const std::string &appName, const std::string &appDesc) : m_strAppName(appName), m_strAppDesc(appDesc)
{
    m_nLogLevel = 0;
    std::cout << " run in UnitApp constructor " << std::endl;
}
void UnitApp::parseAppArgs(int argc, char **argv)
{
    m_argstream.parseArgs(argc, argv);
    std::cout << " run in parse app args" << std::endl;
}

TRetCode UnitApp::run(int argc, char **argv)
{
    m_argstream.arg(parameter('d', "log_level", "show logs level", TYPE_INT, false));
    m_argstream.arg(parameter('l', "log_file", "show log files", TYPE_STRING, false));
    onInitAppArgs(m_argstream);
    parseAppArgs(argc, argv);
    onInitAppConf(m_confMgr);
    std::cout << " run in UnitApp run " << std::endl;
    return onRun();
}

#endif
