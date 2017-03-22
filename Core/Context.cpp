#include "Predefined.h"

#include "Context.h"
#include "Thread.h"
#include "Log.h"

#include "DebugNew.h"


namespace Sapphire
{

	void RemoveNamedAttribute(HashMap<StringHash, Vector<AttributeInfo> >& attributes, StringHash objectType, const char* name)
	{
		HashMap<StringHash, Vector<AttributeInfo> >::Iterator i = attributes.Find(objectType);
		if (i == attributes.End())
			return;

		Vector<AttributeInfo>& infos = i->second_;

		for (Vector<AttributeInfo>::Iterator j = infos.Begin(); j != infos.End(); ++j)
		{
			if (!j->name_.Compare(name, true))
			{
				infos.Erase(j);
				break;
			}
		}

		// If the vector became empty, erase the object type from the map
		if (infos.Empty())
			attributes.Erase(i);
	}

	Context::Context() :
		eventHandler_(0)
	{
#ifdef ANDROID
		// Always reset the random seed on Android, as the Urho3D library might not be unloaded between runs
		SetRandomSeed(1);
#endif

		// Set the main thread ID (assuming the Context is created in it)
		Thread::SetMainThread();
	}

	Context::~Context()
	{
		 
		//以构造相反的顺序移除子系统
		/// \Context 不需要了解子系统相关的信息
		RemoveSubsystem("Audio");
		RemoveSubsystem("UI");
		RemoveSubsystem("Input");
		RemoveSubsystem("Renderer");
		RemoveSubsystem("Graphics");

		subsystems_.Clear();
		factories_.Clear();

		//删除所有的事件数据map
		for (PODVector<VariantMap*>::Iterator i = eventDataMaps_.Begin(); i != eventDataMaps_.End(); ++i)
			delete *i;
		eventDataMaps_.Clear();
	}

	SharedPtr<Object> Context::CreateObject(StringHash objectType)
	{
		//通过对象类型的StringHash来找到对应对象类型的构造工厂
		HashMap<StringHash, SharedPtr<ObjectFactory> >::ConstIterator i = factories_.Find(objectType);
		if (i != factories_.End())
			return i->second_->CreateObject();
		else
			return SharedPtr<Object>();
	}

	void Context::RegisterFactory(ObjectFactory* factory)
	{
		if (!factory)
			return;

		factories_[factory->GetType()] = factory;
	}

	void Context::RegisterFactory(ObjectFactory* factory, const char* category)
	{
		if (!factory)
			return;
		//注册工厂
		RegisterFactory(factory);
		//对象目录,放入工厂类型
		if (String::CStringLength(category))
			objectCategories_[category].Push(factory->GetType());
	}

	void Context::RegisterSubsystem(Object* object)
	{
		if (!object)
			return;

		subsystems_[object->GetType()] = object;
	}

	void Context::RemoveSubsystem(StringHash objectType)
	{
		HashMap<StringHash, SharedPtr<Object> >::Iterator i = subsystems_.Find(objectType);
		if (i != subsystems_.End())
			subsystems_.Erase(i);
	}

	void Context::RegisterAttribute(StringHash objectType, const AttributeInfo& attr)
	{
		// 不支持空或指针类型
		if (attr.type_ == VAR_NONE || attr.type_ == VAR_VOIDPTR || attr.type_ == VAR_PTR)
		{
			SAPPHIRE_LOGWARNING("Attempt to register unsupported attribute type " + Variant::GetTypeName(attr.type_) + " to class " +
				GetTypeName(objectType));
			return;
		}

		attributes_[objectType].Push(attr);

		if (attr.mode_ & AM_NET)
			networkAttributes_[objectType].Push(attr);
	}

	void Context::RemoveAttribute(StringHash objectType, const char* name)
	{
		RemoveNamedAttribute(attributes_, objectType, name);
		RemoveNamedAttribute(networkAttributes_, objectType, name);
	}

	void Context::UpdateAttributeDefaultValue(StringHash objectType, const char* name, const Variant& defaultValue)
	{
		AttributeInfo* info = GetAttribute(objectType, name);
		if (info)
			info->defaultValue_ = defaultValue;
	}

	VariantMap& Context::GetEventDataMap()
	{
		unsigned nestingLevel = eventSenders_.Size();
		while (eventDataMaps_.Size() < nestingLevel + 1)
			eventDataMaps_.Push(new VariantMap());

		VariantMap& ret = *eventDataMaps_[nestingLevel];
		ret.Clear();
		return ret;
	}


	void Context::CopyBaseAttributes(StringHash baseType, StringHash derivedType)
	{
		if (baseType == derivedType)
		{
			SAPPHIRE_LOGWARNING("Attempt to copy base attributes to itself for class " + GetTypeName(baseType));
			return;
		}

		//获取基类型的属性列表
		const Vector<AttributeInfo>* baseAttributes = GetAttributes(baseType);
		if (baseAttributes)
		{
			for (unsigned i = 0; i < baseAttributes->Size(); ++i)
			{
				const AttributeInfo& attr = baseAttributes->At(i);
				attributes_[derivedType].Push(attr);
				if (attr.mode_ & AM_NET)  //是否网络复制模式
					networkAttributes_[derivedType].Push(attr);
			}
		}
	}

	Object* Context::GetSubsystem(StringHash type) const
	{
		HashMap<StringHash, SharedPtr<Object> >::ConstIterator i = subsystems_.Find(type);
		if (i != subsystems_.End())
			return i->second_;
		else
			return 0;
	}

	Object* Context::GetEventSender() const
	{
		if (!eventSenders_.Empty())
			return eventSenders_.Back();
		else
			return 0;
	}

	const String& Context::GetTypeName(StringHash objectType) const
	{
		// Search factories to find the hash-to-name mapping
		HashMap<StringHash, SharedPtr<ObjectFactory> >::ConstIterator i = factories_.Find(objectType);
		return i != factories_.End() ? i->second_->GetTypeName() : String::EMPTY;
	}

	AttributeInfo* Context::GetAttribute(StringHash objectType, const char* name)
	{
		HashMap<StringHash, Vector<AttributeInfo> >::Iterator i = attributes_.Find(objectType);
		if (i == attributes_.End())
			return 0;

		Vector<AttributeInfo>& infos = i->second_;

		for (Vector<AttributeInfo>::Iterator j = infos.Begin(); j != infos.End(); ++j)
		{
			if (!j->name_.Compare(name, true))
				return &(*j);
		}

		return 0;
	}

	void Context::AddEventReceiver(Object* receiver, StringHash eventType)
	{
		eventReceivers_[eventType].Insert(receiver);
	}

	void Context::AddEventReceiver(Object* receiver, Object* sender, StringHash eventType)
	{
		specificEventReceivers_[sender][eventType].Insert(receiver);
	}

	void Context::RemoveEventSender(Object* sender)
	{
		HashMap<Object*, HashMap<StringHash, HashSet<Object*> > >::Iterator i = specificEventReceivers_.Find(sender);
		if (i != specificEventReceivers_.End())
		{
			for (HashMap<StringHash, HashSet<Object*> >::Iterator j = i->second_.Begin(); j != i->second_.End(); ++j)
			{
				for (HashSet<Object*>::Iterator k = j->second_.Begin(); k != j->second_.End(); ++k)
					(*k)->RemoveEventSender(sender);
			}
			specificEventReceivers_.Erase(i);
		}
	}

	void Context::RemoveEventReceiver(Object* receiver, StringHash eventType)
	{
		HashSet<Object*>* group = GetEventReceivers(eventType);
		if (group)
			group->Erase(receiver);
	}

	void Context::RemoveEventReceiver(Object* receiver, Object* sender, StringHash eventType)
	{
		HashSet<Object*>* group = GetEventReceivers(sender, eventType);
		if (group)
			group->Erase(receiver);
	}
}