
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
        int stream = atoi(argv[1]);
	int enabled = -1;
        int ret = cli.VideoStreamGetEnabled(stream, &enabled);
	if (ret < 0)
	{
		printf("%d\n", ret);
		return ret;
	}
        printf("%d\n", enabled);
        return ret;
}

