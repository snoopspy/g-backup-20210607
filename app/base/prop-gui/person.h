#pragma once

#include <GObj>

class G_EXPORT Person : public GObj {
	Q_OBJECT
	Q_PROPERTY(QString name MEMBER name_)
	Q_PROPERTY(int age MEMBER age_)
	Q_PROPERTY(Sex sex MEMBER sex_)
	Q_PROPERTY(QString picture MEMBER picture_)
	Q_ENUMS(Sex)

public:
	enum Sex {
		Unknown = 100,
		Male = 200,
		Female = 300
	};

public:
	QString name_;
	int age_{0};
	Sex sex_{Male};
	QString picture_;

#ifdef QT_GUI_LIB
	GPropItem* propCreateItem(GPropItemParam* param) override;
#endif // QT_GUI_LIB
};
