

all:
	$(MAKE) -C server
	$(MAKE) -C libcameraclient
	$(MAKE) -C scripts

clean:
	$(MAKE) -C server clean
	$(MAKE) -C libcameraclient clean
	$(MAKE) -C scripts clean


