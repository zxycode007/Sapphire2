#pragma once


#include "LinkedList.h"
#include "Variant.h"

namespace Sapphire
{
	class Context;
	class EventHandler;

	/// 对象运行时类型信息
	class SAPPHIRE_API TypeInfo
	{
	public:
		
		TypeInfo(const char* typeName, const TypeInfo* baseTypeInfo);
	
		~TypeInfo();

		// 检查当前类型是不是指定的类型
		bool IsTypeOf(StringHash type) const;
		/// 检查当前类型是不是指定类型
		bool IsTypeOf(const TypeInfo* typeInfo) const;
		/// 检查当前类型是不是指定的类类型
		template<typename T> bool IsTypeOf() const { return IsTypeOf(T::GetTypeInfoStatic()); }

		/// 返回类型hash
		StringHash GetType() const { return type_; }
		/// 返回类型名
		const String& GetTypeName() const { return typeName_; }
		/// 返回基类型信息
		const TypeInfo* GetBaseTypeInfo() const { return baseTypeInfo_; }

	private:
		/// 类型hash值
		StringHash type_;
		/// 类型名
		String typeName_;
		/// 基类类型信息
		const TypeInfo* baseTypeInfo_;
	};

	//将原类型名和基类类型名定义为ClassName和BaseClassName便于Object内部统一调用
#define SAPPHIRE_OBJECT(typeName, baseTypeName) \
    public: \
        typedef typeName ClassName; \
        typedef baseTypeName BaseClassName; \
        virtual Sapphire::StringHash GetType() const { return GetTypeInfoStatic()->GetType(); } \
        virtual const Sapphire::String& GetTypeName() const { return GetTypeInfoStatic()->GetTypeName(); } \
        virtual const Sapphire::TypeInfo* GetTypeInfo() const { return GetTypeInfoStatic(); } \
        static Sapphire::StringHash GetTypeStatic() { return GetTypeInfoStatic()->GetType(); } \
        static const Sapphire::String& GetTypeNameStatic() { return GetTypeInfoStatic()->GetTypeName(); } \
        static const Sapphire::TypeInfo* GetTypeInfoStatic() { static const Sapphire::TypeInfo typeInfoStatic(#typeName, BaseClassName::GetTypeInfoStatic()); return &typeInfoStatic; } \


	//  任意对象的基类，提供类型定义，子系统访问和事件发送/接收功能
	class SAPPHIRE_API Object : public RefCounted
	{
		friend class Context;   //友元，便于对Context私有成员进行访问

	public:
		/// 用环境来构造对象
		Object(Context* context);
		/// 销毁时清理自身的sender和receiver事件结构
		virtual ~Object();

		 
		virtual StringHash GetType() const = 0;
		 
		virtual const String& GetTypeName() const = 0;
		 
		virtual const TypeInfo* GetTypeInfo() const = 0;
		
		/// 事件响应操作
		virtual void OnEvent(Object* sender, StringHash eventType, VariantMap& eventData);

		
		//  返回类型信息静态
		static const TypeInfo* GetTypeInfoStatic() { return 0; }
		/// 检查当前类型是否是指定类型
		static bool IsTypeOf(StringHash type);
		///检查当前类型是否是指定类型
		static bool IsTypeOf(const TypeInfo* typeInfo);
		///检查当前类型是否是指定类
		template<typename T> static bool IsTypeOf() { return IsTypeOf(T::GetTypeInfoStatic()); }
		/// 检查当前实例是否是指定类型
		bool IsInstanceOf(StringHash type) const;
		/// 检查当前实例是否是指定类型
		bool IsInstanceOf(const TypeInfo* typeInfo) const;
		/// 检查当前实例是否是指定类
		template<typename T> bool IsInstanceOf() const { return IsInstanceOf(T::GetTypeInfoStatic()); }

		
		// 订阅可以由任何发送者发送的一个事件
		void SubscribeToEvent(StringHash eventType, EventHandler* handler);
		///  订阅可以由特定发送者发送的一个事件
		void SubscribeToEvent(Object* sender, StringHash eventType, EventHandler* handler);
		/// 取消订阅事件
		void UnsubscribeFromEvent(StringHash eventType);
		/// 取消特定发送者的特定事件类型的事件
		void UnsubscribeFromEvent(Object* sender, StringHash eventType);
		/// 取消特定发送者的所有订阅的事件
		void UnsubscribeFromEvents(Object* sender);
		/// 取消订阅的所有事件
		void UnsubscribeFromAllEvents();
		/// 取消订阅除了列表内的所有事件，和只有用户数据的
		void UnsubscribeFromAllEventsExcept(const PODVector<StringHash>& exceptions, bool onlyUserData);
		/// 给所有订阅者发送事件
		void SendEvent(StringHash eventType);
		/// 用指定的参数给所有的订阅者发送事件
		void SendEvent(StringHash eventType, VariantMap& eventData);
		/// 返回一个预分配的事件数据映射。 
		VariantMap& GetEventDataMap() const;

		/// 返回执行环境Context
		Context* GetContext() const { return context_; }

		 
		// 通过类型返回子系统
		Object* GetSubsystem(StringHash type) const;
		/// 返回活动的事件发送者,未事件处理的为NULL
		Object* GetEventSender() const;
		/// 返回活动的事件发送者,未事件处理的为NULL
		EventHandler* GetEventHandler() const;
		//  返回是否订阅了一个指定类型的事件
		bool HasSubscribedToEvent(StringHash eventType) const;
		/// 返回是否订阅了一个指定发送者和指定类型的事件
		bool HasSubscribedToEvent(Object* sender, StringHash eventType) const;

		/// 返回是否订阅了一个任意的事件
		bool HasEventHandlers() const { return !eventHandlers_.Empty(); }

		// 返回一个子系统的模板版本
		template <class T> T* GetSubsystem() const;
		// 返回对象目录。 目录通过对象工厂注册。如果对象目录没有注册返回空
		const String& GetCategory() const;

	protected:
		/// 执行环境
		Context* context_;

	private:
		// 查找没有特定发送者的第一个事件处理器
		EventHandler* FindEventHandler(StringHash eventType, EventHandler** previous = 0) const;
		///查找有特定发送者的第一个事件处理器
		EventHandler* FindSpecificEventHandler(Object* sender, EventHandler** previous = 0) const;
		/// 查找有特定发送者和特定类型的第一个事件处理器
		EventHandler* FindSpecificEventHandler(Object* sender, StringHash eventType, EventHandler** previous = 0) const;

		// 移除一个特定发送者相关的事件处理器
		void RemoveEventSender(Object* sender);

		 
		//事件处理器列表,  对于非特定的处理器sender为null
		LinkedList<EventHandler> eventHandlers_;
	};

	template <class T> T* Object::GetSubsystem() const { return static_cast<T*>(GetSubsystem(T::GetTypeStatic())); }

	 
	// 基对象工厂
	class SAPPHIRE_API ObjectFactory : public RefCounted
	{
	public:
		/// Construct.
		ObjectFactory(Context* context) :
			context_(context)
		{
			assert(context_);
		}

		/// 创建一个对象，在模板子类中实现
		virtual SharedPtr<Object> CreateObject() = 0;

		/// 返回执行环境
		Context* GetContext() const { return context_; }

		/// 通过对象工厂返回创建的对象的类型信息
		const TypeInfo* GetTypeInfo() const { return typeInfo_; }

		/// 通过对象工厂返回创建对象的类型hash
		StringHash GetType() const { return typeInfo_->GetType(); }

		/// 通过对象工厂返回创建对象的类型名
		const String& GetTypeName() const { return typeInfo_->GetTypeName(); }

	protected:
		/// 执行环境
		Context* context_;
		/// 类型信息
		const TypeInfo* typeInfo_;
	};

	// 对象工厂的模板实现
	template <class T> class ObjectFactoryImpl : public ObjectFactory
	{
	public:
		 
		ObjectFactoryImpl(Context* context) :
			ObjectFactory(context)
		{
			typeInfo_ = T::GetTypeInfoStatic();  //获取对应类型的类类型信息
		}

		// 创建一个特定类型的对象
		virtual SharedPtr<Object> CreateObject() { return SharedPtr<Object>(new T(context_)); }
	};

	// 对于执行事件处理器函数的内部辅助类
	class SAPPHIRE_API EventHandler : public LinkedListNode
	{
	public:
		// 用指定的receiver来构造
		EventHandler(Object* receiver) :
			receiver_(receiver),
			sender_(0),
			userData_(0)
		{
			assert(receiver_);
		}

		///  用指定的receiver和userData来构造
		EventHandler(Object* receiver, void* userData) :
			receiver_(receiver),
			sender_(0),
			userData_(userData)
		{
			assert(receiver_);
		}

		 
		virtual ~EventHandler() { }

		 
		void SetSenderAndEventType(Object* sender, StringHash eventType)
		{
			sender_ = sender;
			eventType_ = eventType;
		}

		///执行事件处理函数
		virtual void Invoke(VariantMap& eventData) = 0;

		// 返回一个事件处理器的唯一拷贝
		virtual EventHandler* Clone() const = 0;

		/// 返回receiver
		Object* GetReceiver() const { return receiver_; }

		 
		Object* GetSender() const { return sender_; }

	 
		const StringHash& GetEventType() const { return eventType_; }

	 
		void* GetUserData() const { return userData_; }

	protected:
		 
		Object* receiver_;
	 
		Object* sender_;
		 
		StringHash eventType_;
		 
		void* userData_;
	};

	
	// 事件处理器辅助执行的模板实现类(保存一个指定类的函数指针)
	template <class T> class EventHandlerImpl : public EventHandler
	{
	public:
		typedef void (T::*HandlerFunctionPtr)(StringHash, VariantMap&);

		
		//用receiver和函数指针构造
		EventHandlerImpl(T* receiver, HandlerFunctionPtr function) :
			EventHandler(receiver),
			function_(function)
		{
			assert(function_);
		}

			// 返回一个事件处理器的唯一拷贝
		EventHandlerImpl(T* receiver, HandlerFunctionPtr function, void* userData) :
			EventHandler(receiver, userData),
			function_(function)
		{
			assert(function_);
		}

		
		// 通过函数指针执行事件处理函数
		virtual void Invoke(VariantMap& eventData)
		{
			T* receiver = static_cast<T*>(receiver_);
			(receiver->*function_)(eventType_, eventData);
		}

		// 返回一个事件处理器的唯一拷贝
		virtual EventHandler* Clone() const
		{
			return new EventHandlerImpl(static_cast<T*>(receiver_), function_, userData_);
		}

	private:
		// 指向处理函数的函数指针
		HandlerFunctionPtr function_;
	};

	/// 描述一个事件的hash ID  和 开始一个定义它的参数的命名空间
#define SAPPHIRE_EVENT(eventID, eventName) static const Sapphire::StringHash eventID(#eventName); namespace eventName
	/// 描述一个世界的参数hash ID，应该被用在一个事件的命名空间里。
#define SAPPHIRE_PARAM(paramID, paramName) static const Sapphire::StringHash paramID(#paramName)
	/// 用于构造指向一个receiver对象和他的成员函数的事件处理器的宏
#define SAPPHIRE_HANDLER(className, function) (new Sapphire::EventHandlerImpl<className>(this, &className::function))
	/// 用于构造指向一个receiver对象和他的成员函数的事件处理器的宏，并且定义了用户数据
#define SAPPHIRE_HANDLER_USERDATA(className, function, userData) (new Sapphire::EventHandlerImpl<className>(this, &className::function, userData))

}