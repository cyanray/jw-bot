#include <iostream>
#include <mirai.h>
#include "main.h"

void CronJobMorning(Cyan::MiraiBot& bot)
{
	using namespace Cyan;
	const static vector<string> good_morning_words =
	{ "早安~", "起床学习啦!", "起床！起床！", "你尚未起床，赖床指数666，打败了99%的人!" };

	bool morning_enable = AppConfig["GoodMorningEnabled"].get<bool>();
	int trigger_time = AppConfig["GoodMorningTriggerTime"].get<int>();
	while (morning_enable)
	{
		int hour = GetHour24();

		// 如果到了触发时间 就给发早安，然后延迟61分钟
		// 如果没到触发时间 就延迟1分钟再次检测
		int next_sleep_time = 1;
		if (hour == trigger_time)
		{
			LOG(INFO) << "准备发送每日课表...";

			int week_of_semester = GetWeekOfSemester();			// 获取本学期第几周
			int week = GetWeekToday();							// 获取今天是星期几
			vector<QQ_t> users = UserDb.GetMorningSubscribers();
			LOG(INFO) << "需要给 " << users.size() << "位同学发送课表";
			for (auto user : users)
			{
				try
				{
					bot.SendMessage(user, MessageChain().Plain(good_morning_words[rand() % good_morning_words.size()]));

					auto today_courses = UserDb.GetCourses(user, week_of_semester, week);

					MessageChain mc;
					int count = today_courses.size();
					if (count > 0)
					{
						mc.Plain("你今天共有 ").Plain(count).Plain(" 节课\n");
						mc = mc + CoursesFormat(today_courses);
					}
					else
					{
						mc.Plain("你今天没有课了！好好休息吧！");
					}

					bot.SendMessage(user, mc);

					LOG(INFO) << "给 [" << user.ToInt64() << "] 同学发送早安课表成功!";

					for (const auto& ele : today_courses)
					{
						string name = ele.Name;
						string time = ele.Time;
						if (name.find("体育") != string::npos)
						{
							// TODO: 更改为更好的判断条件
							if (time == "08:20-10:00" || time == "10:20-12:00")
							{
								bot.SendMessage(user, MessageChain().Plain("今天早上有体育课，记得要吃早餐！"));
								break;
							}
						}
					}
				}
				catch (const std::exception& ex)
				{
					LOG(INFO) << "给 [" << user.ToInt64() << "] 同学发送早安课表时出现异常: " << ex.what();
				}
				std::this_thread::sleep_for(std::chrono::seconds(3));
			}
			next_sleep_time = 61;
		}
		std::this_thread::sleep_for(std::chrono::minutes(next_sleep_time));
	}
}
