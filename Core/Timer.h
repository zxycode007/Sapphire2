#pragma once 

#include "Object.h"


namespace Sapphire
{
	/// 低分辨率操作系统计时器
	class SAPPHIRE_API Timer
	{
	public:
		/// 构造函数，获取起始时钟值
		Timer();

		/// 获取逝去的毫秒数并且可以选择重置
		unsigned GetMSec(bool reset);
		/// 重置计时器
		void Reset();

	private:
		/// 起始的时钟毫秒数
		unsigned startTime_;
	};

	/// 用于性能分析的高分辨率操作系统计时器
	class SAPPHIRE_API HiresTimer
	{
		friend class Time;

	public:
		/// 获取起始的高分辨率的时钟值
		HiresTimer();

		/// 返回流逝的毫秒数并且可以重置它
		long long GetUSec(bool reset);
		/// 重置计时器
		void Reset();

		/// 返回是否支持高分辨率计时器
		static bool IsSupported() { return supported; }

		/// 如果支持，返回高分辨率计时器的频率
		static long long GetFrequency() { return frequency; }

	private:
		/// 起始的时钟值CPU的ticks
		long long startTime_;

		/// 是否支持高分辨率计时器的标志
		static bool supported;
		/// 高分辨率计时器的频率
		static long long frequency;
	};

	///时间和帧计数子系统
	class SAPPHIRE_API Time : public Object
	{
		SAPPHIRE_OBJECT(Time, Object);

	public:
		
		Time(Context* context);
		/// 析构，如果设置了低分辨率计时器重置它
		virtual ~Time();

		/// 用上一帧的持续时间秒数，开始新的一帧，并且发送帧开始事件
		void BeginFrame(float timeStep);
		/// 结束帧， 增加总时间并且发送帧结束事件
		void EndFrame();
		/// 设置按毫秒的低分辨率计时器。 0重设到默认时间
		void SetTimerPeriod(unsigned mSec);

		///获取帧数， 从BeginFram()开始调用后为1帧
		unsigned GetFrameNumber() const { return frameNumber_; }

		/// 返回当前帧的秒时间步进
		float GetTimeStep() const { return timeStep_; }

		/// 获取当前低分辨率计时器毫秒时间周期
		unsigned GetTimerPeriod() const { return timerPeriod_; }

		/// 返回从程序开始时逝去的时间
		float GetElapsedTime();

		/// 获取系统时间毫秒
		static unsigned GetSystemTime();
		/// 获取从1970/1/1开始的系统时间秒
		static unsigned GetTimeSinceEpoch();
		/// 获取日期/时间作为一个字符串
		static String GetTimeStamp();
		/// 休眠一定的毫秒数
		static void Sleep(unsigned mSec);

	private:
		/// 程序开始时逝去的时间
		Timer elapsedTime_;
		/// 帧数
		unsigned frameNumber_;
		/// 秒为单位的时间步进
		float timeStep_;
		/// 低分辨率计时器周期
		unsigned timerPeriod_;
	};
}