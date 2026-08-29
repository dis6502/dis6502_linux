#include "Application.h"
#include "Main.h"
#include "MainWindow.h"
#include "MemoryInspector.h"

#include "MainController.h"

extern std::unique_ptr<Application> g_Application;

extern std::unique_ptr<MemoryInspector> g_MemoryInspector;


MainController::MainController(Main& main) : main(&main) { // TODO Make init or constructor parameters
	this->application = g_Application.get();
	this->mainWindow = this->main->GetMainWindow();
}

void MainController::UpdateDisassembly(bool bForce) {
	main->UpdateDisassembly(bForce);
}

bool MainController::IsEditMode() const {
	return ::g_MemoryInspector->IsEditMode();
}

void MainController::QuitEditMode() {
	if (::g_MemoryInspector->SetEditMode(false)) {
		UpdateDisassembly();
	}
}
