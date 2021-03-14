#include <iostream>
#include <sstream>
#include <glog/logging.h>
#include "main.h"
using namespace std;
using namespace Cyan;

void CmdNews(Message m)
{
	if (m.MessageChain.GetPlainTextFirst() != "教务新闻") return;

	LOG(INFO) << "[" << m.Sender.ToInt64() << "] 使用 [教务新闻] 指令";

	try
	{
		for (const auto& n : NewsDb.GetLatestNews())
		{
			MessageChain news;
			news.Plain("标题: " + n.title + "\n");
			news.Plain("网址: " + n.url + "\n");
			news.Plain("时间: " + n.time);
			m.Reply(news);
			MiraiBot::SleepMilliseconds(200);
		}
	}
	catch (const std::exception& ex)
	{
		LOG(ERROR) << "[" << m.Sender.ToInt64() << "] 使用 [教务新闻] 指令时出现异常: " << ex.what();
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