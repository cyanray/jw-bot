#include <iostream>
#include <sstream>
#include <glog/logging.h>
#include <fmt/core.h>
#include "main.h"
using namespace std;
using namespace Cyan;

void CmdNews(Message m)
{
	if (m.MessageChain.GetPlainTextFirst() != "教务新闻") return;

	LOG(INFO) << "[" << m.Sender << "] 使用 [教务新闻] 指令";

	try
	{
		for (const auto& n : NewsDb.GetLatestNews())
		{
			MessageChain news;
			news.Plain(fmt::format("标题: {}\n网址: {}\n时间: {}", n.title, n.url, n.time));
			m.Reply(news);
			MiraiBot::SleepMilliseconds(200);
		}
	}
	catch (const std::exception& ex)
	{
		LOG(ERROR) << "[" << m.Sender << "] 使用 [教务新闻] 指令时出现异常: " << ex.what();
		try
		{
			m.Reply(MessageChain().Plain("出现错误："s + ex.what()));
		}
		catch (const exception& ex)
		{
			LOG(ERROR) << "[" << m.Sender.ToInt64() << "] 使用 [教务新闻] 指令时出现异常: " << ex.what();
		}
	}
}