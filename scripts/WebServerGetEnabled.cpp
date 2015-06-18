
#include <libclientserver.h>
#include <libcameraclient.h>

int main(int argc, char **argv)
{
        CameraClient cli;
        cli.Connect("unix:/tmp/CameraServer");
        int enabled = -1;
        int ret = cli.WebServerGetEnabled(&enabled);
	if (ret < 0)
	{
		printf("%d\n", ret);
		return ret;
	}
        printf("%d\n", enabled);
        return ret;
}

