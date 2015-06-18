
#include <libclientserver.h>
#include <libcameraclient.h>

int main(int argc, char **argv)
{
	CameraClient cli;
	cli.Connect("unix:/tmp/CameraServer");
	if (argc < 2)
	{
		printf("%d\n", -EINVAL);
		return -EINVAL;
	}
	int value = atoi(argv[1]);
	int ret = cli.WebServerSetPort(value);
	printf("%d\n", ret);
	return ret;
}

