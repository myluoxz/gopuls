#ifndef _GO_PLUS_APP_H_
#define _GO_PLUS_APP_H_

#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>

#include "GoGameWatcher.h"

//using Poco::Util::ServerApplication;
//using Poco::Util::Application;
//using Poco::Util::Option;
//using Poco::Util::OptionSet;
//using Poco::Util::HelpFormatter;
//

/*
 * The main application class.
 * This class handles command-line arguments and
 * configuration files.
 *
 * Start the application with the help
 * option (/help on Windows, --help on Unix) for
 * the available command line options.
 *
 */
class GoPlusApp: public Poco::Util::ServerApplication
{
private:

   GoGameWatcher m_goGameWatcher;

   // ������
   //WGMServer       m_server;

   // ��ʱ��
   //WgmTimer        m_timer;

public:

   /** 
    * ���캯��
    * 
    */
   GoPlusApp();


   ~GoPlusApp();

protected:
   void initialize(Application& self);

   void uninitialize();

   void defineOptions(Poco::Util::OptionSet& options);

   void handleOption(const std::string& name, const std::string& value);

   void displayHelp();

   int main(const std::vector<std::string>& args);
	
private:
   bool _helpRequested;
   //friend class WGMServerConnection;
};

#endif /* end of WGM.h */
