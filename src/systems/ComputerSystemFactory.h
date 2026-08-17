#pragma once

#include "Syntax.h"
#include "ComputerSystemType.h"
#include "Atari5200.h"
#include "Atari800.h"
#include "C64.h"
#include "Oric.h"
#include "Unknown.h"

class ComputerSystem;

class ComputerSystemFactory final {
public:
	ComputerSystemFactory();

	ComputerSystemType GetComputerSystemType(wstring_view id) const;
	gsl::not_null<const ComputerSystemTypeInfo*> GetComputerSystemTypeInfo(ComputerSystemType type) const;
	gsl::not_null<const ComputerSystem*> GetComputerSystem(ComputerSystemType type) const;

private:
    std::unique_ptr<Atari800> atari800 = nullptr;
    std::unique_ptr<Atari5200> atari5200 = nullptr;
    std::unique_ptr<C64> c64 = nullptr;
    std::unique_ptr<Oric> oric = nullptr;
    std::unique_ptr<systems::Unknown> unknown = nullptr;
};
