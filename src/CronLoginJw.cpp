#include <iostream>
#include <mirai.h>
#include "main.h"
using namespace std;
using namespace Cyan;

void CronJobLoginJw(Cyan::MiraiBot& bot)
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::minutes(120));
		try
		{
			JwApi.Login(AppConfig["QzJwAdminUid"], AppConfig["QzJwAdminPwd"]);
		}
		catch (const exception& ex)
		{
			cout << "登录教务网出错: " << ex.what() << endl;	// TODO: log
		}
	}
}