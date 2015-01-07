
#include <libclientserver.h>
#include <libcameraclient.h>

int main(int argc, char **argv)
{
	CameraClient cli;
	cli.Connect("unix:/tmp/CameraServer");
	return cli.Quit();
}

