#include <mirai.h>
#include <HTMLReader.h>
#include "main.h"
#include "../qzjw-cpp-sdk/CURLWrapper/include/CURLWrapper.h"
#include <glog/logging.h>
#include<regex>
#ifdef SendMessage
#undef SendMessage
#endif

void CronJobNews(Cyan::MiraiBot& bot)
{
	using namespace Cyan;
	while (true)
	{
		try
		{
			LOG(INFO) << "发起查询教务新闻的网络请求...";
			vector<News> newsList;
			HTTP http;
			auto resp = http.Get("http://jw.cqjtu.edu.cn/jxxx/tzgg1.htm");
			if (!resp.Ready || resp.StatusCode != 200) continue;

			HTMLDoc html;
			html.Parse(resp.Content);
			// /html/body/div[3]/div/div/div[2]/div/div[2]/div/div/div/div[1]/ul
			auto list =
				html["html"]["body"]
				["div"][2]
				["div"]["div"]["div"][1]
				["div"]["div"][1]
				["div"]["div"]["div"]["div"][0]["ul"];
			auto tables = list.SearchByTagName("dt");
			regex pattern(R"((?:\.\.)?(.+))", regex::ECMAScript);
			for (auto& t : tables)
			{
				HTMLDoc a = t["a"];
				string tmp = a.GetAttribute("href");
				std::smatch matches;
				if (regex_search(tmp, matches, pattern))
				{
					tmp = matches.str(1);
				}
				News news;
				news.url = "http://jw.cqjtu.edu.cn" + tmp;
				news.title = a["span"][0].GetInner();
				news.time = a["span"][1].GetInner();
				LOG(INFO) << "查询到新闻：" << news.time << " 《" << news.title << "》";
				if (!NewsDb.Exist(news.title))
				{
					newsList.emplace_back(std::move(news));
				}
			}
			vector<QQ_t> users = UserDb.GetNewsSubscribers();
			for (auto user : users)
			{
				bot.SendMessage(user, MessageChain().Plain("有新的教务网新闻").Face(rand() % 30));
				for (auto&& news : newsList)
				{
					try
					{
						MessageChain newsTemplate;
						newsTemplate.Plain("标题: " + news.title + "\n");
						newsTemplate.Plain("网址: " + news.url + "\n");
						newsTemplate.Plain("时间: " + news.time);
						bot.SendMessage(user, newsTemplate);
						LOG(INFO) << "发送教务新闻给 [" << user << "] 成功!";
					}
					catch (const exception& ex)
					{
						LOG(ERROR) << "发送教务新闻给 [" << user << "] 失败: " << ex.what();
					}
					MiraiBot::SleepSeconds(2);
				}
				MiraiBot::SleepSeconds(10);
			}
			// 全部发送完成再加入数据库
			for (auto&& news : newsList)
			{
				NewsDb.Add(news.title, news.time, news.url);
			}

		}
		catch (const std::exception& ex)
		{
			LOG(ERROR) << "[教务新闻计划任务] 出现错误:" << ex.what();
		}
		std::this_thread::sleep_for(std::chrono::minutes(20));
	}
}