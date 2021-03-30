#include <iostream>
#include "main.h"
//#include <string>

using namespace std;
using namespace Cyan;
//using namespace string;

void CmdStatus(Cyan::Message m)
{
	if (m.MessageChain.GetPlainTextFirst() != "运行状态") return;

	try
	{
		auto mc = MessageChain().Plain("[课表机器人运行状态]\n");
		mc.Plain("总用户数量: ").Plain(UserDb.GetUserCountAll()).Plain(" 人\n");
		mc.Plain("用户数量(17): ").Plain(UserDb.GetUserCount_17()).Plain(" 人\n");
		mc.Plain("用户数量(18): ").Plain(UserDb.GetUserCount_18()).Plain(" 人\n");
		mc.Plain("用户数量(19): ").Plain(UserDb.GetUserCount_19()).Plain(" 人\n");
		mc.Plain("用户数量(20): ").Plain(UserDb.GetUserCount_20()).Plain(" 人");
		m.Reply(mc);
	}
	catch (const std::exception& ex)
	{
		try
		{
			m.Reply(MessageChain().Plain("出现错误："s + ex.what()));
		}
		catch (...)
		{
		}
	}


	MessageChain mcc;
	auto weathers =  JwApi.GetWeather();
	cout <<"wea size:"<< weathers.size() << endl;
	for (size_t i=0;i<weathers.size();i++)
	{
		//mcc.Plain("地点：").Plain(weathers[i].position).Plain("\n");
		string ss = "科学城";
		cout << "滴答滴答：" << ss << endl;
		cout << "地点：" << weathers[i].position << endl;
		mcc.Plain("日期：").Plain(weathers[i].date).Plain("\n");
		mcc.Plain("天气：").Plain(weathers[i].weather).Plain("\n");
		if (weathers[i].maxTem == "None")
		{
			mcc.Plain("最小温度：").Plain(weathers[i].minTem);
		}
		else
		{
			mcc.Plain("温度：").Plain(weathers[i].minTem).Plain("-").Plain(weathers[i].maxTem);
		}
		
		m.Reply(mcc);
		mcc.Clear();
	}


	//25小时内的天气（温度、降水量）
	auto weaOneDays = JwApi.GetWeatherOf24Hours();
	for (int i = weaOneDays.size() - 1; i >= 0; i--)
	{
		mcc.Plain("今/明天").Plain(" ").Plain(weaOneDays[i].hour).Plain("点\n");
		mcc.Plain("温度:").Plain(weaOneDays[i].tem).Plain("℃\n");
		mcc.Plain("降水量:").Plain(weaOneDays[i].precipi).Plain("mm");
		m.Reply(mcc);
		mcc.Clear();
	}

}