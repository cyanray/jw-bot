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

Jw JwApi("");

UserDatabase UserDb;
NewsDatabase NewsDb;
SQLite::Database RankDb("rank.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
json AppConfig;

int main()
{
#if defined(_WIN32) || defined(WIN32)
	system("chcp 65001");
#endif

	try
	{
		// 读取配置文件
		stringstream config_ss;
		if (!filesystem::exists("config.json"))
		{
			ofstream("config.json");
		}
		fstream config_file("config.json", ios::in | ios::out);
		if (!config_file)
		{
			cout << "打开配置文件失败" << endl;
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
				{"QzJwApiPrefix", "http://jwgl.xxxx.edu.cn/"},
				{"QzJwAdminUid", "631800000000"},
				{"QzJwAdminPwd", "pwd123456"},
				{"first_day_this_semester", 1582473600ll},
				{"GoodMorning_Enabled", true}
			};
			config_file.clear();
			config_file << AppConfig.dump();
			cout << "没有找到配置文件，已生成默认配置文件，请修改。" << endl;
			config_file.close();
			return 1;
		}
		AppConfig = json::parse(config_ss.str());
		config_file.close();
	}
	catch (const exception& ex)
	{
		cout << "读取配置文件时出现异常：" << ex.what() << endl;
		return -1;
	}

	for (auto it = AppConfig.cbegin(); it != AppConfig.cend(); ++it)
	{
		cout << it.key() << " : " << it.value() << endl;
	}
	cout << endl;

	JwApi = Jw(AppConfig["QzJwApiPrefix"]);
	try
	{
		JwApi.Login(AppConfig["QzJwAdminUid"], AppConfig["QzJwAdminPwd"]);
	}
	catch (const exception& ex)
	{
		cout << "登录教务网出错: " << ex.what() << endl;
	}

	MiraiBot bot(AppConfig["MiraiApiHost"], AppConfig["MiraiApiPort"]);

	while (true)
	{
		try
		{
			bot.Auth(AppConfig["AuthKey"], QQ_t(AppConfig["BotQQ"]));
			break;
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}
		MiraiBot::SleepSeconds(1);
	}
	cout << "Bot Working..." << endl;

	// 监听一系列事件...

	bot.On<NewFriendRequestEvent>([&](NewFriendRequestEvent e)
		{
			try
			{
				e.Accept();
				MiraiBot::SleepSeconds(10);
				bot.SendMessage(e.FromId, MessageChain().Plain(HELP_MSG));
			}
			catch (const exception& ex)
			{
				cout << ex.what() << endl;	// TODO: log
			}
		});

	bot.On<BotInvitedJoinGroupRequestEvent>([&](BotInvitedJoinGroupRequestEvent e)
		{
			try
			{
				e.Accept();
			}
			catch (const exception& ex)
			{
				cout << ex.what() << endl;	// TODO: log
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

	bot.On<Message>(CmdSubscribeNews);

	bot.On<Message>(CmdSubscribeMorning);

	bot.On<Message>(CmdUnbinding);

	bot.On<Message>(CmdStatus);

	bot.On<Message>(CmdBinding);

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

	bot.EventLoop();

	return 0;
}