#include <iostream>
#include "main.h"
using namespace std;
using namespace Cyan;

void CmdWeatherOneWeek(Message m)
{
	string txt = m.MessageChain.GetPlainTextFirst();
	if (txt != "一周天气" && txt != "七天天气" && txt != "查一周天气" && txt != "本周天气") return;

	LOG(INFO) << "[" << m.Sender.ToInt64() << "] 使用 [一周天气] 指令";

	try
	{
		MessageChain mc;
		auto weathers = JwApi.GetWeather();
		//cout << "wea size:" << weathers.size() << endl;
		for (size_t i = 0; i < weathers.size(); i++)
		{
			string pos = (weathers[i].Position == "SF") ? "科学城" : "南岸";
			mc.Plain("地点：").Plain(pos).Plain("\n");
			mc.Plain("日期：").Plain(weathers[i].Date).Plain("\n");
			mc.Plain("天气：").Plain(weathers[i].Weather).Plain("\n");
			if (weathers[i].MaxTemperature == "None")
			{
				mc.Plain("最小温度：").Plain(weathers[i].MinTemperature);
			}
			else
			{
				mc.Plain("温度：").Plain(weathers[i].MinTemperature).Plain("-").Plain(weathers[i].MaxTemperature);
			}

			m.Reply(mc);
			mc.Clear();
		}
	}
	catch (const std::exception& ex)
	{
		LOG(ERROR) << "[" << m.Sender.ToInt64() << "] 使用 [一周天气] 指令时出现异常: " << ex.what();
		try
		{
			m.Reply(MessageChain().Plain("出现错误："s + ex.what()));
		}
		catch (const exception& ex)
		{
			LOG(ERROR) << "[" << m.Sender.ToInt64() << "] 使用 [一周天气] 指令时出现异常: " << ex.what();
		}
	}

}