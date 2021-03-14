#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <glog/logging.h>
#include "main.h"
using namespace std;
using namespace Cyan;
using namespace cyanray;

void CmdFreeClassroom(Message m)
{
	static string FreeTimeText[] = { "全天", "上午", "下午", "晚上" };
	try
	{
		string msg_str = m.MessageChain.GetPlainTextFirst();
		string_view msg_view(msg_str);
		if (!msg_view.starts_with("空教室") && !msg_view.starts_with("查空教室"))
		{
			return;
		}

		LOG(INFO) << "[" << m.Sender.ToInt64() << "] 使用 [查空教室] 指令: " << msg_str;

		string date = GetCurrentDate();
		string campus_id = "02";
		string building_id = "0201";
		Jw::FreeTime free_time = Jw::FreeTime::AllDay;

		if (msg_view.find("明天") != msg_view.npos)
		{
			date = GetTomorrowDate();
		}

		if (msg_view.find("A01") != msg_view.npos)
		{
			campus_id = "02";
			building_id = "0201";
		}

		if (msg_view.find("二教") != msg_view.npos)
		{
			campus_id = "01";
			building_id = "0101";
		}

		if (msg_view.find("三教") != msg_view.npos)
		{
			campus_id = "01";
			building_id = "0102";
		}

		if (msg_view.find("全天") != msg_view.npos)
		{
			free_time = Jw::FreeTime::AllDay;
		}

		if (msg_view.find("上午") != msg_view.npos)
		{
			free_time = Jw::FreeTime::AM;
		}

		if (msg_view.find("下午") != msg_view.npos)
		{
			free_time = Jw::FreeTime::PM;
		}

		if (msg_view.find("晚上") != msg_view.npos)
		{
			free_time = Jw::FreeTime::Night;
		}


		auto result = JwApi.GetFreeClassroom(date, free_time, campus_id, building_id);

		// 最多显示 100 个教室(20条消息)
		auto len = min(result.size(), size_t(100));

		if (result.empty())
		{
			m.Reply(MessageChain().Plain("没有找到 " + date + " " + FreeTimeText[(int)(free_time)] + "空闲的教室"));
		}
		else
		{
			m.Reply(MessageChain()
				.Plain(date + " " + FreeTimeText[(int)(free_time)] + "空闲的教室有" + to_string(len) + "个："));
		}

		MessageChain mc;
		for (size_t i = 0; i < len; i++)
		{
			mc
				.Plain(i + 1).Plain(". ")
				.Plain(result[i].BuildingName + " ")
				.Plain(result[i].ClassroomName + " (").Plain(result[i].Capacity).Plain(" 座)");
			if ((i + 1) % 5 == 0)
			{
				m.Reply(mc);
				mc.Clear();
				MiraiBot::SleepMilliseconds(200);
			}
			else
			{
				mc.Plain("\n");
			}
		}
		if (mc.Count() != 0)
		{
			m.Reply(mc);
		}

	}
	catch (const std::exception& ex)
	{
		LOG(ERROR) << "[" << m.Sender.ToInt64() << "] 使用 [查空教室] 指令时出现异常: " << ex.what();
		try
		{
			m.Reply(MessageChain().Plain("出现错误："s + ex.what()));
		}
		catch (const exception& ex)
		{
			LOG(ERROR) << "[" << m.Sender.ToInt64() << "] 使用 [查空教室] 指令时出现异常: " << ex.what();
		}
	}

}