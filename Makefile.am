ACLOCAL_AMFLAGS=-I m4

SUBDIRS=libcameracommon src platforms libcameraclient cameractl


all-local:
	xbuild /nologo /verbosity:quiet WebUI.sln

install-exec-hook:
	ln -s $(libdir)/libcameraclient_csharp.so.0.0.0 $(libdir)/libCameraClientWrapper.so
	ln -s $(libdir)/libcameraclient_csharp.so.0.0.0 /usr/lib/libCameraClientWrapper.so
	ldconfig
	mkdir -p $(prefix)/share/camera/webui
	rsync -az --exclude '*.cs' WebUI/ $(prefix)/share/camera/webui

uninstall-hook:
	echo $(prefix)
	-rm $(libdir)/libCameraClientWrapper.so
	-rm /usr/lib/libCameraClientWrapper.so
	ldconfig

clean-local:
	xbuild /nologo /verbosity:quiet /target:clean WebUI.sln


