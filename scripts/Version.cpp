
#include <libclientserver.h>
#include <libcameraclient.h>

int main(int argc, char **argv)
{
	CameraClient cli;
	std::string version = "";
	cli.Connect("unix:/tmp/CameraServer");
	int ret = cli.Version(&version);
	if (ret == 0)
		printf("%s\n", version.c_str());
	return ret;
}

