// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Domain.h"
#include "Core/Logger.h"

Ptr<Assembly> Domain::loadAssembly(const char* fileName)
{
	auto masm = mono_domain_assembly_open(m_domain, fileName);

	if (!masm)
		return nullptr;

	Ptr<Assembly> assembly(new Assembly);
	assembly->m_assembly = masm;

	Logger::write("Loaded assembly '", fileName, "'", LogLevel::Info);

	return assembly;
}

void Domain::cleanup()
{
	mono_jit_cleanup(m_domain);
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

	domain->m_domain = nullptr; // TODO: create domain

	return domain;
}
