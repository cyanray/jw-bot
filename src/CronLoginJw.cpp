#include <iostream>
#include <mirai.h>
#include "main.h"
#include <glog/logging.h>
using namespace std;
using namespace Cyan;

void CronJobLoginJw(Cyan::MiraiBot& bot)
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::minutes(120));
		try
		{
			LOG(INFO) << "登录教务网中...";
			JwApi.Login(AppConfig["QzJwAdminUid"], AppConfig["QzJwAdminPwd"]);
			LOG(INFO) << "成功登录教务网!";
		}
		catch (const exception& ex)
		{
			LOG(ERROR) << "登录教务网出错: " << ex.what();
		}
	}
}