#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <glog/logging.h>
#include <algorithm>
#include <fmt/core.h>
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
		if (!msg_view.starts_with("空教室")) return;

		LOG(INFO) << "[" << m.Sender << "] 使用 [查空教室] 指令: " << msg_str;

		string date = GetCurrentDate();
		string campus_id = "02";
		string building_id = "0201";
		vector<Jw::FreeTime> free_times;
		unordered_set<int> floors;

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

		//if (msg_view.find("全天") != msg_view.npos)
		//{
		//	free_time = Jw::FreeTime::AllDay;
		//}

		if (msg_view.find("上午") != msg_view.npos)
		{
			free_times.push_back(Jw::FreeTime::AM);
		}

		if (msg_view.find("下午") != msg_view.npos)
		{
			free_times.push_back(Jw::FreeTime::PM);
		}

		if (msg_view.find("晚上") != msg_view.npos)
		{
			free_times.push_back(Jw::FreeTime::Night);
		}

		if (msg_view.find("1楼") != msg_view.npos || msg_view.find("一楼") != msg_view.npos)
		{
			floors.insert(1);
		}
		if (msg_view.find("2楼") != msg_view.npos || msg_view.find("二楼") != msg_view.npos)
		{
			floors.insert(2);
		}
		if (msg_view.find("3楼") != msg_view.npos || msg_view.find("三楼") != msg_view.npos)
		{
			floors.insert(3);
		}
		if (msg_view.find("4楼") != msg_view.npos || msg_view.find("四楼") != msg_view.npos)
		{
			floors.insert(4);
		}
		if (msg_view.find("5楼") != msg_view.npos || msg_view.find("五楼") != msg_view.npos)
		{
			floors.insert(5);
		}
		if (msg_view.find("6楼") != msg_view.npos || msg_view.find("六楼") != msg_view.npos)
		{
			floors.insert(6);
		}
		if (msg_view.find("7楼") != msg_view.npos || msg_view.find("七楼") != msg_view.npos)
		{
			floors.insert(7);
		}
		if (msg_view.find("8楼") != msg_view.npos || msg_view.find("八楼") != msg_view.npos)
		{
			floors.insert(8);
		}
		if (msg_view.find("9楼") != msg_view.npos || msg_view.find("九楼") != msg_view.npos)
		{
			floors.insert(9);
		}

		// 全天时间段的教室是特殊的，所有时间条件它都满足
		free_times.push_back(Jw::FreeTime::AllDay);
		// 下面两个循环用于筛选同时满足其他时间段条件的教室 (顺便将楼层按照字典序排序)
		vector<Jw::FreeClassroom> result;
		map<string, pair<int, Jw::FreeClassroom>> container;
		for (auto&& free_time : free_times)
		{
			auto tmp = JwApi.GetFreeClassroom(date, free_time, campus_id, building_id);
			for (const auto& classroom : tmp)
			{
				if (container.contains(classroom.ClassroomName))
				{
					container[classroom.ClassroomName].first += 1;
				}
				else
				{
					container.insert(make_pair(classroom.ClassroomName, make_pair(1, classroom)));
				}
			}
		}
		for (auto&& t : container)
		{
			if (t.second.first >= free_times.size() - 1)
			{
				result.push_back(t.second.second);
			}
		}

		//选择了楼层选项，则进行楼层筛选
		if (floors.size() != 0)
		{
			std::erase_if(result, [&](const Jw::FreeClassroom& c)
				{
					return c.Floor == -1 || !floors.contains(c.Floor);
				});
		}

		// 最多显示 100 个教室(20条消息)
		auto len = min(result.size(), size_t(100));

		if (result.empty())
		{
			m.Reply(MessageChain().Plain("没有找到 " + date + " 空闲的教室"));
		}
		else
		{
			m.Reply(MessageChain().Plain(fmt::format("{} 空闲的教室有 {} 个: ", date, len)));
		}

		MessageChain mc;
		for (size_t i = 0; i < len; i++)
		{
			mc.Plain(fmt::format("{}. {} {} ({}座)", i + 1, result[i].BuildingName, result[i].ClassroomName, result[i].Capacity));
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
		LOG(ERROR) << "[" << m.Sender << "] 使用 [查空教室] 指令时出现异常: " << ex.what();
		try
		{
			m.Reply(MessageChain().Plain("出现错误："s + ex.what()));
		}
		catch (const exception& ex)
		{
			LOG(ERROR) << "[" << m.Sender << "] 使用 [查空教室] 指令时出现异常: " << ex.what();
		}
	}

}