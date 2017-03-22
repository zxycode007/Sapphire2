#pragma once

#include "Attribute.h"
#include "Object.h"
#include "HashSet.h"


namespace Sapphire
{

	
	//执行上下文 。提供子系统访问，对象工厂和属性，事件接收器
	class SAPPHIRE_API Context : public RefCounted
	{
		friend class Object;

	public:
		 
		Context();
		 
		~Context();

		
		//通过类型hash来创建一个对象，如果没找到工厂返回null
		SharedPtr<Object> CreateObject(StringHash objectType);
		//为一个对象类型注册一个工厂
		void RegisterFactory(ObjectFactory* factory);
		// 注册一个有特点目录的对象工厂
		void RegisterFactory(ObjectFactory* factory, const char* category);
		/// 注册一个子系统
		void RegisterSubsystem(Object* subsystem);
		/// 移除子系统
		void RemoveSubsystem(StringHash objectType);
		/// 注册对象属性
		// 不支持空或指针类型
		void RegisterAttribute(StringHash objectType, const AttributeInfo& attr);
		/// 移除对象的属性
		void RemoveAttribute(StringHash objectType, const char* name);
		/// 用默认值更新对象属性
		void UpdateAttributeDefaultValue(StringHash objectType, const char* name, const Variant& defaultValue);
		/// 返回预分配的事件数据映射。用于优化事件数据映射，避免经常重分配
		VariantMap& GetEventDataMap();

		
		// 复制基类属性到派生类
		void CopyBaseAttributes(StringHash baseType, StringHash derivedType);
		// 注册一个对象工厂的模板版本
		template <class T> void RegisterFactory();
		///注册一个有特点目录的对象工厂的模板版本
		template <class T> void RegisterFactory(const char* category);
		/// 移除子系统的模板版本 
		template <class T> void RemoveSubsystem();
	    // 注册对象类型T注册属性
		template <class T> void RegisterAttribute(const AttributeInfo& attr);
	    // 移除对象类型T的属性
		template <class T> void RemoveAttribute(const char* name);
		//复制基属性
		template <class T, class U> void CopyBaseAttributes();
		//更新默认属性
		template <class T> void UpdateAttributeDefaultValue(const char* name, const Variant& defaultValue);

		
		// 返回子系统类型
		Object* GetSubsystem(StringHash type) const;

		/// 返回所有的子系统
		const HashMap<StringHash, SharedPtr<Object> >& GetSubsystems() const { return subsystems_; }

		/// 返回所有的对象工厂
		const HashMap<StringHash, SharedPtr<ObjectFactory> >& GetObjectFactories() const { return factories_; }

		/// 返回所有的对象目录
		const HashMap<String, Vector<StringHash> >& GetObjectCategories() const { return objectCategories_; }

		/// 返回活动的事件发送者，在事件处理外为NULL
		Object* GetEventSender() const;

		/// 返回活动的事件处理器。由对象设置。 在事件处理外为NULL
		EventHandler* GetEventHandler() const { return eventHandler_; }


		// 由hash值来返回对象类型名，如果unknown为空
		const String& GetTypeName(StringHash objectType) const;
		//  返回一个对象的一个特定的属性,如果没找到为NULL
		AttributeInfo* GetAttribute(StringHash objectType, const char* name);
		/// Template version of returning a subsystem.
		// 取得子系统的模板版本
		template <class T> T* GetSubsystem() const;		 
		// 获取一个指定属性的模板版本
		template <class T> AttributeInfo* GetAttribute(const char* name);

		// 返回一个对象类型的属性描述，如果没有定义返回空
		const Vector<AttributeInfo>* GetAttributes(StringHash type) const
		{
			HashMap<StringHash, Vector<AttributeInfo> >::ConstIterator i = attributes_.Find(type);
			return i != attributes_.End() ? &i->second_ : 0;
		}

		// 返回一个对象类型的网络复制的属性，如果没有定义返回空
		const Vector<AttributeInfo>* GetNetworkAttributes(StringHash type) const
		{
			HashMap<StringHash, Vector<AttributeInfo> >::ConstIterator i = networkAttributes_.Find(type);
			return i != networkAttributes_.End() ? &i->second_ : 0;
		}

		//  返回所有的注册的属性
		const HashMap<StringHash, Vector<AttributeInfo> >& GetAllAttributes() const { return attributes_; }

		// 返回一个发送者和事件类型的对应事件接收者，如果不存在返回空
		HashSet<Object*>* GetEventReceivers(Object* sender, StringHash eventType)
		{
			HashMap<Object*, HashMap<StringHash, HashSet<Object*> > >::Iterator i = specificEventReceivers_.Find(sender);
			if (i != specificEventReceivers_.End())
			{
				HashMap<StringHash, HashSet<Object*> >::Iterator j = i->second_.Find(eventType);
				return j != i->second_.End() ? &j->second_ : 0;
			}
			else
				return 0;
		}

		// 返回一个事件类型的receiver，如果不存在返回null
		HashSet<Object*>* GetEventReceivers(StringHash eventType)
		{
			HashMap<StringHash, HashSet<Object*> >::Iterator i = eventReceivers_.Find(eventType);
			return i != eventReceivers_.End() ? &i->second_ : 0;
		}

	private:
	 
		// 添加一个事件接收者
		void AddEventReceiver(Object* receiver, StringHash eventType);
		/// 添加一个指定事件类型的Receiver
		void AddEventReceiver(Object* receiver, Object* sender, StringHash eventType);
		// 移除一个事件发送者的所有receiver。 调用它的析构
		void RemoveEventSender(Object* sender);
		// 移除一个指定事件的事件接收者
		void RemoveEventReceiver(Object* receiver, Object* sender, StringHash eventType);
		// 从非特定事件移除事件接收者
		void RemoveEventReceiver(Object* receiver, StringHash eventType);

		// 设置当前事件处理器。 通过对象调用
		void SetEventHandler(EventHandler* handler) { eventHandler_ = handler; }

		/// 开始发送事件
		void BeginSendEvent(Object* sender) { eventSenders_.Push(sender); }

		/// 结束事件的发送。在此同时清理事件接收者
		void EndSendEvent() { eventSenders_.Pop(); }

		/// 对象工厂
		HashMap<StringHash, SharedPtr<ObjectFactory> > factories_;
		/// 子系统.
		HashMap<StringHash, SharedPtr<Object> > subsystems_;
		/// 每个对象类型的绑定的属性描述
		HashMap<StringHash, Vector<AttributeInfo> > attributes_;
		/// 每个对象类型的网络复制属性描述
		HashMap<StringHash, Vector<AttributeInfo> > networkAttributes_;
		/// 非特定事件的事件接收者
		HashMap<StringHash, HashSet<Object*> > eventReceivers_;
		/// 对于指定了发送者的事件接收者
		HashMap<Object*, HashMap<StringHash, HashSet<Object*> > > specificEventReceivers_;
		/// 事件发送者栈
		PODVector<Object*> eventSenders_;
		/// 事件数据栈
		PODVector<VariantMap*> eventDataMaps_;
		/// 活动的事件句柄。不保存到栈内主要是性能原因
		EventHandler* eventHandler_;
		///对象目录
		HashMap<String, Vector<StringHash> > objectCategories_;
	};

	//会自动创建一个类型T的对象工厂，并添加到对象工厂列表中
	template <class T> void Context::RegisterFactory() { RegisterFactory(new ObjectFactoryImpl<T>(this)); }
	//指定目录注册对象工厂
	template <class T> void Context::RegisterFactory(const char* category)
	{
		RegisterFactory(new ObjectFactoryImpl<T>(this), category);
	}

	template <class T> void Context::RemoveSubsystem() { RemoveSubsystem(T::GetTypeStatic()); }

	template <class T> void Context::RegisterAttribute(const AttributeInfo& attr) { RegisterAttribute(T::GetTypeStatic(), attr); }

	template <class T> void Context::RemoveAttribute(const char* name) { RemoveAttribute(T::GetTypeStatic(), name); }
	//将属于类型T的属性复制给类型U
	template <class T, class U> void Context::CopyBaseAttributes() { CopyBaseAttributes(T::GetTypeStatic(), U::GetTypeStatic()); }

	template <class T> T* Context::GetSubsystem() const { return static_cast<T*>(GetSubsystem(T::GetTypeStatic())); }
	//获取属性名
	template <class T> AttributeInfo* Context::GetAttribute(const char* name) { return GetAttribute(T::GetTypeStatic(), name); }

	template <class T> void Context::UpdateAttributeDefaultValue(const char* name, const Variant& defaultValue)
	{
		UpdateAttributeDefaultValue(T::GetTypeStatic(), name, defaultValue);
	}

}
