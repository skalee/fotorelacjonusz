#include "uploaderfactory.h"

#include "isanonyuploader.h"
#include "iscodeuploader.h"
#include "isloginuploader.h"
#include "ftpuploader.h"
#include "imguranonyuploader.h"
#include "imgurloginuploader.h"

UploaderFactory::AbstractWrapper::AbstractWrapper(QString name):
	name(name)
{
}

UploaderFactory::AbstractWrapper::~AbstractWrapper()
{
}

template <class T>
class Wrapper : public UploaderFactory::AbstractWrapper
{
public:
	Wrapper(QString name):
		AbstractWrapper(name)
	{}

	virtual ~Wrapper()
	{
		delete object;
	}

	AbstractUploader *make(QWidget *w, QSettings &s)
	{
		return !object.isNull() ? object : object = new T(w, s);
	}

private:
	QPointer<T> object;
};

#define UPLOADER(T, S) uploaders.append(new Wrapper<T>(S));

UploaderFactory::UploaderFactory()
{
	UPLOADER(IsAnonyUploader, tr("Imageshack anonimowo"));
	UPLOADER(IsCodeUploader, tr("Imageshack kod rejestracyjny"));
	UPLOADER(IsLoginUploader, tr("Imageshack login i hasło"));
//	TODO Re-enable me!  (See ftpuploader.h)
//	UPLOADER(FtpUploader, tr("Własne konto FTP"));
	UPLOADER(ImgurAnonyUploader, tr("Imgur anonimowo"));
	UPLOADER(ImgurLoginUploader, tr("Imgur konto"));
}

UploaderFactory::~UploaderFactory()
{
	qDeleteAll(uploaders);
}


