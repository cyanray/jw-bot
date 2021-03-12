#include <iostream>
#include <sstream>
#include "main.h"
using namespace std;
using namespace Cyan;

void CmdNews(Message m)
{
	if (m.MessageChain.GetPlainTextFirst() != "教务新闻") return;
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
		try
		{
			m.Reply(MessageChain().Plain("出现错误："s + ex.what()));
		}
		catch (...)
		{
		}
	}
}