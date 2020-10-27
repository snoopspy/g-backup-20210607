#include <GJson>
#include "base/log/glogmanager.h"
#include "base/log/glogdbwin32.h"
#include "base/log/glogfile.h"
#include "base/log/glogstderr.h"
#include "base/log/glogstdout.h"

int main() {
	GLogManager& logManager = GLogManager::instance();
#ifdef Q_OS_WINDOWS
	logManager.push_back(new GLogDbWin32);
#endif // Q_OS_WINDOWS
	logManager.push_back(new GLogFile);
	logManager.push_back(new GLogStderr);
	logManager.push_back(new GLogStdout);

	QJsonArray ja;
	logManager.save(ja);
	QJsonObject jo;
	jo["logs"] = ja;

	GJson::saveToFile(jo, "sslog.ss");
}
