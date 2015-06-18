
#include <libclientserver.h>
#include <libcameraclient.h>

int main(int argc, char **argv)
{
	CameraClient cli;
	cli.Connect("unix:/tmp/CameraServer");
	int value = 0;
	int ret = cli.WebServerGetPort(&value);
	printf("%d\n", value);
	return ret;
}

