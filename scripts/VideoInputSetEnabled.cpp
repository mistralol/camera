#include <libclientserver.h>
#include <libcameraclient.h>

int main(int argc, char **argv)
{
        CameraClient cli;
        cli.Connect("unix:/tmp/CameraServer");
        if (argc < 3)
        {
                printf("%d\n", -EINVAL);
                return -EINVAL;
        }
        int input = atoi(argv[1]);
        int enabled = atoi(argv[2]);
        int ret = cli.VideoInputSetEnabled(input, enabled);
        printf("%d\n", ret);
        return ret;
}

