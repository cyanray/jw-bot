#include <mirai.h>
#include <HTMLReader.h>
#include "main.h"
#include "../qzjw-cpp-sdk/CURLWrapper/include/CURLWrapper.h"

void CronJobNews(Cyan::MiraiBot& bot)
{
	using namespace Cyan;
	while (true)
	{
		try
		{
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
				for (auto& t : tables)
				{
					HTMLDoc a = t["a"];
					string url = "http://jw.cqjtu.edu.cn" + a.GetAttribute("href");
					string title = a["span"][0].GetInner();
					string date = a["span"][1].GetInner();
					if (!NewsDb.Exist(title))
					{
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
							}
							catch (const exception& ex)
							{
								std::cout << ex.what() << std::endl;
							}
							MiraiBot::SleepSeconds(10);
						}

					}

				}
			}

		}
		catch (const std::exception& ex)
		{
			std::cout << ex.what() << std::endl;		// TODO: log
		}
		std::this_thread::sleep_for(std::chrono::minutes(20));
	}
}