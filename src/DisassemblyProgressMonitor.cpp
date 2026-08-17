#include "DisassemblyProgressMonitor.h"
#include "Disassembly.h"
#include "Application.h"


extern std::unique_ptr<Application> g_Application;


void DisassemblyProgressMonitor::SetVerbose(bool verbose) {
	this->verbose = verbose;
}
bool DisassemblyProgressMonitor::IsVerbose() const {
	return verbose;
}

std::chrono::microseconds DisassemblyProgressMonitor::StartDisassembly(Disassembly& disassembly) {
	using namespace std::chrono;

	const auto start = high_resolution_clock::now();
	DisassembleInternal(disassembly);
	const auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	return duration;
}

void DisassemblyProgressMonitor::DisassembleInternal(Disassembly& disassembly) {
	disassembly.DisassembleInternal();
};

void DisassemblyProgressMonitor::SetPass(wstring_view pass) {
	this->pass = pass;
	g_Application->SendInfoMessageWithID(IDS_LOG_DISASSEMBLY_PROGRESS_MONITOR_PASS, this->pass.c_str());
};

void DisassemblyProgressMonitor::SetSegmentNumber(SEGMENT_NUMBER segmentNumber) {
	g_Application->SendInfoMessageWithID(IDS_LOG_DISASSEMBLY_PROGRESS_MONITOR_SEGMENT, std::to_wstring(segmentNumber));
};

void DisassemblyProgressMonitor::SendInfo(wstring_view message) {
	g_Application->SendInfoMessageWithID(IDS_LOG_DISASSEMBLY_PROGRESS_MONITOR_INFO, pass, message);
}


bool DisassemblyProgressMonitor::IsCancelled() {
	return false;
};