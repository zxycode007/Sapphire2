#include "Context.h"
#include "Timer.h"
#include "WorkQueue.h"
#include "FileSystem.h"
#include "Log.h"
#include "Resource\ResourceCache.h"
#include "Engine.h"
#include "ShaderManager.h"
#include "IVideoDriver.h"

Sapphire::Engine::Engine(Context * ctx):Object(ctx)
{

}

void Sapphire::Engine::initialize()
{
	context_->RegisterSubsystem(new Time(context_));
	context_->RegisterSubsystem(new WorkQueue(context_));
	context_->RegisterSubsystem(new FileSystem(context_));
	context_->RegisterSubsystem(new Log(context_));
	context_->RegisterSubsystem(new ResourceCache(context_));
	context_->RegisterSubsystem(new VideoDriver(context_));
	context_->RegisterSubsystem(new ShaderManager(context_));
}

