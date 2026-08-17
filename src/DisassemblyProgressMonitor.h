#pragma once

#include "Syntax.h"
#include "SegmentTypes.h"

#include <chrono>

class Disassembly;

class DisassemblyProgressMonitor {

public:
	DisassemblyProgressMonitor() = default;
	virtual ~DisassemblyProgressMonitor() = default;

	void SetVerbose(bool verbose);
	bool IsVerbose() const;

	std::chrono::microseconds StartDisassembly(Disassembly& disassembly);
	virtual void SetPass(wstring_view pass);
	virtual void SetSegmentNumber(SEGMENT_NUMBER segmentNumber);
	virtual void SendInfo(wstring_view message);

	virtual bool IsCancelled();

protected:
	wstring pass;

	virtual void DisassembleInternal(Disassembly& disassembly);

private:
	bool verbose = false;

};