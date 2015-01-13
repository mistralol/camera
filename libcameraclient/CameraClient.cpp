
#include <libclientserver.h>

#include <libcameraclient.h>

CameraClient::CameraClient()
{

}

CameraClient::~CameraClient()
{
	if (m_Client)
	{
		Disconnect();
	}
}

bool CameraClient::Connect(const std::string ConnectionString)
{
	m_Client = Client::Create(ConnectionString);
	m_Client->Connect();
	m_Client->WaitForConnect();
	return true;
}

void CameraClient::Disconnect()
{
	m_Client->Disconnect();
	delete m_Client;
	m_Client = NULL;
}

int CameraClient::RTSPGetClientCount(int *value)
{
	PerfCounter PC("RTSPGetClientCount");
	Request request;
	Request response;

	request.SetCommand("RTSPGetClientCount");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (response.GetInt("value", value) == false)
		return -EINVAL;
	
	return ret;
}

int CameraClient::RTSPSetMaxClients(int max)
{
	PerfCounter PC("RTSPSetMaxClients");
	Request request;
	Request response;

	request.SetCommand("RTSPSetMaxClients");
	request.SetArg("value", max);
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::RTSPGetMaxClients(int *max)
{
	PerfCounter PC("RTSPGetMaxClients");
	Request request;
	Request response;

	request.SetCommand("RTSPGetMaxClients");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (response.GetInt("value", max) == false)
		return -EINVAL;

	return ret;
}

int CameraClient::RTSPSetMaxBacklog(int max)
{
	PerfCounter PC("RTSPSetMaxBacklog");
	Request request;
	Request response;

	request.SetCommand("RTSPSetMaxBacklog");
	request.SetArg("value", max);
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::RTSPGetMaxBacklog(int *max)
{
	PerfCounter PC("RTSPGetMaxBacklog");
	Request request;
	Request response;

	request.SetCommand("RTSPGetMaxBacklog");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (response.GetInt("value", max) == false)
		return -EINVAL;

	return ret;
}

int CameraClient::Version(std::string *str)
{
	PerfCounter PC("VERSION");
	Request request;
	Request response;

	request.SetCommand("PING");
	int ret = m_Client->SendRequest(&request, &response);
	if (ret < 0)
		return ret;
	if (response.GetString("value", str) == false)
		return -EINVAL;
	return ret;
}

int CameraClient::Ping()
{
	PerfCounter PC("PING");
	Request request;
	Request response;

	request.SetCommand("PING");
	return m_Client->SendRequest(&request, &response);
}

int CameraClient::Quit()
{
	PerfCounter PC("QUIT");
	Request request;
	Request response;

	request.SetCommand("QUIT");
	return m_Client->SendRequest(&request, &response);
}


