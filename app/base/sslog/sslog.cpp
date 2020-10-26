#include <GJson>
#include "base/log/glogmanager.h"
#ifdef Q_OS_WINDOWS
#include "base/log/glogdbwin32.h"
#endif // Q_OS_WINDOWS
#include "base/log/glogfile.h"
#include "base/log/glogstderr.h"
#include "base/log/glogstdout.h"

int main() {
	GLogManager& logManager = GLogManager::instance();
	logManager.push_back(new GLogFile);
	logManager.push_back(new GLogStderr);
	logManager.push_back(new GLogStdout);

	QJsonArray ja;
	logManager.save(ja);
	QJsonObject jo;
	jo["logs"] = ja;

	GJson::saveToFile(jo, "sslog.ss");
}
