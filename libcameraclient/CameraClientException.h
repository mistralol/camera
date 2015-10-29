
#include <exception>
#include <string.h>
#include <string>

class CameraClientException : public std::exception
{
	public:
		CameraClientException();
		CameraClientException(int error);
		CameraClientException(const std::string msg);
		~CameraClientException();

		virtual const char* what() const throw ();
	private:
		std::string m_msg;
};

