#pragma once

#include <GPluginFactory>

struct MyFactory : GPluginFactory {
	MyFactory(QObject* parent = nullptr);
	~MyFactory() override;

	void loadBlock();
	void loadCapture();
	void loadConvert();
	void loadDelay();
	void loadExtract();
	void loadFilter();
	void loadFlow();
	void loadOther();
	void loadProcess();
	void loadWrite();
};
