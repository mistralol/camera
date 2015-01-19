

all:
	if [ ! -d build ] ; then mkdir build ; fi
	$(MAKE) -C server
	$(MAKE) -C libcameraclient
	$(MAKE) -C scripts

clean:
	$(MAKE) -C server clean
	$(MAKE) -C libcameraclient clean
	$(MAKE) -C scripts clean
	rm -rf build


