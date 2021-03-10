#include <QDebug>
#include <GErr>

using namespace std;

struct Obj {
	GErr& err() {
		return err_;
	}
private:
	GErr err_ { GErr::FAIL, "NOT_SUPPORTED in Obj class" };
};

int main() {
	{
		GErr err;
		qDebug() << err << Qt::endl;
	}

	{
		GErr err{ GErr::NOT_SUPPORTED };
		qDebug() << err << Qt::endl;
	}

	{
		GErr err { GErr::NOT_SUPPORTED, "NOT_SUPPORTED" };
		qDebug() << err << Qt::endl;
	}

	{
		Obj obj;
		GErr& err = obj.err();
		qDebug() << err << Qt::endl;
	}
}
