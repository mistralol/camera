
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


