#include <iostream>
#include <sstream>
#include <regex>
#include <fstream>
#include <filesystem>
#include <mirai.h>
#include <qzjw.h>
#include <nlohmann/json.hpp>

#include "main.h"
#include "database.h"

using namespace std;
using namespace Cyan;
using namespace cyanray;
using namespace nlohmann;

Jw JwApi = Jw("http://jwgl.cqjtu.edu.cn/");

UserDatabase UserDb;
NewsDatabase NewsDb;
SQLite::Database RankDb("rank.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
json AppConfig;

int main(int argc, char* argv[])
{
#if defined(_WIN32) || defined(WIN32)
	system("chcp 65001");
#endif

	google::InitGoogleLogging(argv[0]);
	FLAGS_logtostderr = 1;

	try
	{
		// 读取配置文件
		LOG(INFO) << "读取配置文件...";
		stringstream config_ss;
		if (!filesystem::exists("config.json"))
		{
			ofstream("config.json");
		}
		fstream config_file("config.json", ios::in | ios::out);
		if (!config_file)
		{
			LOG(ERROR) << "打开配置文件失败!";
			return -1;
		}
		config_ss << config_file.rdbuf();
		if (config_ss.str().empty())
		{
			AppConfig =
			{
				{"BotQQ", 100001},
				{"AdminQQ", 100002},
				{"AuthKey", "InitKeyA01B01"},
				{"MiraiApiHost", "localhost"},
				{"MiraiApiPort", 8080},
				{"JwUid", "631800000000"},
				{"JwPassword", "pwd123456"},
				{"FirstDayOfSemester", 1582473600ll},
				{"GoodMorningEnabled", true},
				{"GoodMorningTriggerTime", 5}
			};
			config_file.clear();
			config_file << AppConfig.dump(2, ' ');
			LOG(ERROR) << "没有找到配置文件，已生成默认配置文件，请修改后重新运行本程序!";
			config_file.close();
			return 1;
		}
		AppConfig = json::parse(config_ss.str());
		config_file.close();
	}
	catch (const exception& ex)
	{
		LOG(ERROR) << "读取配置文件时出现异常：" << ex.what();
		return -1;
	}

	for (auto it = AppConfig.cbegin(); it != AppConfig.cend(); ++it)
	{
		LOG(INFO) << "配置: " << it.key() << " : " << it.value();
	}

	try
	{
		LOG(INFO) << "登录教务网中...";
		JwApi.Login(AppConfig["JwUid"], AppConfig["JwPassword"]);
		LOG(INFO) << "成功登录教务网!";
	}
	catch (const exception& ex)
	{
		LOG(ERROR) << "登录教务网出错: " << ex.what();
	}

	MiraiBot bot(AppConfig["MiraiApiHost"], AppConfig["MiraiApiPort"]);
	while (true)
	{
		try
		{
			LOG(INFO) << "尝试连接 mirai...";
			bot.Auth(AppConfig["AuthKey"], QQ_t(AppConfig["BotQQ"]));
			LOG(INFO) << "连接 mirai 成功!";
			break;
		}
		catch (const std::exception& ex)
		{
			LOG(ERROR) << "连接 mirai 时出现错误: " << ex.what();
		}
		MiraiBot::SleepSeconds(1);
	}

	LOG(INFO) << "Bot Working...";

	// 监听一系列事件...

	bot.On<NewFriendRequestEvent>([&](NewFriendRequestEvent e)
		{
			try
			{
				LOG(INFO) << "新好友申请事件: " << e.Nick << "(" << e.FromId << "): “" << e.Message << "” " << endl;
				e.Accept();
				MiraiBot::SleepSeconds(10);
				bot.SendMessage(e.FromId, MessageChain().Plain(HELP_MSG));
			}
			catch (const exception& ex)
			{
				LOG(ERROR) << "处理新好友事件时出错: " << ex.what();
			}
		});

	bot.On<BotInvitedJoinGroupRequestEvent>([&](BotInvitedJoinGroupRequestEvent e)
		{
			try
			{
				LOG(INFO) << "邀请机器人入群事件: " 
						  << e.Nick << "(" << e.FromId << "), " 
						  << e.GroupName << "(" << e.GroupId << ")." << endl;
				e.Accept();
			}
			catch (const exception& ex)
			{
				LOG(ERROR) << "处理邀请机器人入群事件时出错: " << ex.what();
			}
		});

	bot.On<Message>([&](Message m)
		{
			if (m.MessageChain.GetPlainTextFirst() == "帮助")
			{
				m.Reply(MessageChain().Plain(HELP_MSG));
			}
		});

	bot.On<Message>([&](Message m)
		{
			if (m.MessageChain.GetPlainTextFirst() == "第几周")
			{
				int week = GetWeekOfSemester();
				m.Reply(MessageChain().Plain("本周是第 ").Plain(week).Plain(" 周"));
			}
		});

	bot.On<Message>([&](Message m)
		{
			if (m.MessageChain.GetPlainTextFirst() == "校车")
			{
				try
				{
					auto img = bot.UploadFriendImage("school_bus_timetable.png");
					m.Reply(MessageChain().Image(img));
				}
				catch (const exception& ex)
				{
					LOG(ERROR) << "发送校车时刻表时出错: " << ex.what();
				}
			}
		});

	bot.On<Message>(CmdSubscribeNews);

	bot.On<Message>(CmdSubscribeMorning);

	bot.On<Message>(CmdUnbinding);

	bot.On<Message>(CmdStatus);

	bot.On<Message>(CmdBinding);

	bot.On<Message>(CmdRefreshCourses);

	bot.On<Message>(CmdTodayCourses);

	bot.On<Message>(CmdTomorrowCourses);

	bot.On<Message>(CmdWeekCourses);

	bot.On<Message>(CmdNextWeekCourses);

	bot.On<Message>(CmdNews);

	bot.On<Message>(CmdLastWeekCourses);

	bot.On<Message>(CmdScore);

	bot.On<Message>(CmdExamResult);

	bot.On<Message>(CmdExamSchedule);

	bot.On<Message>(CmdRank);

	bot.On<Message>(CmdFreeClassroom);

	bot.On<GroupMessage>(CmdUpdateCourses);


	auto f1 = std::async(std::launch::async, [&]() { CronJobMorning(bot); });

	auto f2 = std::async(std::launch::async, [&]() { CronJobNews(bot); });

	auto f3 = std::async(std::launch::async, [&]() { CronJobLoginJw(bot); });

	bot.EventLoop([](const char* error_msg)
		{
			LOG(WARNING) << "接受 mirai 事件时出现错误: " << error_msg;
		});

	return 0;
}