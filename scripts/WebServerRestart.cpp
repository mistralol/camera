
#include <libclientserver.h>
#include <libcameraclient.h>

int main(int argc, char **argv)
{
	CameraClient cli;
	std::string version = "";
	cli.Connect("unix:/tmp/CameraServer");
	return cli.WebServerRestart();
}

