#include <iostream>
#include "main.h"
using namespace std;
using namespace Cyan;

void CmdWeatherOneDay(Message m)
{
	string txt = m.MessageChain.GetPlainTextFirst();
	if (txt != "今日天气" && txt != "今天天气" && txt != "一天天气" && txt != "查一天天气") return;

	LOG(INFO) << "[" << m.Sender.ToInt64() << "] 使用 [今日天气] 指令";

	try
	{
		auto mc = MessageChain();
		//25小时内的天气（温度、降水量）
		auto weatherOneDays = JwApi.GetWeatherOneDay();
		for (int i = weatherOneDays.size() - 1; i >= 0; i--)
		{
			std::string pos = (weatherOneDays[i].Position == "SF") ? "科学城" : "南岸";
			std::string day = (weatherOneDays[i].Day == "today") ? "今天" : "明天";
			mc.Plain("地点:").Plain(pos).Plain("\n");
			mc.Plain(day).Plain(" ").Plain(weatherOneDays[i].Hour).Plain("点\n");
			mc.Plain("温度:").Plain(weatherOneDays[i].Temperature).Plain("℃\n");
			mc.Plain("降水量:").Plain(weatherOneDays[i].Precipitation).Plain("mm");
			m.Reply(mc);
			mc.Clear();
		}

	}
	catch (const std::exception& ex)
	{
		LOG(ERROR) << "[" << m.Sender.ToInt64() << "] 使用 [今日天气] 指令出现异常: " << ex.what();
		try
		{
			m.Reply(MessageChain().Plain("出现错误："s + ex.what()));
		}
		catch (const exception& ex)
		{
			LOG(ERROR) << "[" << m.Sender.ToInt64() << "] 使用 [今日天气] 指令出现异常" << ex.what();
		}
	}

}