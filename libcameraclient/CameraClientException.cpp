
#include <CameraClientException.h>

CameraClientException::CameraClientException()
{
	m_msg = "Unknown";
}

CameraClientException::CameraClientException(int error)
{
	m_msg = strerror(abs(error));
}

CameraClientException::CameraClientException(const std::string msg)
{
	m_msg = msg;
}

CameraClientException::~CameraClientException()
{

}

const char* CameraClientException::what() const throw ()
{
	return m_msg.c_str();
}

