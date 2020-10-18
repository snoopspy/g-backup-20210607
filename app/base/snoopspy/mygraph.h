#pragma once

#include <GPluginFactory>

struct MyFactory : GPluginFactory {
	MyFactory(QObject* parent = nullptr);
	~MyFactory() override;
};
