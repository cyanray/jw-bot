#include <mirai.h>
#include <HTMLReader.h>
#include "main.h"
#include "../qzjw-cpp-sdk/CURLWrapper/include/CURLWrapper.h"
#include <glog/logging.h>
#include<regex>

void CronJobNews(Cyan::MiraiBot& bot)
{
	using namespace Cyan;
	while (true)
	{
		try
		{
			LOG(INFO) << "发起查询教务新闻的网络请求...";
			HTTP http;
			auto resp = http.Get("http://jw.cqjtu.edu.cn/jxxx/tzgg1.htm");
			if (resp.Ready && resp.StatusCode == 200)
			{
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
					string url = "http://jw.cqjtu.edu.cn" + tmp;
					string title = a["span"][0].GetInner();
					string date = a["span"][1].GetInner();
					LOG(INFO) << "查询到新闻：" << date << " 《" << title << "》";
					if (!NewsDb.Exist(title))
					{
						LOG(INFO) << "新闻 《" << title << "》不在数据库，广播给订阅用户...";
						NewsDb.Add(title, date, url);
						vector<QQ_t> users = UserDb.GetNewsSubscribers();
						for (auto user : users)
						{
							try
							{
								bot.SendMessage(user, MessageChain().Plain("有新的教务网新闻").Face(rand() % 30));
								MessageChain news;
								news.Plain("标题: " + title + "\n");
								news.Plain("网址: " + url + "\n");
								news.Plain("时间: " + date);
								bot.SendMessage(user, news);
								LOG(INFO) << "发送教务新闻给 [" << user << "] 成功!";
							}
							catch (const exception& ex)
							{
								LOG(ERROR) << "发送教务新闻给 [" << user << "] 失败: " << ex.what();
							}
							MiraiBot::SleepSeconds(10);
						}

					}

				}
			}

		}
		catch (const std::exception& ex)
		{
			LOG(ERROR) << "[教务新闻计划任务] 出现错误:" << ex.what();
		}
		std::this_thread::sleep_for(std::chrono::minutes(20));
	}
}