#pragma once
#ifndef QZJW_CPP_SDK_QZJW_H_H_
#define QZJW_CPP_SDK_QZJW_H_H_
#include <string>
#include <vector>
using std::string;
using std::vector;

namespace cyanray
{
	class Jw
	{
	public:
		// 课程信息
		struct Course
		{
			// 课程名称
			string Name;
			// 上课时间
			string StartTime;
			// 结束时间
			string EndTime;
			// 开课周次
			string Schedule;
			// 讲师姓名
			string Instructor;
			// 教室名称
			string Classroom;
			// 上课星期(1~7)
			int Week;
			// 从第N讲开始(1~6), TODO: 更换合适的名称
			int StartLesson;
			// 到第N讲结束(1~6), TODO: 更换合适的名称
			int EndLesson;
		};
		// 考试结果
		struct ExamResult
		{
			// 课程名称
			string Name;
			// 学期
			string Semester;
			// 课程类别(必修/选修)
			string CourseCategory;
			// 课程性质(通识教育课)
			string CourseNature;
			// 成绩
			string Score;
			// 学分
			double Credit;
		};
		// 考试安排
		struct ExamSchedule
		{
			// 课程名称
			string CourseName;
			// 考试教室
			string ExamRoom;
			// 中文考试时间
			string ChineseTime;
			// 准确考试时间
			string Time;
		};
		// 空闲教室
		struct FreeClassroom
		{
			string ClassroomName;
			int Capacity;
			string CampusName;
			string BuildingName;
			int Floor;  //楼层
		};

		enum class FreeTime
		{
			AllDay, AM, PM, Night
		};

		//天气
		struct Weather
		{
			//位置（双福 南岸）
			string Position;
			//日期
			string Date;
			//天气状况
			string Weather;
			//最小温度
			string MinTemperature;
			//最大温度
			string MaxTemperature;
		};

		//25小时的天气数据
		struct WeatherOneDay
		{
			//地点（科学城、南岸）
			string Position;
			//哪天（今天、明天）
			string Day;
			//小时
			string Hour;
			//温度
			string Temperature;
			//降水量
			string Precipitation;
		};

		/**
		 * \brief 初始化教务系统类
		 * \param api_prefix 教务系统地址前缀
		 */
		explicit Jw(const string& api_prefix) :api_prefix_(api_prefix)
		{
		}
		/**
		 * \brief 登录教务系统
		 * \param uid 学号
		 * \param pwd 密码
		 * \return 始终为 true，有错误会抛出异常
		 */
		bool Login(const string& uid, const string& pwd);
		/**
		 * \brief 获取课表
		 * \param uid 学号
		 * \param week 周次
		 * \param semester 学期(如：2019-2020-2)，留空为当前学期 
		 * \return vector<Course>，出错会抛出异常
		 */
		vector<Course> GetCourses(const string& uid, int week, const string& semester = "");
		/**
		 * @brief 获取考试结果
		 * @param uid 学号
		 * @param semester 学期(如：2019-2020-2)，留空为当前学期 
		 * @return vector<ExamResult>，出错会抛出异常
		*/
		vector<ExamResult> GetExamResult(const string& uid, const string& semester = "");
		/**
		 * @brief 获取考试安排
		 * @param uid 学号
		 * @return vector<ExamSchedule>，出错会抛出异常
		*/
		vector<ExamSchedule> GetExamSchedule(const string& uid);
		/**
		 * @brief 根据学号获取专业
		 * @param uid 学号
		 * @return 专业名称
		*/
		string GetMajor(const string& uid);
		/**
		 * @brief 获取登录后的Token
		 * @return Token值，为空说明未登录
		*/
		string GetToken() const
		{
			return token_;
		}

		vector<FreeClassroom> GetFreeClassroom(const string& date, FreeTime freeTime, const string& campus_id, const string& building_id);

		/**
		* @brief 根据教室名获取楼层
		* @param classroom 教室名
		* @return 楼层号
		*/
		int GetFloor(const string& classroom);

		/**
		* @brief 获取天气
		* @return vector<Weather>
		*/
		vector<Weather> GetWeatherOneWeek();

		/**
		* @brief 根据url获取pos位置的天气(7天)
		* @param url
		* @param pos 位置(如"SF"、"NA")
		* @return vector<Weather>
		*/
		vector<Weather> GetWeatherOneWeekByUrl(const string& url, const string& pos);

		/**
		* @brief 获取一天的天气
		* @return vector<WeaOneDay>
		*/
		vector<WeatherOneDay> GetWeatherOneDay();

		/**
		* @brief 根据获取当前时间开始25小时的天气
		* @param url
		* @param pos 位置(如"SF"、"NA")
		* @return vector<WeaOneDay>
		*/
		vector<WeatherOneDay> GetWeatherOneDayByUrl(const string& url, const string& pos);

	private:
		string api_prefix_;
		string token_;
	};
}

#endif // !QZJW_CPP_SDK_QZJW_H_H_
