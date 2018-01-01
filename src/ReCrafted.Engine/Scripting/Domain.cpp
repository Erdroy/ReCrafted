// ReCrafted (c) 2016-2018 Always Too Late

#include "Domain.h"
#include "Core/Logger.h"
#include "Core/GameInfo.h"
#include "Core/GameMain.h"

Ptr<Domain> Domain::Root;

const char* jit_options[] = {
	"--soft-breakpoints",
	"--debugger-agent=transport=dt_socket,address=127.0.0.1:55000"
};

const char* rootDomainName = "ReCrafted";
const char* runtimeVersion = "v4.0.30319";

Ptr<Assembly> Domain::loadAssembly(const char* fileName)
{
	// do mono related stuff
	auto masm = mono_domain_assembly_open(m_domain, fileName);
	auto mimg = mono_assembly_get_image(masm);

	if (!masm || !mimg)
	{
		Logger::logError("Failed to load assembly '{0}'", fileName);
		return nullptr;
	}

	// create assembly instance
	Ptr<Assembly> assembly(new Assembly);
	assembly->m_assembly = masm;
	assembly->m_image = mimg;
	assembly->m_domain = m_domain;

	// add to the loaded assembly list
	m_loadedAssemblies.add(assembly);

	Logger::logInfo("Loaded assembly '{0}'", fileName);

	return assembly;
}

void Domain::cleanup()
{
	//mono_jit_cleanup(m_domain);
	m_domain = nullptr;
}

MonoDomain* Domain::getMono() const
{
	return m_domain;
}

Ptr<Domain> Domain::create(MonoDomain* monoDomain)
{
	Ptr<Domain> domain(new Domain);
	domain->m_domain = monoDomain;
	return domain;
}

Ptr<Domain> Domain::create(const char* name, Ptr<Domain> parent)
{
	Ptr<Domain> domain(new Domain);

	domain->m_domain = mono_domain_create();

	return domain;
}

Ptr<Domain> Domain::createRoot()
{
	if(Root)
	{
		Logger::logWarning("Cannot create second root domain!");
		return nullptr;
	}

	mono_set_dirs("../mono/lib", "../mono/etc");

	if (GameInfo::containsArgument(TEXT("-debug")) && !GameInfo::containsArgument(TEXT("-nodebug")))
	{
		mono_jit_parse_options(2, const_cast<char**>(jit_options));
	}

	auto domain = mono_jit_init_version(rootDomainName, runtimeVersion);

	if(!domain)
	{
		Logger::logException("Failed to create root domain! Domain Name: {0} Runtime Version: {1}", rootDomainName, runtimeVersion);
		GameMain::quit(); // quit
		return nullptr;
	}

	if (!GameInfo::containsArgument(TEXT("-nodebug")))
	{
		mono_debug_init(MONO_DEBUG_FORMAT_MONO);
		mono_debug_domain_create(domain);
	}

	// create instance
	Root = create(domain);
	return Root;
}
